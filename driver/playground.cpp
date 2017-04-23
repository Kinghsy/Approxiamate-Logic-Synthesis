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

    /*
    auto net = BlifBooleanNet(TempPath / fBlif("mffc"));
    net.prepareSimulator();
    auto simRes = net.profileBySimulation(100);

    TTable tab = net.truthTable();
    BoolFunction fun(net.inputNodeList().size(), tab,
                net.inputNodeList(), net.outputNodeList()[0]);
    AlgorithmDecompose algo;
    AlgorithmDecompose::ResultType res =
            algo.operate(fun, simRes, FULL_SEARCH);
    res.deInfo.printBody(cout);*/



    auto& abc = ExAbc::getInstance();
    vector<string> vec;
    File alu4("apex2",".blif");
    vec.push_back(McncAigPath / alu4.toString());
    for (int i = 0; i < 2; ++i) {
        vec.push_back(TempPath / alu4[""][i].toString());
    }

    for (auto& item : vec ) {
        abc.reboot();
        abc.loadBlif(vec.back());
        vec.pop_back();
        abc.execute("sweep");
        abc.map();
        std::cout << abc.postMapArea() << std::endl;
    }


    return 0;
}