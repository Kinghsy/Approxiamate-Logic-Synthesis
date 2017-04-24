//
// Created by tripack on 4/15/17.
//

#include <common.h>
#include <algorithm>

#include "../circuit/interface.h"
#include "../newApprox/src/bool_function.h"
#include "../newApprox/src/algorithm_decompose.h"
#include "../newApprox/src/decomp_small.h"
#include "header.h"

#define SIM_ROUND 40000
#define TOLLENT_RATE 0.10f

// mode
#define NEW_APPROX 1
#define PRE_DECOMP 2
#define MIXXED 3
#define PRE_DECOMP_BOUNS 0.005f

int searchMode = PRE_DECOMP;
int newApproxMethd = AlgorithmDecompose::BRANCH_AND_BOUND;

using std::cout;
using std::endl;
using std::setw;

using std::string;
using std::map;
using std::vector;
using std::unordered_map;
typedef std::shared_ptr<IAlgorithmDecompose> AlgPtr;

const AlgorithmDecompose::Mode searchStrategy
        = AlgorithmDecompose::BRANCH_AND_BOUND;

const std::string baseName  = "C880";
const auto blifCircuit      = fBlif(baseName);
const auto blifMffc         = fBlif(baseName)["_mffc_"];
const auto blifApproxMffc   = fBlif(baseName)["_mffc_approx_"];
const AlgPtr algo(new AlgorithmDecompose(searchStrategy));

