//
// Created by tripack on 16-10-26.
//

#ifndef VE490_BOOLEAN_FUNCTION_H
#define VE490_BOOLEAN_FUNCTION_H

#include "object.h"
#include <memory>
#include <stdint.h>
#include "conts.h"
#include <iostream>

using std::unique_ptr;
using std::tuple;

class KMap : public Object {
    int *height, *width;
    int **table;

public:
    KMap();
    KMap(int *hei, int *wid, int **tab);
    ~KMap();

    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>> divide();


};

class BooleanFunction : public Object {

    int portName[PORT_SIZE];
    int truthTable[TRUTH_TABLE_SIZE];

public:

    unique_ptr<KMap> *getKMap(int *portPart1, int *portPart2);
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>> divide(int *portPart1, int *portPart2);

    bool operator() (int64_t input);
    BooleanFunction();
    BooleanFunction(int *portname, int portSize, int *truthtable);
    ~BooleanFunction();

};

#endif //VE490_BOOLEAN_FUNCTION_H
