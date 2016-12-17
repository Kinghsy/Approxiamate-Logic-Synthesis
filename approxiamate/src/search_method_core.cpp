//
// Created by king on 16-12-17.
//

#include "boolean_function.h"
#include "conts.h"
#include "search.h"
#include "search_method_core.h"

#include "../../common/truth_table.h"

using namespace std;

TruthTable calculApproxTruthTable(TruthTable &truthTab) {

    int portSize=truthTab.numInput();
    int *portName=new int[portSize];
    for (int i = 0; i < portSize; ++i) portName[i]=1;
    int truthTableSize = (1 << portSize);
    int *truthTable=new int[truthTableSize];
    for (int i = 0; i < truthTableSize; ++i) truthTable[i]=(int)truthTab[i];

    BooleanFunction initBF(portName, portSize, truthTable);
    SearchTree wholeSearch(initBF);

    delete[] portName;
    delete[] truthTable;

    SearchSpacePtr ssPtr;
    while ((ssPtr=wholeSearch.getNextSearchSpace())!= nullptr) {
    }
    ssPtr=wholeSearch.getBestSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;

}


TruthTable writeApproxBlifFileByTruthTable(TruthTable &truthTab, string BlifFileName) {

    int portSize=truthTab.numInput();
    int *portName=new int[portSize];
    for (int i = 0; i < portSize; ++i) portName[i]=1;
    int truthTableSize = (1 << portSize);
    int *truthTable=new int[truthTableSize];
    for (int i = 0; i < truthTableSize; ++i) truthTable[i]=(int)truthTab[i];

    BooleanFunction initBF(portName, portSize, truthTable);
    SearchTree wholeSearch(initBF);

    delete[] portName;
    delete[] truthTable;

    SearchSpacePtr ssPtr;
    while ((ssPtr=wholeSearch.getNextSearchSpace())!= nullptr) {
    }
    ssPtr=wholeSearch.getBestSpace();
    ssPtr->generateBlifFile(BlifFileName, truthTab);
    ssPtr->printSearchSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;
}