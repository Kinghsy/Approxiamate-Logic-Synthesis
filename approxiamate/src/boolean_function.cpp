//
// Created by king on 16-11-2.
//

#include "boolean_function.h"
#include <memory>
#include <cstring>
#include "conts.h"

using std::unique_ptr;
using std::shared_ptr;


KMap::KMap() {
    height=NULL;
    width=NULL;
    table=NULL;
    return ;
}

KMap::KMap(int *hei, int *wid, int **tab) {
    height=hei;
    width=wid;
    table=tab;
    return ;
}

KMap::~KMap() {
    delete height;
    delete width;
    delete[] table;
    return ;
}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>> KMap::divide() {


}

BooleanFunction::BooleanFunction() {
    for (int i = 0; i < PORT_SIZE; ++i) {
        portName[i]=0;
    }
    for (int j = 0; j < TRUTH_TABLE_SIZE; ++j) {
        truthTable[j]=0;
    }
    return ;
}

BooleanFunction::BooleanFunction(int *portname, int portSize, int *truthtable) {
    for (int i = 0; i < PORT_SIZE; ++i) {
        portName[i]=portname[i];
    }
    for (int j = 0; j < TRUTH_TABLE_SIZE; ++j) {
        truthTable[j]=truthtable[j];
    }
}

BooleanFunction::~BooleanFunction() {
    return ;
}

unique_ptr<KMap>* BooleanFunction::getKMap(int *portPart1, int *portPart2) {
    int size1=0;
    int size2=0;

}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>> BooleanFunction::divide(int *portPart1,
                                                                                        int *portPart2) {

}