int main(int argc, char* argv[]) {

    NodeName baseName = "C880";

    auto select =
            [](const FFC& ffc) -> bool {
                return ((ffc.inputNode.size() <= 6) &&
                        (ffc.inputNode.size() >= 3));
            };

    StopWatch sw;
    sw.start();

    auto initNet = BlifBooleanNet(BenchmarkAigPath / blifCircuit);
    sw.take("LoadInit");

    auto initSimResult = initNet.profileBySimulation(SIM_ROUND);
    sw.take("SimInit");

    //PreDecomp& dp = PreDecomp::getInstance();
    sw.take("LoadPreDecomp");

    auto unmodedCircuit = fBlif(baseName);
    auto filePath = BenchmarkAigPath;
    size_t nowError = 0;
    bool flag = true;
    int i = -1;

    //for (int i = 0; i < 15; ++i) {

    vector<NodeName> lastFocus;
    NodeName lastFfcNode;
    while (flag) {
        i++; flag = false;
        cout << endl;

        sw.take("Take Round");
        cout << " Round: " << i << endl;

        auto unmodedNet = BlifBooleanNet(filePath / unmodedCircuit);
        sw.take(" Load Round Circuit");

        unmodedNet.prepareSimulator();
        sw.take(" Compile Round Circuit");

        auto unmodedSimResult = unmodedNet.profileBySimulation(SIM_ROUND);
        sw.take(" Simu Round Circuit");

        auto res = unmodedNet.compareBySimulation(initNet, SIM_ROUND);
        nowError = res.nErrors;
        cout << "  now error: " << nowError << endl;
        cout << endl;

        auto ffcs = unmodedNet.getFFC();
        sw.take(" Take Round FFC");

        filterMffcBySize(ffcs);
        filterMffcContainOutput(ffcs, unmodedNet.outputNodeList());

        auto ffc = findFirstFFC(ffcs, select);
        sw.take(" Take FFC");

        vector<FFC> ffcSet;
        vector<BlifBuilder> blifSet;

        std::cout << "Last Focus: " << lastFocus << std::endl;
        auto lastFocusSim = unmodedSimResult.focus(lastFocus);
        std::cout << lastFocusSim.cdata << std::endl;
        cout << "Local Error at " << lastFfcNode << "\n";
        auto localErr = unmodedNet.localErrorSim(initNet, 10000, lastFfcNode);
        cout << localErr.nErrors << "\n\n";

        while (ffc) {
            /*std::cout << " Found MFFC of " << ffc->name << " over "
                      << ffc->inputNode.size() << " inputs, "
                      << ffc->nodeSet.size() << " nodes" << endl;*/
            unmodedNet.exportFfcToBlifFile(*ffc, TempPath / fBlif("mffc"));
            auto ffcCircuit = BlifBooleanNet(TempPath / fBlif("mffc"));
            //cout << " Inputs: " << ffcCircuit.inputNodeList() << endl;
            sw.take("  ffc load");

            TTable ffcTable = ffcCircuit.truthTable();
            sw.take("  ffc truth table");

            auto focusRes = unmodedSimResult.focus(ffcCircuit.inputNodeList());


            sw.take("   focus");

            BoolFunction fun(ffcCircuit.inputNodeList().size(), ffcTable,
                            ffcCircuit.inputNodeList(), ffcCircuit.outputNodeList()[0]);
            //cout << ffcTable << endl;
            //fun.display();
            AlgorithmDecompose::ResultType res
                    = (*algo)(fun, unmodedSimResult);
            sw.take("   algorithm decompose");

            /*cout << ffcTable << endl;
            cout << res.fun.getTTable() << endl;
            cout << "  MathchError: " << countMatchError(ffcTable, res.fun.getTTable(), focusRes) << "\t";
            cout << "  MissMatchError(By decomp): " << res.errorCount << endl;
            sw.take("   cout results");*/

            //cout << res.deInfo.nNode()  << " " << ffc->totalSet.size() - ffc->inputNode.size() << endl;
            if  (res.deInfo.nNode() == (ffc->totalSet.size() - ffc->inputNode.size())) {
                // cout << "  Modified Size: " << ffc->totalSet.size() - ffc->inputNode.size() << " --> " << res.deInfo.nNode() << endl;
                filterCurrentMffc(ffcs, *ffc);
                ffc = findFirstFFC(ffcs, select);
                continue;
            }
            if (nowError + countMatchError(ffcTable, res.fun.getTTable(), focusRes)
                    > SIM_ROUND * TOLLENT_RATE) {
                cout << "  Error exceed: " << nowError + countMatchError(ffcTable, res.fun.getTTable(), focusRes) << endl;
                filterCurrentMffc(ffcs, *ffc);
                ffc = findFirstFFC(ffcs, select);
                continue;
            }

            flag = true;
            nowError += countMatchError(ffcTable, res.fun.getTTable(), focusRes);

            cout << endl;
            std::cout << " Found MFFC of " << ffc->name << " over "
                      << ffc->inputNode.size() << " inputs, "
                      << ffc->nodeSet.size() << " nodes" << endl;
            cout << " Inputs: " << ffcCircuit.inputNodeList() << endl;

            cout << ffcTable << endl;
            cout << res.fun.getTTable() << endl;
            printMatchError(ffcTable, res.fun.getTTable(), focusRes);
            cout << focusRes.cdata << endl;
            cout << "  MathchError: " << countMatchError(ffcTable, res.fun.getTTable(), focusRes) << "\t";
            cout << "  MissMatchError(By decomp): " << res.errorCount << endl;
            cout << "  Modified Size: " << ffc->totalSet.size() - ffc->inputNode.size() << " --> " << res.deInfo.nNode() << endl;
            cout << "  current error: " << nowError << endl;
            cout << endl;



            res.deInfo.replaceInputOrder(ffcCircuit.inputNodeList());

            res.deInfo.exportBlif(TempPath / fBlif("mffc_approx"));
            res.deInfo.printBody(std::cout);
            auto approxNet = BlifBooleanNet(TempPath / fBlif("mffc_approx"));
            sw.take("   reload");

            try {

                throwFalse(approxNet.inputNodeList() == res.fun.inputPorts());
                throwFalse(approxNet.truthTable() == res.fun.getTTable());
            } catch (...) {
                std::cout << "\nDebug Info: " << std::endl;
                std::cout << res.fun.inputPorts() << std::endl;
                std::cout << approxNet.inputNodeList() << std::endl;
                std::cout << res.fun.getTTable() << std::endl;
                std::cout << approxNet.truthTable() << std::endl;
                std::cout << "\n BLif:" << std::endl;
                std::cout << res.deInfo << std::endl;
                assert(0);
            }

            ffcSet.push_back(*ffc);
            blifSet.push_back(res.deInfo);
            lastFocus = ffcCircuit.inputNodeList();
            lastFfcNode = ffc->name;

            //break;

            filterMffcByIntersection(ffcs, *ffc);
            ffc = findFirstFFC(ffcs, select);

        }

        char tempCh[10];
        sprintf(tempCh, "%d", i);
        unmodedNet.exportReplacedBlif(TempPath / fBlif(baseName + string("__") + string(tempCh)),
                        ffcSet, blifSet);



        std::cout << "Now Focus: " << lastFocus <<  std::endl;

        filePath = TempPath;
        unmodedCircuit = fBlif(baseName + string("__") + string(tempCh));

        //getchar();

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



