//
// Created by tripack on 16-11-18.
//

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include <common.h>
#include "header.h"
#include "../abc_expansion/ex_abc.h"
#include "../circuit/interface.h"
#include "../newApprox/src/algorithm_decompose.h"
#include "../newApprox/src/decomp_small.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::set;
using std::unique_ptr;

typedef unique_ptr<IAlgorithmDecompose> UAlgPtr;
typedef std::shared_ptr<IAlgorithmDecompose> AlgPtr;

struct ReplaceInfo {
    std::shared_ptr<IAlgorithmDecompose::ResultType> matchPtr = nullptr;
    std::shared_ptr<BlifBooleanNet::FFC> ffcPtr = nullptr;
};

auto BenchmarkCircuitPath = DataPath.sub("circuit").sub("benchmark");

float   MAXIMUM_ERROR_RATE = 0.05f;
float   TIGHT_ERROR_RATE = 0.02f;
float   RELAXED_ERROR_RATE = 0.10f;
float   PREDECOMP_BONUS = 0.005f;
float   DEPTH_BONUS = 0.9f;
int     SWEEP_TRIVIAL_ROUND = 5;
size_t  SIMULATION_SAMPLE = 40000;

float powi(float f, size_t e) {
    if (e == 0) return 1;
    else return f * powi(f, e - 1);
}

