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

    auto net0 = BlifBooleanNet(BenchmarkAigPath / fBlif("C880"));

    //auto net1 = BlifBooleanNet(TempPath / fBlif("apex2__3"));
    auto net2 = BlifBooleanNet(TempPath / fBlif("C880__2"));

    //net1.verifySimulator(10000);
    net2.verifySimulator(10000);

//    auto mffc = BlifBooleanNet(TempPath / fBlif("n1004"));
//    auto appx = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
//
//    assert(mffc.inputNodeList() == appx.inputNodeList());

//    std::cout << mffc.truthTable() << endl;
//    std::cout << appx.truthTable() << endl;

//    auto mffcset = net1.getFFC();
//    for (auto& elem : mffcset)
//        if (elem.first == "n1004")
//            net1.exportFfcToBlifFile(elem.second, TempPath / "n1004.blif");

//    auto sim1 = net1.profileBySimulation(40000);
//    auto fsim1 = sim1.focus({"n441", "n1001", "n87", "n673"});
//    std::cout << fsim1.cdata << std::endl;
//
//    auto sim2 = net2.profileBySimulation(40000);
//    auto fsim2 = sim2.focus({"n441", "n1001", "n87", "n673"});
//    std::cout << fsim2.cdata << std::endl;
//
//    auto rn = net2.localErrorSim(net1, 40000, "n1004");
//    std::cout << "node diff " << rn.nErrors << std::endl;

    auto& n2 = net2;
    auto& n1 = net0;
    auto r = n2.compareBySimulation(n1, 40000);
    std::cout << r.nErrors << std::endl;
     r = n2.compareBySimulation(n1, 40000);
    std::cout << r.nErrors << std::endl;
     r = n2.compareBySimulation(n1, 40000);
    std::cout << r.nErrors << std::endl;
     r = n2.compareBySimulation(n1, 40000);
    std::cout << r.nErrors << std::endl;
     r = n2.compareBySimulation(n1, 40000);
    std::cout << r.nErrors << std::endl;
//
//    auto net1 = BlifBooleanNet(TempPath / fBlif("a"));
//    std::cout << net1.truthTable() << std::endl;

//    auto& abc = ExAbc::getInstance();
//    vector<string> vec;
//    File alu4("apex2",".blif");
//    vec.push_back(McncAigPath / alu4.toString());
//    for (int i = 0; i < 2; ++i) {
//        vec.push_back(TempPath / alu4[""][i].toString());
//    }
//
//    for (auto& item : vec ) {
//        abc.reboot();
//        abc.loadBlif(vec.back());
//        vec.pop_back();
//        abc.execute("sweep");
//        abc.map();
//        std::cout << abc.postMapArea() << std::endl;
//    }

    auto& abc = ExAbc::getInstance();
    abc.loadBlif(BenchmarkAigPath / fBlif("C880"));
    abc.resyn2();
    abc.map();
    std::cout << abc.postMapArea() << std::endl;

    abc.reboot();
    abc.loadBlif(TempPath / fBlif("C880__2"));
    abc.resyn2();
    abc.map();
    std::cout << abc.postMapArea() << std::endl;

    return 0;
}