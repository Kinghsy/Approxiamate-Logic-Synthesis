//
// Created by tripack on 17-4-9.
//
#include <algorithm>
#include <iostream>
#include "../circuit/interface.h"
#include "sim_profile.h"

using std::vector;
using std::string;

SimulationResult::SimulationResult(
        const BlifBooleanNet &net, size_t nSamples) {
    size_t nInputs = net.nInputs();
    size_t nOutputs = net.nOutputs();
    size_t nInternals = net.internalNodeSet().size();

    this->inputResult.resize(nSamples);
    this->outputResult.resize(nSamples);
    this->internalResult.resize(nSamples);

    for (auto& v : inputResult) v.resize(nInputs);
    for (auto& v : outputResult) v.resize(nOutputs);
    for (auto& v : internalResult) v.resize(nInternals);

    this->nSamples = nSamples;
}

FocusedSimulationResult::FocusedSimulationResult
        (const SimulationResult &result,
         const std::vector<std::string> &node)
        : nodeOrder(node) {
    size_t size = node.size();
    if (size > 16) {
        std::cerr << "Cannot not focus on more than 16 nodes" << std::endl;
        assert(0);
    }
    enum NODE_TYPE {INPUT, OUTPUT, INTERNAL};
    std::vector<NODE_TYPE> type;
    std::vector<size_t> index;

    for (const auto& n : node) {
        const vector<string> *container = &result.inputName;
        auto it = std::find(container->begin(), container->end(), n);
        if (it != container->end()) {
            type.push_back(INPUT);
            index.push_back(it - container->begin());
            continue;
        }
        container = &result.outputName;
        it = std::find(container->begin(), container->end(), n);
        if (it != container->end()) {
            type.push_back(OUTPUT);
            index.push_back(it - container->begin());
            continue;
        }
        container = &result.internalName;
        it = std::find(container->begin(), container->end(), n);
        if (it != container->end()) {
            type.push_back(INTERNAL);
            index.push_back(it - container->begin());
            continue;
        }
        std::cerr << "Cannot find node by name: " << n << std::endl;
        assert(0);
    }


    for (int j = 0; j < result.nSamples; ++j) {
        std::string function;
        for (int i = 0; i < size; ++i) {
            size_t ind = index[i];
            const std::vector<int>& inputResult = result.inputResult[j];
            const std::vector<int>& outputResult = result.outputResult[j];
            const std::vector<int>& internalResult = result.internalResult[j];
            int result = 0;
            switch (type[i]) {
                case INPUT:
                    result = inputResult.at(ind);
                    break;
                case OUTPUT:
                    result = outputResult.at(ind);
                    break;
                case INTERNAL:
                    result = internalResult.at(ind);
                    break;
                default:
                    assert(0);
            }
            if (result == 0)
                function.push_back('0');
            else
                function.push_back('1');
        }
        // TODO: Could be problematic? Standard should have specified int()==0
        data[function]++;
    }
}

size_t FocusedSimulationResult::count(const std::string &term) const {
    return data.at(term);
}

size_t FocusedSimulationResult::count(const std::vector<std::string> &termSet) const {
    size_t sum = 0;
    for (const std::string& term : termSet) {
        sum += data.at(term);
    }
    return sum;
}

FocusedSimulationResult
SimulationResult::focus(const std::vector<std::string> &node) const {
    return FocusedSimulationResult(*this, node);
}