int main() {

    std::string circuitName = "C880";

    File blifInit   = fBlif(circuitName);
    File blifApprox = fBlif(circuitName)["approx"];
    File blifFFC    = fBlif(circuitName)["mffc"];
    File blifAppFFC = fBlif(circuitName)["mffc_aprx"];

    ExAbc& abc      = ExAbc::getInstance();
   // PreDecomp& pd   = PreDecomp::getInstance();

    size_t currentErr = 0;

    /* ====================================================
     * ============ Statistics ============================
     * ==================================================== */

    auto netInit = BlifBooleanNet(BenchmarkCircuitPath / blifInit);
    auto mffcInit = netInit.getFFC();
    filterMffcBySize(mffcInit);
    std::map<size_t, size_t> mffcSizeCount;
    for (const auto& e : mffcInit)
        mffcSizeCount[e.second.inputNode.size()]++;
    cout << "Circuit File: " << netInit.name() << endl;
    cout << "I/O: " << netInit.nInputs() << "/" << netInit.nOutputs();
    cout << "\tNode Count: " << netInit.gateCount() << std::endl;
    abc.loadBlif(BenchmarkCircuitPath / blifInit);
    abc.resyn2(); abc.map();
    cout << "Mapped Area: " << abc.postMapArea() << std::endl;

//    std::cout << "Size of different MFFCs" << std::endl;
//    for (const auto& p : mffcSizeCount)
//        std::cout << p.first << "\t\t" << p.second << std::endl;



    /* =====================================================
     * ==============  Stage 1 Sweep Trivials ==============
     * ===================================================== */
    { // Serves as a scope
        BlifBooleanNet *net = &netInit;
        for (int i = 0; i < SWEEP_TRIVIAL_ROUND; i++) {
            std::cout << "Sweep Stage " << i << endl;
            if (i != 0) {
                abc.loadBlif(TempPath / blifApprox[i - 1]);
                abc.resyn2();
                abc.writeBlif(TempPath / blifApprox[i - 1]["r"]);
                abc.map();
                std::cout << "Area: " << abc.postMapArea() << endl;
                net = new BlifBooleanNet(TempPath / blifApprox[i - 1]["r"]);
                auto compare = netInit.compareBySimulation(*net, SIMULATION_SAMPLE);
                if (compare.nErrors > TIGHT_ERROR_RATE * SIMULATION_SAMPLE) {
                    std::cout << "Error Rate: " << compare.nErrors << std::endl;
                    currentErr = compare.nErrors;
                    break;
                }
                currentErr = compare.nErrors;
                std::cout << "Error Rate: " << compare.nErrors << std::endl;
            }
            auto blifSubMffc = blifFFC[i];
            auto mffcSet = net->getFFC();
            filterMffcBySize(mffcSet);
            auto select =
                    [](const FFC& ffc) -> bool {
                        return ((ffc.inputNode.size() <= 6) &&
                                (ffc.inputNode.size() >= 3));
                    };
            auto simData = net->profileBySimulation(SIMULATION_SAMPLE);

            std::vector<ReplaceInfo> replaceCandidate;

            for (size_t counter = 0;; counter++) {
                auto FfcObject = findFirstFFC(mffcSet, select);
                if (FfcObject == nullptr) break;
                net->exportFfcToBlifFile(*FfcObject, TempPath / blifSubMffc[counter]);
                auto netMffc = BlifBooleanNet(TempPath / blifSubMffc[counter]);
                BoolFunction mffcOrigFun(
                        netMffc.nInputs(),
                        netMffc.truthTable(),
                        netMffc.inputNodeList(),
                        FfcObject->name
                );
                auto algorithm = UAlgPtr(new AlgorithmDecomposeSmall(PREDECOMP_BONUS * 2));
                auto match = (*algorithm)(mffcOrigFun, simData);
                const auto& depths = net->depth2Output();
                if (!depths.count(FfcObject->name)) {
                    filterCurrentMffc(mffcSet, *FfcObject);
                    FfcObject = findFirstFFC(mffcSet, select);
                    continue;
                }
                size_t depth2output = depths.at(FfcObject->name);
                float ratio = powi(DEPTH_BONUS, depth2output);
                match.errorCount *= ratio;
                if (match.errorCount > RELAXED_ERROR_RATE * SIMULATION_SAMPLE) {
                    filterCurrentMffc(mffcSet, *FfcObject);
                } else if (match.deInfo.nNode() == FfcObject->nodeSet.size()) {
                    filterCurrentMffc(mffcSet, *FfcObject);
                } else {
                    std::cout << "Seleceted MFFC at " << FfcObject->name << endl;
                    std::cout << netMffc.truthTable() << std::endl;
                    std::cout << match.fun.getTTable() << std::endl;
                    std::cout << "nInput: " << FfcObject->inputNode.size() << "; ";
                    std::cout << "Depths: " << depth2output <<"; ";
                    std::cout << "MError: " << match.errorCount <<"; \n";
                    ReplaceInfo info;
                    info.ffcPtr = std::make_shared<BlifBooleanNet::FFC>(*FfcObject);
                    info.matchPtr = std::make_shared<IAlgorithmDecompose::ResultType>(match);
                    replaceCandidate.push_back(info);
                    filterMffcByIntersection(mffcSet, *FfcObject);
                }
                FfcObject = findFirstFFC(mffcSet, select);
            }

            // FFC Selection
            auto compare =
                    [](const ReplaceInfo& i1, const ReplaceInfo& i2) -> bool {
                        return i1.matchPtr->errorCount < i2.matchPtr->errorCount;
            };
            std::sort(replaceCandidate.begin(), replaceCandidate.end(), compare);
            std::vector<BlifBooleanNet::FFC> ffc2replace;
            std::vector<BlifBuilder> builder;
            size_t accumulatedErr = currentErr;
            assert(accumulatedErr < TIGHT_ERROR_RATE * SIMULATION_SAMPLE);
            for (const auto& candidate : replaceCandidate) {
                auto candidateErr = candidate.matchPtr->errorCount;
                if (accumulatedErr > TIGHT_ERROR_RATE * SIMULATION_SAMPLE) break;
                std::cout << "Accepted MFFC @ " << candidate.ffcPtr->name << "; ";
                std::cout << "Local Error: " << candidate.matchPtr->errorCount << endl;
                ffc2replace.push_back(*candidate.ffcPtr);
                builder.push_back(candidate.matchPtr->deInfo);
                accumulatedErr += candidateErr;
            }
            net->exportReplacedBlif(TempPath / blifApprox[i],
                                    ffc2replace, builder);
            if (i != 0) delete net;
        }
    }

}

//=======================================================

