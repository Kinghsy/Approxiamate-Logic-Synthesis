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
    auto& instance = ExAbc::getInstance();

    instance.loadBlif(BenchmarkAigPath / "C880.blif");

    return 0;
}