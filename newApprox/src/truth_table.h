//
// Created by king on 17-4-11.
//

#ifndef VE490_T_TABLE_H
#define VE490_T_TABLE_H

#include <boost/dynamic_bitset.hpp>
#include "const.h"
#include <unordered_map>

class KMap;

class TTable {
    size_t inputSize;
    DBitset data;

public:

    TTable(const DBitset& table, size_t nInputs);
    TTable(unsigned long long int value);
    TTable(const std::string& valStr);

    TTable cofactor(size_t input, bool = true);

    friend TTable combineTruthTable(
            const TTable& t1, const TTable& t2,
            const DBitset& t1Mask, const DBitset& t2Mask);

    DBitset::reference operator[](size_t term);
    DBitset::reference operator[](const DBitset& term);
    TTable& set(size_t term, bool); // returns reference to itself
    TTable& set(const DBitset&, bool);

    TTable operator^(const TTable& t2);

    TTable& flip(); // Returns reference to itself
    TTable& reset(); // Returns reference to itself

    size_t count();

    KMap getKMap(std::vector<size_t> node1,
                 std::vector<size_t> node2);

    TTable project(std::unordered_map<size_t, bool> condition,
                   std::vector<size_t> onList);

    size_t nInputs();
};

const TTable XOR_TABLE= TTable("0110");
const TTable AND_TABLE= TTable("0001");
const TTable OR_TABLE = TTable("0111");



#endif //VE490_TRUTH_TABLE_H
