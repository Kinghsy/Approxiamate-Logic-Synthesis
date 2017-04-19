//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <algorithm>

#include "../circuit/interface.h"
#include "header.h"
#include "../newApprox/src/algorithm_decompose.h"
#include "../newApprox/src/bool_function.h"


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


    auto test =
            [](const FFC& ffc) -> bool {
                return ffc.inputNode.size() == 5;
            };

    auto ffc = findFirstFFC(mffcs, test);
    sw.take("FindFFC");

    /*
    while (ffc) {
        std::cout << "Found MFFC of " << ffc->name << " over "
                  << ffc->inputNode.size() << " inputs, "
                  << ffc->nodeSet.size() << " nodes\n";
        net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
        auto mffc = BlifBooleanNet(TempPath / fBlif("mffc"));
        std::cout << "Inputs: " << mffc.inputNodeList() << endl;
        sw.take("FFC Load");
        TTable table = mffc.truthTable();
        sw.take("FFC TTable");
        auto focusedResult = simResult.focus(mffc.inputNodeList());
        sw.take("Focus");
        auto &preDecomp = PreDecomp::getInstance();
        sw.take("preDecomp");
        auto &match = preDecomp.getMatch(table, mffc.inputNodeList(), focusedResult);
        std::cout << table << endl;
        std::cout << match.function << endl;
        sw.take("Match");
        std::cout << "MatchError: " << countMatchError(table, match.function, focusedResult) << "\t";
        std::cout << "discardInput: " << match.discardMask.count() << endl;

        auto builderAlg = BuildCircuitFromMatch(ffc->name);
        auto blifBuilder = builderAlg(match, mffc.inputNodeList());
        std::cout << blifBuilder;
        sw.take("BuildBlif");

        blifBuilder.exportBlif(TempPath / fBlif("mffc_approx"));
        auto netAprrox = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
        sw.take("ReloadBlif");

        assert(mffc.truthTable() == match.function);

        filterMffcByIntersection(mffcs, *ffc);
        sw.take("Filter");
        ffc = findFirstFFC(mffcs, test);
        sw.take("FindFFC");
    }*/


    AlgorithmDecompose algo;
    while (ffc) {
        std::cout << "Found MFFC of " << ffc->name << " over "
                  << ffc->inputNode.size() << " inputs, "
                  << ffc->nodeSet.size() << " nodes\n";
        net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
        auto mffc = BlifBooleanNet(TempPath / fBlif("mffc"));
        std::cout << "Inputs: " << mffc.inputNodeList() << endl;
        sw.take("FFC Load");
        TTable table = mffc.truthTable();
        sw.take("FFC TTable");

        auto focusedResult = simResult.focus(mffc.inputNodeList());
        sw.take("Focus");

        BoolFunction fun(mffc.inputNodeList().size(), table,
                mffc.inputNodeList(), mffc.outputNodeList()[0]);
        AlgorithmDecompose::ResultType res =
                algo.operate(fun, simResult, BRANCH_AND_BOUND);

        std::cout << table << endl;
        std::cout << res.fun.getTTable() << endl;
        sw.take("Match");
        std::cout << "MatchError: " << countMatchError(table, res.fun.getTTable(), focusedResult) << "\t";
        std::cout << "MissMatchError(By Decomp): " <<  res.errorCount << endl;

        res.deInfo.printBody(cout);
        sw.take("BuildBlif");

        res.deInfo.exportBlif(TempPath / fBlif("mffc_approx"));
        auto netAprrox = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
        sw.take("ReloadBlif");

        filterMffcByIntersection(mffcs, *ffc);
        sw.take("Filter");
        ffc = findFirstFFC(mffcs, test);
        sw.take("FindFFC");
    }

    sw.report();
    return 0;
}



