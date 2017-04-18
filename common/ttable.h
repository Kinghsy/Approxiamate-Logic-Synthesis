//
// Created by king on 17-4-11.
//

#ifndef VE490_T_TABLE_H
#define VE490_T_TABLE_H

#include "bitset_manipulate.h"
#include <unordered_map>
#include <ostream>

class TTable {
    DBitset data;
    size_t inputSize;

    DBitset expandTo(const DBitset& mask) const;

public:

    const DBitset& cdata() const {return data;}

    TTable(size_t nInputs) {
        assert(nInputs > 0);
        data = DBitset(1ul << nInputs);
    }

    TTable(const DBitset& table, size_t nInputs) {
        assert((1ul << nInputs) == table.size() && nInputs != 0);
        inputSize = nInputs;
        data = table;
    };

    TTable(const DBitset& table) {
        inputSize = getLSB(table.size());
        assert(table.size() == (1ul << inputSize));
        data = table;
    };

    TTable(const std::vector<size_t> v)
            : TTable(vec2bitset(v)) {}

    TTable(const std::vector<size_t> v, size_t nInputs)
            : TTable(vec2bitset(v), nInputs) {}

    TTable(unsigned long long int value, size_t nInputs)
            : data(1ul << nInputs, value), inputSize(nInputs) {}

    TTable(const std::string& valStr) {
        size_t length = valStr.size();
        assert(length > 1);
        size_t shift = __builtin_ctz(length);
        assert(length == (1ul << shift));
        inputSize = shift;
        data = DBitset(valStr);
    }

    TTable(const std::string& valStr, size_t nInputs) {
        size_t length = 1ul << nInputs;
        assert(length >= valStr.size());
        inputSize = nInputs;
        data = DBitset(valStr);
        data.resize(length);
    }

    inline size_t nInputs() const {return this->inputSize;}
    inline size_t nTerms() const {return data.size();}

    inline std::string toString() const {
        std::string s;
        boost::to_string(data, s);
        return s;
    }

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

    // Below 2 function intentionally left blank so that
    // using them gives you compile error.
    DBitset::reference operator[](const std::string& term);
    inline DBitset::const_reference operator[](const std::string& term) const;

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

    inline TTable operator~() const {
        return ~this->data;
    }

    inline TTable operator^(const TTable& t2) const {
        assert(this->inputSize == t2.inputSize);
        return TTable(this->data ^ t2.data, inputSize);
    }

    inline bool operator== (const TTable& t2) const {
        if (this->inputSize != t2.inputSize) return false;
        return this->data == t2.data;
    }

    inline TTable& flip()  {data.flip(); return *this;} // Returns reference to itself
    inline TTable& reset() {data.reset(); return *this;} // Returns reference to itself
    inline size_t count() { return data.count();}


    TTable project(const std::unordered_map<size_t, bool>& condition) const;

    TTable project(const std::vector<size_t>& location,
                   const DBitset& condition) const;

    std::vector<TTable> breakdown(const DBitset& row,
                                  const DBitset& col);

    TTable cofactor(size_t input, bool = true) const;

    friend void flipBinaryTTableInput(TTable& t, bool in1, bool in2);

    friend TTable combineTruthTable(
            const TTable& t1, const TTable& t2,
            const DBitset& t1Mask, const DBitset& t2Mask,
            const TTable& method);

    friend struct std::hash<TTable>;

    friend std::ostream &operator<<(std::ostream &os, const TTable &table);
    friend std::istream &operator>>(std::istream &is, TTable &table);
};

namespace std {
    template<>
    struct hash<TTable> {
        std::size_t operator()(const TTable& key) const {
            using boost::hash_value;
            using boost::hash_combine;
            size_t seed = 0;
            hash_combine(seed, hash_value(key.data.m_bits));
            hash_combine(seed, hash_value(key.inputSize));
            return seed;
        }
    };
}

const TTable NOT_1_INPUT        = TTable("01");
const TTable NORMAL_1_INPUT     = TTable("10");
const TTable ALL_0_1_INPUT      = TTable("00");
const TTable ALL_1_1_INPUT      = TTable("11");

const TTable XOR_TABLE              = TTable("0110");
const TTable NXOR_TABLE             = ~XOR_TABLE;
const TTable AND_TABLE              = TTable("1000");
const TTable NAND_TABLE             = ~AND_TABLE;
const TTable OR_TABLE               = TTable("1110");
const TTable LEFT_RELA_TABLE        = TTable("1010"); // only relevant to left.
const TTable LEFT_RELA_NOT_TABLE    = ~LEFT_RELA_TABLE; // only relevant to left and has an not gate.
const TTable RIGHT_RELA_TABLE       = TTable("1100"); // only relevant to right.
const TTable RIGHT_RELA_NOT_TABLE   = ~RIGHT_RELA_TABLE; // only relevant to right and has an not gate.
const TTable ALL_IRR_TABLE_0        = TTable("0000"); // irrelevant to both sides, and return 0 always.
const TTable ALL_IRR_TABLE_1        = ~ALL_IRR_TABLE_0; // irrelevant to both sides, and return 1 always.

TTable combineTruthTable(
        const TTable& t1, const TTable& t2,
        const DBitset& t1Mask, const DBitset& t2Mask,
        const TTable& method);

void flipBinaryTTableInput(TTable& t, bool in1, bool in2);


#endif //VE490_TRUTH_TABLE_H
