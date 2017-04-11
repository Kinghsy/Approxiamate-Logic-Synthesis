//
// Created by king on 17-4-11.
//

#ifndef VE490_TRUTH_TABLE_H
#define VE490_TRUTH_TABLE_H

#include <boost/dynamic_bitset.hpp>

#include "boolean_function.h"
#include "kmap.h"
#include "const.h"

class TruthTable {
    size_t inputSize;

public:

    TruthTable cofactor(size_t input, bool = true);
    friend combineTruthTable(TruthTable t1, TruthTable t2, dyBitset t1Mask, dyBitset t2Mask);

    int operator[](size_t term);
    int operator[](dyBitset term);

    dyBitset diff(TruthTable table);

    Kmap getKMap(std::vector<int> node1,
                 std::vector<int> node2);
    TruthTable project(std::map<int, int> condition,
                       std::vector<int> onList);
    size_t nInputs();
}



#endif //VE490_TRUTH_TABLE_H
