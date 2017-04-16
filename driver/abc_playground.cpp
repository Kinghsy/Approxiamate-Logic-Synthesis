//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <resource.h>
#include <random>
#include <ctime>
#include <iostream>

#include "../circuit/interface.h"
#include "../abc_expansion/ex_abc.h"


int main() {
    StopWatch s;

    auto& instance = ExAbc::getInstance();

    s.take();

    instance.loadBlif(BenchmarkAigPath / "C880.blif");

    s.take();

    instance.map();

    s.take();

    std::cout << instance.postMapArea() << std::endl;

    s.report();

    return 0;
}