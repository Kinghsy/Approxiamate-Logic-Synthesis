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
#include "../newApprox/src/decomp_small.h"
#include "header.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

int main() {

    auto net = BlifBooleanNet(TempPath / fBlif("testing"));
    net.prepareSimulator();
    auto simRes = net.profileBySimulation(100);

    TTable tab = net.truthTable();
    BoolFunction fun(net.inputNodeList().size(), tab,
                net.inputNodeList(), net.outputNodeList()[0]);
    AlgorithmDecompose algo;
    AlgorithmDecompose::ResultType res =
            algo.operate(fun, simRes, FULL_SEARCH);
    res.deInfo.printBody(cout);


/*
    auto& abc = ExAbc::getInstance();
    vector<string> vec;

    File c880("C880", ".blif");
    vec.push_back(BenchmarkAigPath / c880.toString());

    for (int i = 0; i < 3; ++i) {
        vec.push_back(TempPath / c880[""][i].toString());
    }

    for (auto& item : vec ) {
        abc.reboot();
        abc.loadBlif(vec.back());
        vec.pop_back();
        abc.execute("sweep");
        abc.map();
        std::cout << abc.postMapArea() << std::endl;
    }
    */
    return 0;
}