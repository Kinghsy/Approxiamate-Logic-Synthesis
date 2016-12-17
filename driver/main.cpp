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

using std::string;
using std::vector;
using std::cout;

int main(int argc, char* agrv[]) {

    string initFileName = "9symml.blif";
    string withFileName = "9symml_with.blif";
    string outFileName = "9symml_final.blif";

    BlifBooleanNet initNet(initFileName);
    BlifBooleanNet mffc = initNet.getMFFC(4, 6);
    string replaceFileName = "mffc.blif";
    TruthTable initMffcTruthTable = mffc.truthTable();
    TruthTable finalMffcTruthTable = writeApproxBlifFileByTruthTable(initMffcTruthTable, withFileName);
    replacePartialBlif(initFileName, replaceFileName, withFileName, outFileName);
    BlifBooleanNet modifiedNet(outFileName);
    BlifCompareResult r = sampleCompareBlifs(initNet, modifiedNet);
    cout << initNet.gateCount() << "/" << modifiedNet.gateCount() << endl;
    cout << r.errorCount << "/" << r.nSamples;
}