//int main(int argc, char* agrv[]) {
//
//    string base="c880";
//    string exten="blif";
//    string vizExten="viz";
//    string pngExten="png";
//    string initFileName = base+"."+exten;
//    BlifBooleanNet rawData(initFileName);
//    cout << rawData.name() << " " << rawData.gateCount() << " " << rawData.nInputs() << " " << rawData.nOutputs() << endl;
//    string withFileName = base +"_with." +exten;
//    FilenameGenerator fnGen(base+"__","."+exten);
//    FilenameGenerator gener1(base+"_","_unmodied."+vizExten);
//    FilenameGenerator gener2(base+"_","_modied."+vizExten);
//    FilenameGenerator gener1_(base+"_","_unmodied."+pngExten);
//    FilenameGenerator gener2_(base+"_","_modied."+pngExten);
//    rawData.exportGraphViz(base + "."+vizExten);
//    string command1 = "dot -Tpng -o "+base+"."+pngExten+" < "+base+"."+vizExten;
//    //system(command1.c_str());
//    string tmp="dot -Tpng -o "+base+"."+pngExten+" < "+base+"."+vizExten;
//    //system(tmp.c_str());
//
//    std::vector<int > hisGateNum;
//    hisGateNum.push_back(rawData.gateCount());
//
//    while (fnGen.genState() < 15) {
//
//        cout << "round " << fnGen.genState() << endl;
//        string outFileName = fnGen.generate();
//        BlifBooleanNet initNet(initFileName);
//        //BlifBooleanNet mffc = initNet.getMFFC(4, 9);
//        FfcSelectPolicy policy;
//        while (true) {
//            BlifBooleanNet::FFC chosenFfc = selectFFC(initNet.getFFC(), policy);
//            string replaceFileName = "ffc.blif";
//            initNet.exportFfcToBlifFile(chosenFfc, replaceFileName);
//            BlifBooleanNet ffc(replaceFileName);
//            cout << "  FFC inputs: " << ffc.nInputs() << endl;
//            string name1 = gener1.generate();
//            initNet.exportGraphVizwithHighlight(name1, ffc.totalNodeSet(), "red");
//            string tmp1 = "dot -Tpng -o " + gener1_.generate() + " < " + name1;
//            //system(tmp1.c_str());
//
//            TruthTable initFfcTruthTable = ffc.truthTable();
//            clock_t timeStart = clock();
//            TruthTable finalFfcTruthTable = writeApproxBlifFileByTruthTable(initFfcTruthTable, withFileName, FULL_SEARCH | XOR_IGNORE);
//            clock_t timeEnd = clock();
//            cout << "    time:" << ((double) (timeEnd - timeStart)) / CLOCKS_PER_SEC << " s\n";
//            replacePartialBlif(initFileName,
//                               replaceFileName,
//                               withFileName,
//                               outFileName);
//            BlifBooleanNet withNet(withFileName);
//            BlifBooleanNet modifiedNet(outFileName);
//            BlifCompareResult r = sampleCompareBlifs(rawData, modifiedNet, (1 << 20));
//            string name2 = gener2.generate();
//            modifiedNet.exportGraphVizwithHighlight(name2, withNet.totalNodeSet(), "cyan");
//            string tmp2 = "dot -Tpng -o " + gener2_.generate() + " < " + name2;
//            //system(tmp2.c_str());
//            cout << "    " << modifiedNet.gateCount() << "/" << rawData.gateCount() << endl;
//            cout << "    " << r.errorCount << "/" << r.nSamples << endl;
//            double areaDec = ((double)(rawData.gateCount() - modifiedNet.gateCount())) / ((double)(rawData.gateCount()));
//            double accuDec = ((double)(r.errorCount)) / ((double)(r.nSamples));
//            if ( (accuDec < areaDec) && (modifiedNet.gateCount() < hisGateNum.back()) ) {
//                initFileName = outFileName;
//                cout << "   this is ok, next round." << endl;
//                hisGateNum.push_back(modifiedNet.gateCount());
//                break;
//            } else {
//                cout << "   this failed, go one search." << endl;
//                continue;
//            }
//        }
//    }
//
//}

