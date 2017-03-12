//
// Created by tripack on 16-11-18.
//

#include <string>
#include <vector>
#include <iostream>
#include <truth_table.h>
#include <fstream>
#include <cstdlib>

#include "../MFFC/interface.h"
#include "../MFFC/err_asses.h"
#include "../approxiamate/src/search_method_core.h"
#include "../blif_replace/blif_replace.h"
#include "../common/generator.h"
#include "../policy/policy.h"

using std::string;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::set;

class FilenameGenerator : public Generator<string> {
    string base;
    string exten;
    int count;
public:
    FilenameGenerator(string baseName, string extention) {
        base = baseName;
        exten = extention;
        count = 0;
    }
    int genState() {
        return count;
    }
    string generate() override {
        return (base+std::to_string(count++)+exten);
    }
    bool hasEnded() const override {
        return false;
    }
    Generator<string> *clone() const override {
        assert(0);
        return nullptr;
    }
};

//=======================================================

int main(int argc, char* agrv[]) {

    string base="c880";
    string exten="blif";
    string vizExten="viz";
    string pngExten="png";
    string initFileName = base+"."+exten;
    BlifBooleanNet rawData(initFileName);
    cout << rawData.name() << " " << rawData.gateCount() << " " << rawData.nInputs() << " " << rawData.nOutputs() << endl;
    string withFileName = base +"_with." +exten;
    FilenameGenerator fnGen(base+"__","."+exten);
    FilenameGenerator gener1(base+"_","_unmodied."+vizExten);
    FilenameGenerator gener2(base+"_","_modied."+vizExten);
    FilenameGenerator gener1_(base+"_","_unmodied."+pngExten);
    FilenameGenerator gener2_(base+"_","_modied."+pngExten);
    rawData.exportGraphViz(base + "."+vizExten);
    string command1 = "dot -Tpng -o "+base+"."+pngExten+" < "+base+"."+vizExten;
    //system(command1.c_str());
    string tmp="dot -Tpng -o "+base+"."+pngExten+" < "+base+"."+vizExten;
    //system(tmp.c_str());

    std::vector<int > hisGateNum;
    hisGateNum.push_back(rawData.gateCount());

    while (fnGen.genState() < 15) {

        cout << "round " << fnGen.genState() << endl;
        string outFileName = fnGen.generate();
        BlifBooleanNet initNet(initFileName);
        //BlifBooleanNet mffc = initNet.getMFFC(4, 9);
        FfcSelectPolicy policy;
        while (true) {
            BlifBooleanNet::FFC chosenFfc = selectFFC(initNet.getFFC(), policy);
            string replaceFileName = "ffc.blif";
            initNet.exportFfcToBlifFile(chosenFfc, replaceFileName);
            BlifBooleanNet ffc(replaceFileName);
            cout << "  FFC inputs: " << ffc.nInputs() << endl;
            string name1 = gener1.generate();
            initNet.exportGraphVizwithHighlight(name1, ffc.totalNodeSet(), "red");
            string tmp1 = "dot -Tpng -o " + gener1_.generate() + " < " + name1;
            //system(tmp1.c_str());

            TruthTable initFfcTruthTable = ffc.truthTable();
            clock_t timeStart = clock();
            TruthTable finalFfcTruthTable = writeApproxBlifFileByTruthTable_BFS(initFfcTruthTable, withFileName);
            clock_t timeEnd = clock();
            cout << "    time:" << ((double) (timeEnd - timeStart)) / CLOCKS_PER_SEC << " s\n";
            replacePartialBlif(initFileName,
                               replaceFileName,
                               withFileName,
                               outFileName);
            BlifBooleanNet withNet(withFileName);
            BlifBooleanNet modifiedNet(outFileName);
            BlifCompareResult r = sampleCompareBlifs(rawData, modifiedNet, (1 << 20));
            string name2 = gener2.generate();
            modifiedNet.exportGraphVizwithHighlight(name2, withNet.totalNodeSet(), "cyan");
            string tmp2 = "dot -Tpng -o " + gener2_.generate() + " < " + name2;
            //system(tmp2.c_str());
            cout << "    " << modifiedNet.gateCount() << "/" << rawData.gateCount() << endl;
            cout << "    " << r.errorCount << "/" << r.nSamples << endl;
            double areaDec = ((double)(rawData.gateCount() - modifiedNet.gateCount())) / ((double)(rawData.gateCount()));
            double accuDec = ((double)(r.errorCount)) / ((double)(r.nSamples));
            if ( (accuDec < areaDec) && (modifiedNet.gateCount() < hisGateNum.back()) ) {
                initFileName = outFileName;
                cout << "   this is ok, next round." << endl;
                hisGateNum.push_back(modifiedNet.gateCount());
                break;
            } else {
                cout << "   this failed, go one search." << endl;
                continue;
            }
        }
    }

}

