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
#include "../newApprox/src/bool_function.h"
#include "../newApprox/src/algorithm_decompose.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

int main() {

    auto net0 = BlifBooleanNet(BenchmarkAigPath / fBlif("C880"));

    const auto& d = net0.depth2Output();

    for (auto elem : d) {
        std::cout << elem.first << " " << elem.second << endl;
    }

    return 0;
}