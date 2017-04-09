//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"
#include "../pre_decomp/pre_decomp.h"


int main() {
    StopWatch s("My Watch");

    s.start();

    PreDecomp& pd = PreDecomp::getInstance();

    s.take("PD");

    std::cout << pd.nLut3() << std::endl;
    std::cout << pd.nLut4() << std::endl;
    std::cout << pd.nLut5() << std::endl;
    std::cout << pd.nLut6() << std::endl;

    s.take();

    auto e = pd.getMatch("01011011001101110", 6);

    s.take();

    s.report();

    return 0;
}