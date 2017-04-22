//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <algorithm>

#include "../circuit/interface.h"
#include "../newApprox/src/bool_function.h"
#include "../newApprox/src/algorithm_decompose.h"
#include "header.h"

#define SIM_ROUND 10000


using std::cout;
using std::endl;
using std::setw;


using std::string;
using std::map;
using std::vector;
using std::unordered_map;



int main(int argc, char* argv[]) {

    NodeName baseName = "C880";

    auto select =
            [](const FFC& ffc) -> bool {
                return ((ffc.inputNode.size() <= 6) &&
                        (ffc.inputNode.size() >= 4));
            };
    AlgorithmDecompose algo;

    StopWatch sw;
    sw.start();
    auto initCircuit = fBlif(baseName);
    auto initNet = BlifBooleanNet(BenchmarkAigPath / initCircuit);
    sw.take("Load Init");

    initNet.prepareSimulator();
    sw.take("Compile Init");

    auto initSimResult = initNet.profileBySimulation(SIM_ROUND);
    sw.take("Simu Init");


    auto unmodedCircuit = fBlif(baseName);
    auto filePath = BenchmarkAigPath;

    for (int i = 0; i < 10; ++i) {

        sw.take("Take Round");
        cout << " Round: " << i << endl;

        auto unmodedNet = BlifBooleanNet(filePath / unmodedCircuit);
        sw.take(" Load Round Circuit");

        unmodedNet.prepareSimulator();
        sw.take(" Compile Round Circuit");

        auto unmodedSimResult = unmodedNet.profileBySimulation(SIM_ROUND);
        sw.take(" Simu Round Circuit");

        auto ffcs = unmodedNet.getFFC();
        sw.take(" Take Round FFC");

        filterMffcBySize(ffcs);
        filterMffcContainOutput(ffcs, unmodedNet.outputNodeList());

        auto ffc = findFirstFFC(ffcs, select);
        sw.take(" Take FFC");

        vector<FFC> ffcSet;
        vector<BlifBuilder> blifSet;

        while (ffc) {
            std::cout << " Found MFFC of " << ffc->name << " over "
                      << ffc->inputNode.size() << " inputs, "
                      << ffc->nodeSet.size() << " nodes" << endl;
            unmodedNet.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
            auto ffcCircuit = BlifBooleanNet(TempPath / fBlif("mffc"));
            cout << " Inputs: " << ffcCircuit.inputNodeList() << endl;
            sw.take("  ffc load");

            TTable ffcTable = ffcCircuit.truthTable();
            sw.take("  ffc truth table");

            auto focusRes = unmodedSimResult.focus(ffcCircuit.inputNodeList());
            sw.take("   focus");

            BoolFunction fun(ffcCircuit.inputNodeList().size(), ffcTable,
                            ffcCircuit.inputNodeList(), ffcCircuit.outputNodeList()[0]);
            AlgorithmDecompose::ResultType res =
                algo.operate(fun, unmodedSimResult, FULL_SEARCH);
            sw.take("   algorithm decompose");

            cout << ffcTable << endl;
            cout << res.fun.getTTable() << endl;
            cout << "  MathchError: " << countMatchError(ffcTable, res.fun.getTTable(), focusRes) << "\t";
            cout << "  MissMatchError(By decomp): " << res.errorCount << endl;
            sw.take("   cout results");

            res.deInfo.printBody(cout);
            sw.take("   build blif");

            res.deInfo.exportBlif(TempPath / fBlif("mffc_approx"));
            auto approxNet = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
            sw.take("   reload");

            ffcSet.push_back(*ffc);
            blifSet.push_back(res.deInfo);

            break;
            // FIXME
        }

        char tempCh[10];
        sprintf(tempCh, "%d", i);
        unmodedNet.exportReplacedBlif(TempPath / fBlif(baseName + string("__") + string(tempCh)),
                        ffcSet, blifSet);

        filePath = TempPath;
        unmodedCircuit = fBlif(baseName + string("__") + string(tempCh));

        getchar();

    }

    //auto mffcs = net.getFFC();
    //sw.take("Mffc");

    //filterMffcBySize(mffcs);
    //sw.take("FilterSize");

    //filterMffcContainOutput(mffcs, net.outputNodeList());
    //sw.take("FilterOutput");

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

    //net.prepareSimulator();
    //sw.take("Compiling");

    //auto simResult = net.profileBySimulation(SIM_ROUND);
    //sw.take("Simulation");


    /*auto test =
            [](const FFC& ffc) -> bool {
                return ((ffc.inputNode.size() <= 6) &&
                        (ffc.inputNode.size() >= 4));
            };*/

    /*auto ffc = findFirstFFC(mffcs, test);
    sw.take("FindFFC");*/

    /*while (ffc) {
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
        auto &match = preDecomp.getMatch(table, mffc.inputNodeList(), focusedResult, 0.005f);
        std::cout << table << endl;
        std::cout << match.function << endl;
        sw.take("Match");
        std::cout << "MatchError: " << countMatchError(table, match.function, focusedResult) << "\t";
        std::cout << "discardInput: " << match.discardMask.count() << endl;

        auto builderAlg = BuildCircuitFromMatch(ffc->name);
        auto blifBuilder = builderAlg(match, mffc.inputNodeList());

        std::cout << "BuilderSize: " << blifBuilder.nNode() << "\n";

        if (blifBuilder.nNode() == ffc->nodeSet.size()) {
            filterCurrentMffc(mffcs, *ffc);
        } else {
            //std::cout << blifBuilder;
            sw.take("BuildBlif");

            blifBuilder.exportBlif(TempPath / fBlif("mffc_approx"));
            auto netAprrox = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
            sw.take("ReloadBlif");

            filterMffcByIntersection(mffcs, *ffc);
        }
        sw.take("Filter");
        ffc = findFirstFFC(mffcs, test);
        sw.take("FindFFC");
    }
*/

//    AlgorithmDecompose algo;
//    while (ffc) {
//        if (ffc->inputNode.size() != ffc->nodeSet.size() + 1) {
//            std::cout << "Found MFFC of " << ffc->name << " over "
//                      << ffc->inputNode.size() << " inputs, "
//                      << ffc->nodeSet.size() << " nodes\n";
//            net.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
//            auto mffc = BlifBooleanNet(TempPath / fBlif("mffc"));
//            std::cout << "Inputs: " << mffc.inputNodeList() << endl;
//            sw.take("FFC Load");
//            TTable table = mffc.truthTable();
//            sw.take("FFC TTable");
//
//            /*auto focusedResult = simResult.focus(mffc.inputNodeList());
//            for (int i = 0; i < (1 << mffc.inputNodeList().size()); ++i) {
//                DBitset k(mffc.inputNodeList().size(), i);
//                cout << focusedResult.count(k) << endl;
//            }*/
//            sw.take("Focus");
//
//            BoolFunction fun(mffc.inputNodeList().size(), table,
//                             mffc.inputNodeList(), mffc.outputNodeList()[0]);
//            AlgorithmDecompose::ResultType res =
//                    algo.operate(fun, simResult, BRANCH_AND_BOUND);
//
//            std::cout << table << endl;
//            std::cout << res.fun.getTTable() << endl;
//            sw.take("Match");
//            std::cout << "MatchError: " << countMatchError(table, res.fun.getTTable(), focusedResult) << "\t";
//            std::cout << "MissMatchError(By Decomp): " << res.errorCount << endl;
//
//            res.deInfo.printBody(cout);
//            sw.take("BuildBlif");
//
//            res.deInfo.exportBlif(TempPath / fBlif("mffc_approx"));
//            auto netAprrox = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
//            sw.take("ReloadBlif");
//
//            //system("pause()");
//            //sleep(500);
//            //getchar();
//        }
//
//        filterMffcByIntersection(mffcs, *ffc);
//        sw.take("Filter");
//        ffc = findFirstFFC(mffcs, test);
//        sw.take("FindFFC");
//    }

    //sw.report();
    return 0;
}



