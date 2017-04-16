//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <algorithm>

#include "../circuit/interface.h"


using std::cout;
using std::endl;
using std::setw;


using std::string;
using std::map;
using std::vector;
using std::unordered_map;



int main(int argc, char* argv[]) {

    StopWatch sw;
    auto circuit = fBlif("C880");
    sw.start();
    auto net = BlifBooleanNet(BenchmarkAigPath / circuit);
    sw.take("Load");
    auto mffcs = net.getFFC();
    sw.take("Mffc");
    filterMffcBySize(mffcs);
    sw.take("FilterSize");
    filterMffcContainOutput(mffcs, net.outputNodeList());
    sw.take("FilterOutput");

    for (const auto& pair : mffcs) {
        cout << std::setw(10) << pair.first << " ";
        const auto& attr = pair.second;
        cout << attr.inputNode.size() << " " << attr.nodeSet.size() << " ";
        for (auto &&item : attr.nodeSet) {
            cout << item << " ";
            assert(!net.outputNodeSet().count(item) || item == pair.first);
        }
        cout << endl;
    }

    auto simResult = net.profileBySimulation(100);

    for (auto &&row : simResult.outputName) {
        cout << row << "\t";
    }
    cout << endl;

    auto name = simResult.outputName;
    name.resize(5);

    for (auto && e : name) cout << e << " "; cout << endl;


    for (auto &&row : simResult.outputResult) {
        for (auto &&col : row) {
            cout << col;
        }
        cout << "\n";
    }

    auto focus = simResult.focus(name);
    for (size_t i = 0; i < (1ul << 5); i ++) {
        std::cout << DBitset(5, i) << " " << focus.count(DBitset(5, i)) << endl;
    }


    sw.report();

    return 0;
}



