//
// Created by king on 17-4-11.
//

#ifndef VE490_BOOLEAN_FUNCTION_H
#define VE490_BOOLEAN_FUNCTION_H

#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <map>

#include "const.h"
#include "kmap.h"
#include "truth_table.h"

class BooleanFunction {
private:
    int inputSize;
    TruthTable truthTab;
    std::vector<std::string> portName;
    std::string outPortName;
public:
    BooleanFucntion(const int &inputSize,
                    const TruthTable& truthTab,
                    const std::vector<std::string>& portName,
                    const std::string& outPortName);
    ~BooleanFunction();

    void operator= (const BooleanFunction &initBF);
    bool opeator== (const BooleanFunction &initBF);
    size_t operator^(const BooleanFunction &initBF);

    int getInputSize();
    int getVal(const DBitset& term);
    int getVal(const size_t term);
    bool isAll0s();
    bool isAll1s();

    friend combineMethod findMethod(const BooleanFunction& bf);

    std::string getPortName(const int& i);
    std::string getOutPortName();
}
#endif //VE490_BOOLEAN_FUNCTION_H
