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

    DBitset expandTo(const DBitset& mask) const;

public:

    TTable(const DBitset& table, size_t nInputs) {
        assert((1ul << nInputs) == table.size());
        inputSize = nInputs;
        data = table;
    };

    TTable(unsigned long long int value, size_t nInputs)
            : data(1ul << nInputs, value), inputSize(nInputs) {}

    TTable(const std::string& valStr, size_t nInputs) {
        size_t length = valStr.size();
        assert(length == (1ul << nInputs));
        inputSize = nInputs;
        data = DBitset(valStr);
    }

    TTable(const std::string& valStr) {
        size_t length = valStr.size();
        assert(length != 0);
        size_t shift = __builtin_ctz(length);
        assert(length == (1ul << shift));
        inputSize = shift;
        data = DBitset(valStr);
    }

    inline size_t nInputs() {return this->inputSize;}

    inline DBitset::reference operator[](size_t term) {
        assert(term < (1ul << inputSize));
        return data[term];
    }

    inline DBitset::reference operator[](const DBitset& term) {
        assert(term.size() == inputSize);
        return data[term.to_ulong()];
    }

    inline DBitset::const_reference operator[](size_t term) const {
        assert(term < (1ul << inputSize));
        return data[term];
    }

    inline DBitset::const_reference operator[](const DBitset& term) const {
        assert(term.size() == inputSize);
        return data[term.to_ulong()];
    }

    inline TTable& set(size_t term, bool b) {
        assert(term < (1ul << inputSize));
        data.set(term, b);
        return *this;
    }

    // returns reference to itself
    inline TTable& set(const DBitset& term, bool b) {
        assert(term.size() == inputSize);
        data.set(term.to_ulong(), b);
        return *this;
    }

    inline TTable operator^(const TTable& t2) const {
        assert(this->inputSize == t2.inputSize);
        return TTable(this->data ^ t2.data, inputSize);
    }

    inline bool operator== (const TTable& t2) const {
        assert(this->inputSize == t2.inputSize);
        return this->data == t2.data;
    }

    inline TTable& flip()  {data.flip(); return *this;} // Returns reference to itself
    inline TTable& reset() {data.reset(); return *this;} // Returns reference to itself
    inline size_t count() { return data.count();}


    TTable project(std::unordered_map<size_t, bool> condition) const;

    friend TTable combineTruthTable(
            const TTable& t1, const TTable& t2,
            const DBitset& t1Mask, const DBitset& t2Mask,
            const TTable& method);

    TTable cofactor(size_t input, bool = true) const;
};

const TTable NOT_1_INPUT= TTable("10");
const TTable NORMAL_1_INPUT= TTable("01");

const TTable XOR_TABLE= TTable("0110");
const TTable AND_TABLE= TTable("0001");
const TTable OR_TABLE = TTable("0111");

TTable combineTruthTable(
        const TTable& t1, const TTable& t2,
        const DBitset& t1Mask, const DBitset& t2Mask,
        const TTable& method);



#endif //VE490_TRUTH_TABLE_H
