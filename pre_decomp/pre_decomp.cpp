//
// Created by tripack on 17-4-9.
//

#include "pre_decomp.h"
#include "../circuit_profile/sim_profile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

using std::ifstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::cerr;
using std::cout;
using std::endl;

PreDecomp* PreDecomp::instance = nullptr;

PreDecomp::PreDecomp(const std::string &libName) {
    std::cout << "Initializing PreDecomp Database\n";
    std::cout << "This could take a while!\n";
    this->libPath = libName;
    ifstream db(libPath);
    int version = 0;
    db >> version;
    if (!db || version < MINIMUM_VERSION) {
        cerr << "Cannot determine database version or database out of date." << endl;
        cerr << "The mininum supported database version is " << MINIMUM_VERSION << endl;
        cerr << "Please run 'precomp_rel' to regenerate the database" << endl;
        exit(-1);
    }
    metaData.resize(7); // Currently 6
    data.resize(7); // Currently 6
    string line;
    while (std::getline(db, line)) {

        if (line.empty()) continue;

        std::stringstream ss(line);
        DbEntry entry;

        std::string combineStr;
        std::string functionStr;


        ss >> entry.nInputs >> functionStr;
        ss >> entry.leftMask >> entry.rightMask
           >> entry.left >> entry.right
           >> combineStr >> entry.discardMask;

        std::bitset<64> keyFun(functionStr);
        entry.function = TTable(functionStr);

        if (combineStr == "AND") {
            entry.combine = AND_TABLE;
        } else if (combineStr == "NAND") {
            entry.combine = ~AND_TABLE;
        } else if (combineStr == "XOR") {
            entry.combine = XOR_TABLE;
        } else if (combineStr == "NXOR") {
            entry.combine = ~XOR_TABLE;
        } else if (combineStr == "LEFT") {
            entry.combine = LEFT_RELA_TABLE;
        } else if (combineStr == "RIGHT") {
            entry.combine = RIGHT_RELA_TABLE;
        } else if (combineStr == "CONST_ZERO") {
            entry.combine = ALL_IRR_TABLE_0;
        } else if (combineStr == "CONST_ONE") {
            entry.combine = ALL_IRR_TABLE_1;
        } else {
            assert(0);
        }

        if (!ss) {
            if (line != "") {
                cerr << "Cannot parse line:\n" << line << endl;
                cerr << "Got:"
                     << entry.nInputs << "\t"
                     << entry.function.toString() << "\t"
                     << bitset2Str(entry.leftMask) << "\t"
                     << bitset2Str(entry.rightMask) << "\t"
                     << entry.left.toString() << "\t"
                     << entry.right.toString() << "\t"
                     << entry.combine.toString() << "\t"
                     << bitset2Str(entry.discardMask) << "\t";
                cerr << endl;
            }
            continue;
        }

        entry.nInputs = entry.function.nInputs();
        metaData[entry.nInputs].push_back(keyFun);
        data[entry.nInputs].push_back(entry);
    }
    db.close();

    std::cout << "Done!\n";
}

PreDecomp &PreDecomp::getInstance() {
    if (instance == nullptr) {
        instance = new PreDecomp;
    }
    return *instance;
}

const PreDecomp::DbEntry&
PreDecomp::getMatch(const DBitset &funStr, size_t inputSize) const {
    auto& metadataSet = metaData.at(inputSize);
    std::bitset<64> function(funStr.to_ulong());
    DBitset mask = ones(64, 1ul << inputSize);
    std::bitset<64> validMask(mask.to_ulong());
    //std::cout << function << endl;
    //std::cout << validMask << endl;
    function &= validMask;
    size_t minError = 64;
    const DbEntry* minEntry = nullptr;
    for (size_t i = 0; i < metadataSet.size(); i++) {
        size_t error = (metadataSet[i] ^ function).count();
        if (error < minError) {
            //std::cout << metadataSet[i] << endl;
            minEntry = &(data.at(inputSize).at(i));
            minError = error;
        }
    }
    return *minEntry;
}


