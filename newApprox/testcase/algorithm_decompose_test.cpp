//
// Created by king on 17-4-19.
//

#include <gtest/gtest.h>
#include <vector>

#include "../src/bool_function.h"
#include "../src/kmap.h"
#include "../src/algorithm_decompose.h"
#include "../../common/ttable.h"
#include "../../driver/header.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;

TEST(ALGORITHM__Test, TEST_CASE_1) {

    StopWatch sw;
    Path temp = ProjectBase.sub("run");
    auto circuit = fBlif("testing2");
    sw.start();
    auto net = BlifBooleanNet(temp / circuit);
    sw.take("Load");
    auto mffcs = net.getFFC();
    sw.take("Mffc");
    filterMffcBySize(mffcs);
    sw.take("FilterSize");
    filterMffcContainOutput(mffcs, net.outputNodeList());
    sw.take("FilterOutput");

    net.prepareSimulator();

    sw.take("Compiling");

    auto simResult = net.profileBySimulation(10000);

    sw.take("Simulation");


    auto test =
            [](const FFC& ffc) -> bool {
                return ffc.inputNode.size() == 5;
            };

    auto ffc = findFirstFFC(mffcs, test);
    sw.take("FindFFC");

    std::cout << "Found MFFC of " << ffc->name << " over "
              << ffc->inputNode.size() << " inputs, "
              << ffc->nodeSet.size() << " nodes\n";
    net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
    auto mffc = BlifBooleanNet(TempPath / fBlif("mffc"));
    std::cout << "Inputs: " << mffc.inputNodeList() << endl;
    sw.take("FFC Load");
    TTable table = mffc.truthTable();


    //==========================
    cout << endl;
    cout << endl;

    BoolFunction fun(5, table, mffc.inputNodeList(), string("xx"));
    fun.display();

    AlgorithmDecompose decompose;
    AlgorithmDecompose::ResultType res =
            decompose.operate(fun, simResult, BFS_SEARCH);

    res.fun.display();
    cout << res.errorCount << endl;

};