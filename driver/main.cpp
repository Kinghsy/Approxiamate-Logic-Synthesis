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

using std::string;
using std::vector;
using std::cout;

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

    string base="9symml";
    string exten="blif";
    string initFileName = base+"."+exten;
    BlifBooleanNet rawData(initFileName);
    cout << rawData.name() << " " << rawData.gateCount() << " " << rawData.nInputs() << " " << rawData.nOutputs() << endl;
    string withFileName = base +"_with." +exten;
    FilenameGenerator fnGen(base+"__","."+exten);
    rawData.exportGraphViz(base + ".viz");


    while (fnGen.genState() < 1) {

        cout << "round " << fnGen.genState() << endl;
        string outFileName = fnGen.generate();
        BlifBooleanNet initNet(initFileName);
        BlifBooleanNet mffc = initNet.getMFFC(4, 6);
        cout << "MFFC inputs: " << mffc.nInputs() << endl;
        string replaceFileName = "mffc.blif";
        TruthTable initMffcTruthTable = mffc.truthTable();
        TruthTable finalMffcTruthTable = writeApproxBlifFileByTruthTable(initMffcTruthTable, withFileName);
        replacePartialBlif(initFileName,
                           replaceFileName,
                           withFileName,
                           outFileName);
        BlifBooleanNet modifiedNet(outFileName);
        BlifCompareResult r = sampleCompareBlifs(rawData, modifiedNet, (1 << 20));
        cout << "    " << modifiedNet.gateCount()  << "/" << rawData.gateCount() << endl;
        cout << "    " << r.errorCount << "/" << r.nSamples << endl;
        initFileName=outFileName;

    }

}

