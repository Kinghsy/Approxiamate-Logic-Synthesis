//
// Created by tripack on 17-4-2.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"

int main() {

    BlifBooleanNet net(McncAigPath / "apex2.blif");
    auto i = net.inputNodeList();
    auto o = net.outputNodeList();
    auto all = net.totalNodeSet();
    std::cout << "intputs:" << std::endl;
    for (const auto& e : i) std::cout << e << " ";

    std::cout << std::endl;

    std::cout << "outputs:" << std::endl;
    for (const auto& e : o) std::cout << e << " ";

    std::cout << std::endl;

    std::cout << "all:" << std::endl;
    for (const auto& e : all) std::cout << e << " ";

    std::cout << std::endl;

    std::cout << net.gateCount() << std::endl;

    net.verifySimulator(40000);

    return 0;
}