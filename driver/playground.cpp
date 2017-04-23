//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"
#include "../abc_expansion/ex_abc.h"


int main() {
    auto neta = BlifBooleanNet(TempPath / "a.blif");
    auto netb = BlifBooleanNet(TempPath / "b.blif");

    auto r = neta.compareBySimulation(netb, 1000);

    std::cout << r.nSamples << " " << r.nErrors << std::endl;

    std::cout << neta.truthTable() << std::endl;
    std::cout << netb.truthTable() << std::endl;
    std::cout << (neta.truthTable() ^ netb.truthTable()) << std::endl;
    std::cout << (neta.truthTable() ^ netb.truthTable()).count() << std::endl;

    return 0;
}