//
// Created by tripack on 17-3-29.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>
#include <bitset>

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

    std::string funStr = "01011011001101110";

    auto e = pd.getMatch(funStr, 6);

    s.take();

    std::cout << std::bitset<64>(funStr) << std::endl;
    std::cout << e.function << " " << e.nInputs << std::endl;
    std::cout << e.leftMask << " " << e.rightMask << std::endl;
    std::cout << e.combine << std::endl;
    std::cout << e.left << " " << e.right << std::endl;

    s.report();

    return 0;
}