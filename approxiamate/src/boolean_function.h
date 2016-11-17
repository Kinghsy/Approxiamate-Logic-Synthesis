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

class KMap {//: public Object {

public:

    int height, width;
    int *(*table);

    KMap();
    KMap(int &hei, int &wid, int **tab);
    ~KMap();


};

class BooleanFunction {//: public Object {

    int portSize;
    int *portName;//[PORT_SIZE];
    int *truthTable;//[TRUTH_TABLE_SIZE];

    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> findMinError1(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> findMinError2(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> findMinError3(const KMap &kmap, int *portPart1, int *portPart2);
    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> findMinError4(const KMap &kmap, int *portPart1, int *portPart2);
  //boolean function of part1, boolean function of part2, the error, the opeartion
    std::tuple<int, int> findDifference(int len, int *mode0, int *mode1, int *goal);
  //error, answer(0 or 1); if used mode[0] then goes 0, else goes 1.
    void findMajorRow(const KMap &kmap, int *majorRow, int *notSure, int &deterNum, int &notSureNum, const int &mode);
    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> findBestMajorRow(const KMap &kmap, int *majorRow, int *notSure, const int &deterNum, const int &notSureNum, const int &mode, int *portPart1, int *portPart2);


public:

    std::unique_ptr<KMap> getKMap(int *portPart1, int *portPart2);
    //the first part as the height and second part as width
    std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int> divide(int *portPart1, int *portPart2);
    //boolean function of part1, boolean function of part2, the error, the opeartion
    std::string toString();

    //bool operator() (int64_t input);
    BooleanFunction();
    BooleanFunction(int *portname, int portsize, int *truthtable);
    ~BooleanFunction();

};

#endif //VE490_BOOLEAN_FUNCTION_H
