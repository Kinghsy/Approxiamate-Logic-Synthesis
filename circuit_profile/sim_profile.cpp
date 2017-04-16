//
// Created by tripack on 17-4-9.
//
#include <algorithm>
#include <iostream>
#include "../circuit/interface.h"
#include "sim_profile.h"
#include "../newApprox/src/const.h"

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
        : nodeOrder(node),
          data(1ul << (node.size()), 0),
          cdata(data) {

    assert(data.size() == (1ul << node.size()));

    size_t size = node.size();
    if (size > 20) {
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
        DBitset dBitset(size);
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
            dBitset[i] = result;
        }
        data[dBitset.to_ulong()]++;
    }

    size_t sum = 0;
    for (auto &&datum : data) sum += datum;
    assert(sum == result.nSamples);
    this->nSamples = sum;
}

size_t FocusedSimulationResult::count(const std::string &term) const {
    return data.at(std::bitset<64>(term).to_ullong());
}

size_t FocusedSimulationResult::count(const std::vector<std::string> &termSet) const {
    size_t sum = 0;
    for (const std::string& term : termSet) {
        sum += data.at(std::bitset<64>(term).to_ullong());
    }
    return sum;
}

size_t FocusedSimulationResult::count(const DBitset &term) const {
    assert(term.size() == this->nodeOrder.size());
    return data.at(term.to_ulong());
}

size_t FocusedSimulationResult::count(const vector<DBitset> &termSet) const{
    assert(termSet[0].size() == this->nodeOrder.size());
    size_t count = 0;
    for (const auto& term : termSet) {
        count += data.at(term.to_ulong());
    }
    return 0;
}



FocusedSimulationResult
SimulationResult::focus(const std::vector<std::string> &node) const {
    return FocusedSimulationResult(*this, node);
}

size_t countMatchError(const TTable &orignal, const TTable &next,
                       const FocusedSimulationResult &sim) {
    TTable diff = orignal ^ next;
    size_t error = 0;
    for (size_t i = 0; i < diff.nTerms(); i++) {
        if (diff[i]) error += sim.count(i);
    }
    return error;
}


std::ostream &operator<<(std::ostream &os, const FocusedSimulationResult &result) {
    os << "Focused result on\n";
    for (const auto& node : result.nodeOrder) {
        os << std::setw(7) << node << " ";
    }
    os << "\n";
    size_t count = 0;
    for (size_t i = 0; i < result.data.size(); ++i) {
        count += result.data[i];
        os << DBitset(result.nodeOrder.size(), i)
           << " " << result.data[i] << "\n";
    }
    os << "Total " << count << " samples\n";

    return os;
}
