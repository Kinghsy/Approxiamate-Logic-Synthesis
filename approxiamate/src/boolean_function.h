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

class KMap;
typedef std::shared_ptr<KMap> KMapPtr;

class KMap {//: public Object {

public:

    int height, width;
    int *(*table);

    bool operator==(const KMap &b);
    void display();

    KMap();
    KMap(const int &hei, const int &wid, int **tab);
    ~KMap();


};

class BooleanFunction;

typedef std::shared_ptr<BooleanFunction> BooleanFunctionPtr;

class BooleanFunction {//: public Object {

    int portSize;
    int *portName;//[PORT_SIZE];
    int *truthTable;//[TRUTH_TABLE_SIZE];
    int inputNum; // the number of inputs for this Boolean Function.

    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int> findMinError1(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int> findMinError2(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int> findMinError3(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int> findMinError4(const KMap &kmap, int *portPart1, int *portPart2);
  //boolean function of part1, boolean function of part2, the error, the opeartion
    std::tuple<int, int> findDifference(int len, int *mode0, int *mode1, int *goal);
  //error, answer(0 or 1); if used mode[0] then goes 0, else goes 1.
    void findMajorRow(const KMap &kmap, int *majorRow, int *notSure, int &deterNum, int &notSureNum, const int &mode);
    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int> findBestMajorRow(const KMap &kmap, int *majorRow, int *notSure, const int &deterNum, const int &notSureNum, const int &mode, int *portPart1, int *portPart2);


public:

    KMapPtr getKMap(int *portPart1, int *portPart2);
    //the first part as the height and second part as width
    std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int>
        divide(int *portPart1, int *portPart2);
    //boolean function of part1, boolean function of part2, the error, the opeartion
    std::string toString();
    int booleanCompare(BooleanFunction &b);
    BooleanFunctionPtr combine(BooleanFunction &b, const int oper);

    bool isAll0s();
    bool isAll1s();
    int getInputNum();
    int getPortSize();
    int *getPortName();
    int *getTruthTable();
    BooleanFunctionPtr copy();

    //bool operator() (int64_t input);
    bool operator== (const BooleanFunction &b);
    BooleanFunction();
    BooleanFunction(int *portname, int portsize, int *truthtable);
    BooleanFunction(BooleanFunction &init);
    ~BooleanFunction();

};

//BooleanFunction initialBoolFunction;

#endif //VE490_BOOLEAN_FUNCTION_H
