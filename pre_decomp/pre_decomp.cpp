//
// Created by tripack on 17-4-9.
//

#include "pre_decomp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using std::ifstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::cerr;
using std::cout;
using std::endl;

PreDecomp* PreDecomp::instance = nullptr;

static int unPow(int x) {
    switch (x) {
        case 1: return 0;
        case 2: return 1;
        case 4: return 2;
        case 8: return 3;
        case 16: return 4;
        case 32: return 5;
        case 64: return 6;
        default:
            assert(0);
    }
}

PreDecomp::PreDecomp(const std::string &libName) {
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
}

PreDecomp &PreDecomp::getInstance() {
    if (instance == nullptr) {
        instance = new PreDecomp;
    }
    return *instance;
}

PreDecomp::~PreDecomp() {}

const PreDecomp::DbEntry& PreDecomp::getMatch(const std::string &funStr, size_t inputSize) {
    auto& metadataSet = metaData.at(inputSize);
    std::bitset<64> function(funStr);
    std::bitset<64> validMask((1ull << (1ull << inputSize)) - 1);
    function &= validMask;
    size_t minError = 64;
    const DbEntry* minEntry = nullptr;
    for (size_t i = 0; i < metadataSet.size(); i++) {
        size_t error = (metadataSet[i]^ function).count();
        if (error < minError) {
            minEntry = &(data.at(inputSize).at(i));
            minError = error;
        }
    }
    return *minEntry;
}
