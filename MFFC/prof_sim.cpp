//
// Created by tripack on 17-3-30.
//
#include <resource.h>
#include <iostream>
#include <dlfcn.h>

#include "interface.h"
#include "pattern_gen.h"


CircuitProfile BlifBooleanNet::profileBySimulation(int samples) {
    std::string library = Temp / "circuit.so";
    std::string source = Temp / "circuit.cpp";

    this->exportToCpp(source);

    std::string cmd = "g++ -shared -fPIC -Ofast -march=native " + source + " -o " + library;
    std::cout << "Executing: " << cmd << std::endl;
    system(cmd.c_str());

    std::cout << "Loading library at: " << library << std::endl;
    void* libhandle = dlopen(library.c_str(), RTLD_NOW | RTLD_LOCAL);

    assert(libhandle != nullptr);

    typedef void (*CircuitFun)(const char input[],
                               char output[],
                               char node[]);
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

    for (int j = 0; j < 2000000; j++) {
        std::vector<int> p = g.generate();
        std::vector<char> pat(p.begin(), p.end());

        auto ret1 = this->evalAllOutputAt(p);
        std::vector<char> output;
        std::vector<char> nodes;
        nodes.resize(this->internalNodeSet().size());
        output.resize(this->nOutputs());

        circuit(pat.data(), output.data(), nodes.data());

        for (auto& e : output) e = e & 0x1;

        std::vector<int> intOut(output.begin(), output.end());

        assert(ret1 == intOut);
    }

    std::cout << "Done." << std::endl;

    dlclose(libhandle);
}