const PreDecomp::DbEntry &
PreDecomp::getMatch(const DBitset &funStr, size_t inputSize,
                    const FocusedSimulationResult& simResult,
                    const float SIZE_ERROR_BONUS) const {
    if (inputSize > 6) assert(0);
    assert(inputSize == simResult.nodeOrder.size());
    auto& metadataSet = metaData.at(inputSize);
    std::bitset<64> function(funStr.to_ulong());
    DBitset mask = ones(64, 1ul << inputSize);
    std::bitset<64> validMask(mask.to_ulong());
    function &= validMask;



    if (inputSize <= 5) { // Compare error on all ouputs
        const DbEntry* minEntry = nullptr;
        long minError = LONG_MAX;
        for (size_t i = 0; i < metadataSet.size(); i++) {
            auto diff = metadataSet[i]^ function;
            size_t error = 0;
            for (size_t ind = 0; ind < (1ul << inputSize); ind++) {
                if (diff.test(ind)) error += simResult.count(ind);
            }
            size_t nDiscard = data.at(inputSize).at(i).discardMask.count();
            float weightedErr = error - SIZE_ERROR_BONUS * nDiscard * simResult.cnSamples;
            if (weightedErr < minError) {
                minEntry = &(data.at(inputSize).at(i));
                minError = lroundf(weightedErr);
            }
        }
        assert(minEntry);
        return *minEntry;
    } else {
        const DbEntry* minEntry = nullptr;
        long minError = LONG_MAX;

        size_t nTerms = 1ul << inputSize;

        std::vector<size_t> errorByTerm = simResult.cdata;
        std::sort(errorByTerm.begin(), errorByTerm.end());
        size_t mid = errorByTerm[nTerms >> 1] + errorByTerm[(nTerms >> 1) - 1];
        mid = mid >> 1;

        std::bitset<64> compareMask;
        compareMask.reset();

        for (size_t ind = 0; ind < nTerms; ind++)
            if (simResult.count(ind) > mid) compareMask.set(ind);

        // Search the space, find 20 functions that
        // differs by minimum number on the non-don't care set

        const size_t CANDIDATE_NUMBER = 20;
        struct Candidate {
            const DbEntry* entry;
            std::bitset<64> function;
        };

        std::multimap<size_t, Candidate> minHeapUnmasked;
        minError = LONG_MAX;
        for (size_t i = 0; i < metadataSet.size(); i++) {
            auto diff = (metadataSet[i] ^ function).count();
            if (minHeapUnmasked.size() < CANDIDATE_NUMBER) {
                auto t = Candidate{&(data.at(inputSize).at(i)), metadataSet[i]};
                minHeapUnmasked.emplace(diff, t);
                minError = minHeapUnmasked.rbegin()->first;
                continue;
            }

            if (diff < minError) {
                minHeapUnmasked.erase(--minHeapUnmasked.end()); // Pops the last element
                auto t = Candidate{&(data.at(inputSize).at(i)), metadataSet[i]};
                minHeapUnmasked.insert(std::make_pair(diff, t));
                minError = minHeapUnmasked.rbegin()->first;
            }
        }

        std::multimap<size_t, Candidate> minHeapMasked;
        minError = LONG_MAX;
        for (size_t i = 0; i < metadataSet.size(); i++) {
            auto diffFun = (metadataSet[i]^ function) & compareMask;
            auto diff = diffFun.count();
            if (minHeapMasked.size() < CANDIDATE_NUMBER) {
                auto t = Candidate{&(data.at(inputSize).at(i)), metadataSet[i]};
                minHeapMasked.emplace(diff, t);
                minError = minHeapMasked.rbegin()->first;
                continue;
            }

            if (diff < minError) {
                minHeapMasked.erase(--minHeapMasked.end()); // Pops the last element
                auto t = Candidate{&(data.at(inputSize).at(i)), metadataSet[i]};
                minHeapMasked.emplace(diff, t);
                minError = minHeapMasked.rbegin()->first;
            }
        }

        minError = LONG_MAX;
        minEntry = nullptr;
        for (auto &&item : minHeapUnmasked) {
            auto fun = item.second.function;
            auto diffFun = fun ^ function;
            int error = 0;
            for (size_t ind = 0; ind < (1ul << inputSize); ind++) {
                if (diffFun.test(ind)) error += simResult.count(ind);
            }
            size_t nDiscard = item.second.entry->discardMask.count();
            float weightedErr = error - SIZE_ERROR_BONUS * nDiscard * simResult.cnSamples;
            if (weightedErr < minError) {
                minError = lroundf(weightedErr);
                minEntry = item.second.entry;
            }
        }

        for (auto &&item : minHeapMasked) {
            auto fun = item.second.function;
            auto diffFun = fun ^ function;
            size_t error = 0;
            for (size_t ind = 0; ind < (1ul << inputSize); ind++) {
                if (diffFun.test(ind)) error += simResult.count(ind);
            }
            size_t nDiscard = item.second.entry->discardMask.count();
            float weightedErr = error - SIZE_ERROR_BONUS * nDiscard * simResult.cnSamples;
            if (weightedErr < minError) {
                minError = lroundf(weightedErr);
                minEntry = item.second.entry;
            }
        }

        assert(minEntry);
        return *minEntry;
    }

    assert(0);
}


const PreDecomp::DbEntry &
PreDecomp::getMatch(const TTable &fun,const std::vector<NodeName> &nodeName,
                    FocusedSimulationResult simResult,
                    const float SIZE_ERROR_BONUS) const {
    assert(nodeName == simResult.nodeOrder);
    return getMatch(fun.cdata(), fun.nInputs(),
                    simResult, SIZE_ERROR_BONUS);
}


