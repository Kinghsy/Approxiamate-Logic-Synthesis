//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <algorithm>

#include "../circuit/interface.h"
#include "header.h"


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

//    for (const auto& pair : mffcs) {
//        cout << std::setw(10) << pair.first << " ";
//        const auto& attr = pair.second;
//        cout << attr.inputNode.size() << " " << attr.nodeSet.size() << " ";
//        for (auto &&item : attr.nodeSet) {
//            cout << item << " ";
//            assert(!net.outputNodeSet().count(item) || item == pair.first);
//        }
//        cout << endl;
//    }

    net.prepareSimulator();

    sw.take("Compiling");

    auto simResult = net.profileBySimulation(10000);

    sw.take("Simulation");

    auto ffc = findFirstFFC(mffcs,
                            [](const FFC& ffc) -> bool {
                                return ffc.inputNode.size() == 6;
                            });
    assert(ffc);

    sw.take("FindFFC");

    net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
    auto mffc = BlifBooleanNet(TempPath / fBlif("mffc"));
    sw.take("FFC Load");
    TTable table = mffc.truthTable();
    sw.take("FFC TTable");
    auto focusedResult = simResult.focus(mffc.inputNodeList());
    sw.take("Focus");
    auto& preDecomp = PreDecomp::getInstance();
    sw.take("preDecomp");
    auto& match = preDecomp.getMatch(table, mffc.inputNodeList(), focusedResult);
    std::cout << table << endl;
    std::cout << match.function << endl;
    sw.take("Match");
    std::cout << "MatchError: " << countMatchError(table, match.function, focusedResult) << endl;
    sw.report();

    std::cout << focusedResult << endl;

    return 0;
}



