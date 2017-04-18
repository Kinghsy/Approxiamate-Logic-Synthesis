//
// Created by king on 17-4-14.
//

#include <gtest/gtest.h>
#include <vector>

#include "../src/kmap.h"
#include "../src/bool_function.h"
#include "../../driver/header.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;


TEST(TEST_NEW_KMAP, test_case1) {

    cout << endl;

    vector<string> inputs;
    inputs.push_back("x1");
    inputs.push_back("x2");
    inputs.push_back("x3");
    string output("cc");
    BoolFunction fun(3, TTable("01011101"), inputs, output);

    vector<string> part1;
    part1.push_back(string("x2"));
    vector<string> part2;
    part2.push_back(string("x1"));
    part2.push_back(string("x3"));

    Kmap map(fun, part1, part2);
    map.display();
    part1.push_back(string("x3"));
    part2.pop_back();
    Kmap map1(fun, part1, part2);
    map1.display();

    ASSERT_EQ(1, 1);
}

TEST(TEST_NEW_KMAP, test_case2) {

    cout << endl;

    vector<string> inputs;
    inputs.push_back("x1");
    inputs.push_back("x2");
    inputs.push_back("x3");
    string output("cc");
    BoolFunction fun1(3, TTable("01011101"), inputs, output);
    vector<string> part1;
    part1.push_back(string("x2"));
    vector<string> part2;
    part2.push_back(string("x1"));
    part2.push_back(string("x3"));

    Kmap map1(fun1, part1, part2);

    BoolFunction fun2(3, TTable("01010101"), inputs, output);

    Kmap map2(fun2, part1, part2);

    bool flag;
    cout << (flag = (map1 == map2)) << endl;

    size_t count;
    cout << (count = (map1 ^ map2)) << endl;

    ASSERT_EQ(false, flag);
    ASSERT_EQ(1, count);

}

TEST(TEST_NEW_KMAP, test_case3) {


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
    vector<string > name1;
    name1.push_back(string("x0"));
    name1.push_back(string("x1"));
    name1.push_back(string("x2"));
    vector<string > name2;
    name2.push_back(string("x3"));
    name2.push_back(string("x4"));
    Kmap map(fun, name1, name2);
    map.display();

    auto res = map.divide(simResult);
    res.leftFunc.display();
    res.rightFunc.display();
    cout << res.errorCount << endl;
    for (int i = 0; i < 4; ++i) {
        cout << res.method[i];
    }
    cout << endl;

}