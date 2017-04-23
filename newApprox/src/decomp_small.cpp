//
// Created by tripack on 4/22/17.
//

#include <iostream>
#include <common.h>
#include "decomp_small.h"
#include "../../circuit/interface.h"
#include "../../pre_decomp/pre_decomp.h"

using namespace std;

IAlgorithmDecompose::ResultType
AlgorithmDecomposeSmall::operator()
        (const BoolFunction &bf, const SimulationResult &simData) {


    TTable table = bf.getTTable();
    inputs = bf.inputPorts();
    output = bf.getOutPortName();

    this->nameGenerator = NodeNameGenerator(output);

    auto focusedResult = simData.focus(inputs);
    auto &preDecomp = PreDecomp::getInstance();
    auto &match = this->match(table.cdata(), inputs.size(), focusedResult);

    size_t matchError = countMatchError(table, match.function, focusedResult);

    //std::cout << table << endl;
    //std::cout << match.function << endl;
    //std::cout << "\tMatchError: " << matchError << "\t";
    //std::cout << "discardInput: " << match.discardMask.count() << endl;

    /*for (size_t term = 0; term < focusedResult.cdata.size(); term++) {
        if ((table ^ match.function)[term])
            std::cout << term << ":" << focusedResult.cdata[term] << "; ";
    }*/
    //std::cout << endl;
    auto blifBuilder = buildFromMatch(match, inputs, output);

    //std::cout << "BuilderSize: " << blifBuilder.nNode() << "\n";

    ResultType rst;

    rst.fun = BoolFunction(bf.getInputSize(), match.function,
                           inputs, bf.getOutPortName());
    rst.deInfo = blifBuilder;
    rst.errorCount = matchError;

    return rst;
}

BlifBuilder AlgorithmDecomposeSmall::buildFromMatch
        (const PreDecomp::DbEntry &entry,
         const std::vector<NodeName> &node,
         const NodeName &thisNode) {

    assert(entry.nInputs == node.size());

    const auto& preDecomp = PreDecomp::getInstance();
    const auto& leftFun = entry.left;
    const auto& rightFun = entry.right;

    if (entry.combine == ALL_IRR_TABLE_0) {
        return BlifBuilder::buildConst(thisNode, false);
    } else if (entry.combine == ALL_IRR_TABLE_1) {
        return BlifBuilder::buildConst(thisNode, true);
    }

    BlifBuilder  leftBlif = BlifBuilder::buildConst("none", true);
    BlifBuilder rightBlif = BlifBuilder::buildConst("none", true);

    if (leftFun.nInputs() == 1) {
        size_t nodeNum = entry.leftMask.find_first();
        leftBlif = BlifBuilder::buildInput(node.at(nodeNum),
                                           entry.left == NOT_1_INPUT);
    } else if (leftFun.nInputs() == 2) {
        size_t leftNum = entry.leftMask.find_first();
        size_t rightNum = entry.leftMask.find_next(leftNum);
        auto b1 = BlifBuilder::buildInput(node.at(leftNum), false);
        auto b2 = BlifBuilder::buildInput(node.at(rightNum), false);
        leftBlif = combineBilfBuilder(b1, b2, entry.left,
                                      nameGenerator.generate());
    } else {
        const auto leftMatch = preDecomp.getMatch(leftFun.cdata(), leftFun.nInputs());
        leftBlif = buildFromMatch(leftMatch,
                                  pickByDbitset(node, entry.leftMask),
                                  nameGenerator.generate());
    }

    if (rightFun.nInputs() == 1) {
        size_t nodeNum = entry.rightMask.find_first();
        rightBlif = BlifBuilder::buildInput(node.at(nodeNum),
                                            entry.right == NOT_1_INPUT);
    } else if (rightFun.nInputs() == 2) {
        size_t leftNum = entry.rightMask.find_first();
        size_t rightNum = entry.rightMask.find_next(leftNum);
        auto b1 = BlifBuilder::buildInput(node.at(leftNum), false);
        auto b2 = BlifBuilder::buildInput(node.at(rightNum), false);
        rightBlif = combineBilfBuilder(b1, b2, entry.right,
                                       nameGenerator.generate());
    } else {
        const auto rightMatch = preDecomp.getMatch(rightFun.cdata(),
                                                   rightFun.nInputs());
        rightBlif = buildFromMatch(rightMatch,
                                   pickByDbitset(node, entry.rightMask),
                                   nameGenerator.generate());
    }

    return combineBilfBuilder(leftBlif, rightBlif, entry.combine, thisNode);
}

const PreDecomp::DbEntry &
AlgorithmDecomposeSmall::match(const DBitset &fun, size_t inputSize,
                               const FocusedSimulationResult &simResult) {
    if (inputSize > 6) assert(0);
    auto& pd = PreDecomp::getInstance();
    const auto& metaData = pd.cmetaData;
    const auto& data = pd.cdata;


    assert(inputSize == simResult.nodeOrder.size());
    auto& metadataSet = metaData.at(inputSize);
    std::bitset<64> function(fun.to_ulong());
    DBitset mask = ones(64, 1ul << inputSize);
    std::bitset<64> validMask(mask.to_ulong());
    function &= validMask;


    if (inputSize <= 5) { // Compare error on all ouputs
        const PreDecomp::DbEntry* minEntry = nullptr;
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
        const PreDecomp::DbEntry* minEntry = nullptr;
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
            const PreDecomp::DbEntry* entry;
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

}
