//
// Created by tripack on 17-3-30.
//
#include <resource.h>
#include <iostream>
#include <dlfcn.h>

#include "interface.h"



CircuitProfile BlifBooleanNet::profileBySimulation(int samples) {
    std::string library = Temp / "circuit.so";
    std::string source = Temp / "circuit.cpp";

    this->exportToCpp(source);

    std::string cmd = "g++ -shared -fPIC -Ofast " + source + " -o " + library;
    std::cout << "Executing: " << cmd << std::endl;
    system(cmd.c_str());

    std::cout << "Loading library at: " << library << std::endl;
    void* libhandle = dlopen(library.c_str(), RTLD_NOW | RTLD_LOCAL);

    assert(libhandle != nullptr);

    typedef void (*CircuitFun)(const bool input[], bool output[], bool node[]);
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

    dlclose(libhandle);
}