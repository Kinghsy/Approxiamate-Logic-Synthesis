//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"


int main() {
    StopWatch s("My Watch");

    s.start();
    BlifBooleanNet net(BenchmarkAigPath / "C3540.blif");
    s.take("Load");
    auto i = net.inputNodeList();
    auto o = net.outputNodeList();
    auto all = net.totalNodeSet();
    s.take("Nodes");
//    std::cout << "intputs:" << std::endl;
//    for (const auto& e : i) std::cout << e << " ";
//
//    std::cout << std::endl;

//    std::cout << "outputs:" << std::endl;
//    for (const auto& e : o) std::cout << e << " ";
//
//    std::cout << std::endl;

//    std::cout << "all:" << std::endl;
//    for (const auto& e : all) std::cout << e << " ";

    std::cout << std::endl;

    std::cout << net.gateCount() << std::endl;

    s.take("Gate");
    net.profileBySimulation(1);
    s.take("Compile");
    auto profile = net.profileBySimulation(300000);

    s.take("Prof");
    std::cout << profile.nSamples << std::endl;
    std::cout << profile.inputResult.size() << std::endl;
    std::cout << profile.outputResult.size() << std::endl;
    std::cout << profile.internalResult.size() << std::endl;

    s.take("Count");

    s.report();

    return 0;
}