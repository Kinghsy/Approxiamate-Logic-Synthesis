//
// Created by tripack on 17-3-30.
//
#include <common.h>
#include <iostream>
#include <dlfcn.h>

#include "interface.h"
#include "pattern_gen.h"

#define CONTEXT_PTR(x) (static_cast<SimulationContext*>(x))

typedef void (*CircuitFun)(const int input[],
                           int output[],
                           int node[]);
typedef std::vector<std::string> (*ConstVectorFun)();

struct SimulationContext {
    void* libHandle;
    ConstVectorFun inputNodeName;
    ConstVectorFun outputNodeName;
    ConstVectorFun internalNodeName;
    CircuitFun circuit;
};




void *BlifBooleanNet::getSimulationContext() const{
    if (simulationContext.isValid())
        return simulationContext.get();

    //std::cout << "Preparing Simulation Context." << std::endl;

    int i = Random::randomInt();
    if (i < 0) i = -i;
    std::string library = TempPath / fSo("sim")[i];
    std::string source = TempPath / fCpp("sim")[i];

    this->exportToCpp(source);

    std::string cmd = "g++ -std=c++14 -shared -fPIC -Ofast -march=native " + source + " -o " + library;
    std::cout << "Executing: " << cmd << std::endl;
    int rec = system(cmd.c_str());
    assert(rec == 0);

    //std::cout << "Loading library at: " << library << std::endl;
    void* libhandle = dlopen(library.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (libhandle == nullptr) {
        std::cerr << dlerror() << std::endl;
        assert(libhandle != nullptr);
    }

    //std::cout << "Accessing symbols... ";
    CircuitFun circuit = (CircuitFun)dlsym(libhandle, "circuit");
    ConstVectorFun inputNode = (ConstVectorFun)dlsym(libhandle, "inputNode");
    ConstVectorFun outputNode = (ConstVectorFun)dlsym(libhandle, "outputNode");
    ConstVectorFun internalNode = (ConstVectorFun)dlsym(libhandle, "internalNode");

    assert(circuit != nullptr);
    assert(inputNode != nullptr);
    assert(outputNode != nullptr);
    assert(internalNode != nullptr);

    SimulationContext* ctx = new SimulationContext;

    ctx->libHandle = libhandle;
    ctx->inputNodeName = inputNode;
    ctx->outputNodeName = outputNode;
    ctx->internalNodeName = internalNode;
    ctx->circuit = circuit;

    simulationContext.setData((void*)ctx);

    //std::cout << "Done." << std::endl;

    return simulationContext.get();
}

void BlifBooleanNet::releaseSimulationContext() {
    if (!simulationContext.isValid()) return;
    SimulationContext* ctx = (SimulationContext*) simulationContext.get();

    dlclose(ctx->libHandle);
    delete ctx;
}

SimulationResult
BlifBooleanNet::profileBySimulation(int samples) {

    auto context = CONTEXT_PTR(getSimulationContext());

    //std::cout << "Begin simulation" << std::endl;

    InfiniteRandomPatternGenerator g(this->nInputs());

    SimulationResult result(*this, samples);

    result.inputName = context->inputNodeName();
    result.outputName = context->outputNodeName();
    result.internalName = context->internalNodeName();

    for (int j = 0; j < samples; j++) {
        auto& inputVec = result.inputResult[j];
        inputVec = g.generate();

        auto& outputVec = result.outputResult[j];
        auto& internalVec = result.internalResult[j];

        assert(inputVec.size() == result.inputName.size());
        assert(outputVec.size() == result.outputName.size());
        assert(internalVec.size() == result.internalName.size());

        context->circuit(inputVec.data(), outputVec.data(), internalVec.data());
    }

    //std::cout << "Done." << std::endl;

    return result;
}

void BlifBooleanNet::verifySimulator(int samples) {

    auto context = CONTEXT_PTR(getSimulationContext());

    std::cout << "Begin simulation" << std::endl;

    SimulationResult result(*this, samples);

    result.inputName = context->inputNodeName();
    result.outputName = context->outputNodeName();
    result.internalName = context->internalNodeName();

    std::cout << "Begin simulation" << std::endl;

    InfiniteRandomPatternGenerator g(this->nInputs());

    for (int j = 0; j < samples; j++) {
        std::vector<int> p = g.generate();

        auto ret1 = this->evalAllOutputAt(p);
        std::vector<int> output;
        std::vector<int> nodes;
        nodes.resize(this->internalNodeSet().size());
        output.resize(this->nOutputs());

        context->circuit(p.data(), output.data(), nodes.data());

        assert(ret1 == output);
    }

    std::cout << "Done." << std::endl;
}


CompareResult
BlifBooleanNet::compareBySimulation(const BlifBooleanNet &net2,
                                    size_t nSamples) {

    auto context0 = CONTEXT_PTR(this->getSimulationContext());
    auto context1 = CONTEXT_PTR(net2.getSimulationContext());

    /*=====================================================
     * ================= SIMULATION  ======================
     * ==================================================== */

    //std::cout << "Performing Pre-sim Checks..." << std::endl;

    // Validity checks

    auto inputName0 = context0->inputNodeName();
    auto inputName1 = context1->inputNodeName();
    assert (inputName0 == inputName1);

    auto outputName0 = context0->outputNodeName();
    auto outputName1 = context1->outputNodeName();
    assert (outputName0 == outputName1);

    //std::cout << "Running Simulation..." << std::endl;

    InfiniteRandomPatternGenerator g(this->nInputs());

    size_t error = 0;

    for (int j = 0; j < nSamples; j++) {
        std::vector<int> p = g.generate();

        std::vector<int> output0;
        std::vector<int> output1;
        std::vector<int> nodes0;
        std::vector<int> nodes1;
        nodes0.resize(this->internalNodeSet().size());
        nodes1.resize(net2.internalNodeSet().size());
        output0.resize(this->nOutputs());
        output1.resize(net2.nOutputs());

        context0->circuit(p.data(), output0.data(), nodes0.data());
        context1->circuit(p.data(), output1.data(), nodes1.data());

        if (output0 != output1) error++;
    }

    CompareResult r;

    r.nSamples = nSamples;
    r.nErrors = error;

    //std::cout << "Done!" << std::endl;

    return r;
}

