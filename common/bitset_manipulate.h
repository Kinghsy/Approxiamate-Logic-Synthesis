//
// Created by tripack on 17-4-13.
//

#ifndef VE490_BITSET_MANIPULATE_H
#define VE490_BITSET_MANIPULATE_H

#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>
#include <iostream>

typedef boost::dynamic_bitset<> DBitset;

static inline std::string bitset2Str(const DBitset& bitset) {
    std::string s;
    boost::to_string(bitset, s);
    return s;
}

// For a bitset of length "length", fill its least significant
// n bits using 1
static inline DBitset ones(size_t length, size_t n) {
    DBitset mask(n, 0);
    mask.set().resize(length);
    return mask;
}


// Duplicates the first n bits and fill them in the "l"
// fillWith<16>(4, 0010 0100 0111 0110B)
//  => (0110 0110 0110 0110B)
static inline DBitset fillWith(size_t n, DBitset f) {
    f &= ones(f.size(), n);
    while (f.size() > n) {
        f |= f << n;
        n <<= 1;
    }
    return f;
}


static inline DBitset extract(const DBitset& bitset,
                              const std::vector<size_t> ind) {
    DBitset result(ind.size());
    for (int i = 0; i < ind.size(); ++i)
        result[i] = bitset.test(ind[i]);
    return result;
};

static inline DBitset extract(const DBitset& bitset,
                              const DBitset& mask) {
    std::vector<size_t> ind;
    size_t x = mask.find_first();
    while (x != mask.npos) {
        ind.push_back(x);
        x = mask.find_next(x);
    }
    return extract(bitset, ind);
};

static inline size_t getLSB(size_t bitset) {
    assert(bitset != 0);
    return __builtin_ctz(bitset);
}

static inline size_t getLSB(const DBitset& bitset) {
    assert(!bitset.empty());
    return bitset.find_first();
}

static inline DBitset catBitset(const DBitset& d1, const DBitset& d2) {
    DBitset result = d1;
    DBitset ld2 = d2;
    ld2.resize(d1.size() + d2.size());
    result.resize(d1.size() + d2.size());
    result |= ld2 << (d1.size());
    return result;
}

static inline DBitset operator+(const DBitset& d1, const DBitset& d2) {
    return catBitset(d1, d2);
}


// Puts a one dimensional bitset into a 2D array
// with inputs specified by col on the column
static std::vector<DBitset>
breakdown(const DBitset& bitset,
          const DBitset& row, const DBitset& col) {
    assert(row.size() == col.size());
    assert(row == ~col);
    assert(bitset.size() == (1ul << row.size()));

    size_t total = row.size();
    size_t width = 1ul << (row.count());
    size_t height = 1ul << (col.count());

    std::vector<DBitset> result(height, DBitset(width));

    for (size_t i = 0; i < bitset.size(); i++) {
        auto currInd = DBitset(total, i);
        auto key = extract(currInd, col);
        auto rowKey = extract(currInd, row);
        result[key.to_ulong()][rowKey.to_ulong()] = bitset[i] ;
    }

    return result;
}

template <typename T>
std::vector<T> pickByDbitset(const std::vector<T>& v,
                             const DBitset& set) {
    assert(set.size() == v.size());
    std::vector<T> ret;
    for (size_t i = 0; i < set.size(); i++) {
        if (set.test(i)) ret.push_back(v.at(i));
    }
    return ret;
}

template <typename T>
DBitset vec2bitset(const std::vector<T>& v){
    DBitset result(v.size());
    auto data = v.data();
    for (size_t i = 0; i < v.size(); i++) {
        result[i] = data[i] > 0 ? 1 : 0;
    }
    return result;
}



#endif //VE490_BITSET_MANIPULATE_H
