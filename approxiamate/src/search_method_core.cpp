//
// Created by king on 16-12-17.
//

#include "boolean_function.h"
#include "conts.h"
#include "search.h"
#include "search_method_core.h"

#include "../../common/truth_table.h"

using namespace std;


TruthTable calculApproxTruthTable(TruthTable &truthTab, int ActivedMode) {
    ActivedModeApplied = ActivedMode;
    if (ActivedMode & FULL_SEARCH) {
        return calculApproxTruthTable_Full(truthTab);
    } else
    if (ActivedMode & BRANCH_AND_BOUND) {
        return calculApproxTruthTable_BranchAndBound(truthTab);
    } else
    if (ActivedMode & BFS_SEARCH) {
        return calculApproxTruthTable_BFS(truthTab);
    } else {
        assert(0);
    }
}
TruthTable calculApproxTruthTable_BranchAndBound(TruthTable &truthTab) {

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
    wholeSearch.getRootSpace()->printSearchSpace(); //===================
    while ((ssPtr=wholeSearch.getNextSearchSpace())!= nullptr) {
        ssPtr->printSearchSpace(); //=========================
    }
    ssPtr=wholeSearch.getBestSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;

}
TruthTable calculApproxTruthTable_Full(TruthTable &truthTab) {

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
    wholeSearch.getRootSpace()->printSearchSpace(); //===================
    while ((ssPtr=wholeSearch.getNextSearchSpace_Full())!= nullptr) {
        ssPtr->printSearchSpace(); //=========================
    }
    ssPtr=wholeSearch.getBestSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;

}
TruthTable calculApproxTruthTable_BFS(TruthTable &truthTab) {

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
    wholeSearch.getRootSpace()->printSearchSpace(); //===================
    while ((ssPtr=wholeSearch.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace(); //=========================
    }
    ssPtr=wholeSearch.getBestSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;

}


TruthTable writeApproxBlifFileByTruthTable(TruthTable &truthTable, std::string BlifFileName, int ActivedMode) {
    ActivedModeApplied = ActivedMode;
    if (ActivedMode & FULL_SEARCH) {
        return writeApproxBlifFileByTruthTable_Full(truthTable, BlifFileName);
    } else
    if (ActivedMode & BRANCH_AND_BOUND) {
        return writeApproxBlifFileByTruthTable_BranchAndBound(truthTable, BlifFileName);
    } else
    if (ActivedMode & BFS_SEARCH) {
        return writeApproxBlifFileByTruthTable_BFS(truthTable, BlifFileName);
    } else {
        assert(0);
    }
}
TruthTable writeApproxBlifFileByTruthTable_BFS(TruthTable &truthTab, string BlifFileName) {

    //cout << "BFS Search Called." << endl;

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
    while ((ssPtr=wholeSearch.getNextSearchSpace_BFS())!= nullptr) {
    }
    ssPtr=wholeSearch.getBestSpace();
    ssPtr->generateBlifFile(BlifFileName, truthTab);
    //ssPtr->printSearchSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;
}
TruthTable writeApproxBlifFileByTruthTable_Full(TruthTable &truthTab, string BlifFileName) {

    //cout << "Full Search Called." << endl;

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
    while ((ssPtr=wholeSearch.getNextSearchSpace_Full())!= nullptr) {
    }
    ssPtr=wholeSearch.getBestSpace();
    ssPtr->generateBlifFile(BlifFileName, truthTab);
    //ssPtr->printSearchSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;
}
TruthTable writeApproxBlifFileByTruthTable_BranchAndBound(TruthTable &truthTab, string BlifFileName) {

    //cout << "Branch And Bound Search Called." << endl;

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
    //ssPtr->printSearchSpace();
    BooleanFunction finalBF(*(ssPtr->getFinalBooleanFuntion()));
    truthTable=finalBF.getTruthTable();

    TruthTable finalTruthTab(truthTab);

    for (int i = 0; i < truthTableSize; ++i)
        finalTruthTab[i]=truthTable[i];

    return finalTruthTab;
}
