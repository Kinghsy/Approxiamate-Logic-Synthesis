//
// Created by tripack on 17-3-30.
//
#include <resource.h>
#include <iostream>
#include <dlfcn.h>

#include "interface.h"
#include "pattern_gen.h"


BlifBooleanNet::SimulationResult::SimulationResult(
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

BlifBooleanNet::SimulationResult
BlifBooleanNet::profileBySimulation(int samples) {
    std::string library = Temp / "circuit.so";
    std::string source = Temp / "circuit.cpp";

    this->exportToCpp(source);

    std::string cmd = "g++ -shared -fPIC -Ofast -march=native " + source + " -o " + library;
    std::cout << "Executing: " << cmd << std::endl;
    system(cmd.c_str());

    std::cout << "Loading library at: " << library << std::endl;
    void* libhandle = dlopen(library.c_str(), RTLD_NOW | RTLD_LOCAL);

    assert(libhandle != nullptr);

    typedef void (*CircuitFun)(const int input[],
                               int output[],
                               int node[]);
    typedef std::vector<std::string> (*ConstVectorFun)();

    std::cout << "Accessing symbols... ";
    CircuitFun circuit = (CircuitFun)dlsym(libhandle, "circuit");
    ConstVectorFun inputNode = (ConstVectorFun)dlsym(libhandle, "inputNode");
    ConstVectorFun outputNode = (ConstVectorFun)dlsym(libhandle, "outputNode");
    ConstVectorFun internalNode = (ConstVectorFun)dlsym(libhandle, "internalNode");

    assert(circuit != nullptr);
    assert(inputNode != nullptr);
    assert(outputNode != nullptr);
    assert(internalNode != nullptr);

    std::cout << "Done." << std::endl;

    std::cout << "Begin simulation" << std::endl;

    InfiniteRandomPatternGenerator g(this->nInputs());

    BlifBooleanNet::SimulationResult result(*this, samples);

    result.inputName = inputNode();
    result.outputName = outputNode();
    result.internalName = internalNode();

    for (int j = 0; j < samples; j++) {
        auto& inputVec = result.inputResult[j];
        inputVec = g.generate();

        auto& outputVec = result.outputResult[j];
        auto& internalVec = result.internalResult[j];

        assert(inputVec.size() == result.inputName.size());
        assert(outputVec.size() == result.outputName.size());
        assert(internalVec.size() == result.internalName.size());

        circuit(inputVec.data(), outputVec.data(), internalVec.data());
    }

    std::cout << "Done." << std::endl;

    dlclose(libhandle);

    return result;
}

void BlifBooleanNet::verifySimulator(int samples) {
    std::string library = Temp / "circuit.so";
    std::string source = Temp / "circuit.cpp";

    this->exportToCpp(source);

    std::string cmd = "g++ -shared -fPIC -Ofast -march=native " + source + " -o " + library;
    std::cout << "Executing: " << cmd << std::endl;
    system(cmd.c_str());

    std::cout << "Loading library at: " << library << std::endl;
    void* libhandle = dlopen(library.c_str(), RTLD_NOW | RTLD_LOCAL);

    assert(libhandle != nullptr);

    typedef void (*CircuitFun)(const int input[],
                               int output[],
                               int node[]);
    typedef std::vector<std::string> (*ConstVectorFun)();

    std::cout << "Accessing symbols... ";
    CircuitFun circuit = (CircuitFun)dlsym(libhandle, "circuit");
    ConstVectorFun inputNode = (ConstVectorFun)dlsym(libhandle, "inputNode");
    ConstVectorFun outputNode = (ConstVectorFun)dlsym(libhandle, "outputNode");
    ConstVectorFun internalNode = (ConstVectorFun)dlsym(libhandle, "internalNode");

    assert(circuit != nullptr);
    assert(inputNode != nullptr);
    assert(outputNode != nullptr);
    assert(internalNode != nullptr);

    std::cout << "Done." << std::endl;

    std::cout << "Begin simulation" << std::endl;

    InfiniteRandomPatternGenerator g(this->nInputs());

    for (int j = 0; j < samples; j++) {
        std::vector<int> p = g.generate();

        auto ret1 = this->evalAllOutputAt(p);
        std::vector<int> output;
        std::vector<int> nodes;
        nodes.resize(this->internalNodeSet().size());
        output.resize(this->nOutputs());

        circuit(p.data(), output.data(), nodes.data());

        assert(ret1 == output);
    }

    std::cout << "Done." << std::endl;

    dlclose(libhandle);
}

