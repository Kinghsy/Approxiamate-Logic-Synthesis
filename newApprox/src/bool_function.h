//
// Created by king on 17-4-10.
//

#ifndef VE490_BOOL_FUNCTION_H
#define VE490_BOOL_FUNCTION_H

#include <vector>

#include "kmap.h"
#include "../../circuit_profile/sim_profile.h"

class BoolFunction {

private:

    int inputSize;
    std::vector<int > truthTable; // consist of 0/1
    std::vector<std::string > portName;
    FocusedSimulationResult simRes;

    int divideMode; // e.g. divideMode = XOR_IGNORE

public:

    Kmap buildKmap(std::vector<std::string >, std::vector<std::string >);
    BoolFunction combineWith(const BoolFunction& a, const int& opera);
    bool divideAble();

    void operator= (const BoolFunction& initFun);
    bool operator== (const BoolFunction& initFun);
    int operator[] (const int& i); // return one of its truthtable
    int operator^ (const BoolFunction& initFun);

    BoolFunction();
    BoolFunction(const BoolFunction& initFun);
    BoolFunction(std::vector<int > truthTable, std::vector<std::string >, FocusedSimulationResult simRes);
    ~BoolFunction();

    void displayName();
    void displayWhole();

};

#endif //VE490_BOOL_FUNCTION_H
