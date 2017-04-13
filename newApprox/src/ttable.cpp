//
// Created by tripack on 17-4-12.
//

#include "const.h"
#include "ttable.h"

// Duplicates the first n bits and fill them in the "l"
// fillWith<16>(4, 0010 0100 0111 0110B)
//  => (0110 0110 0110 0110B)
static DBitset fillWith(size_t n, DBitset f) {
    return f.size() == n ? (f) : fillWith(n << 1, f | (f << n));
}

static inline  DBitset ones(size_t length, size_t n) {
    DBitset mask(n, 0);
    mask.set().resize(length);
    return mask;
}

// Does post-split masking
// Splits 0 - 2^(n - 1)
// Example m = 5, n = 3
//          0000 0000 0000 0000 1111 1111 1111 1111
//          1101 1010
// 1)       1111
// 2)                           1111
// 3)       1111                1111
// Repeats these first 32 bits on remaining sections
DBitset expansionMask(size_t k, size_t m, size_t n) {

    size_t halfLengthN = 1ul << (n - 1);
    size_t halfLengthM = 1ul << (m - 1);
    DBitset mask = ones(1ul << k, halfLengthN);

    return fillWith(1ul << m, mask | (mask << halfLengthM));
}

DBitset TTable::expandTo(const DBitset &mask) const {
    assert(mask.count() == this->inputSize);

    size_t k = mask.size();
    size_t n = mask.count();

    DBitset uf = this->data;
    uf.resize(1ul << k);

//    std::cout << "Mask: " << mask << std::endl;
    for (size_t i = k; i > 0; i--) {
        size_t halfL = 1ul << (i - 1);
        size_t halfN = 1ul << (n - 1);
//        std::cout << "uf = " << uf << std::endl;
//        std::cout << "i = " << i << std::endl;
//        std::cout << "n = " << n << std::endl;
        if (mask.test(i - 1)) {
//            std::cout << "Split." << std::endl;
            // This bit is used, we split!
            uf = uf | (uf << (halfL - halfN));
            uf &= expansionMask(k, i, n);
            n--;
        } else {
            uf = uf | (uf << halfL);
//            std::cout << "Fill." << std::endl;
        }
    }
    return uf;
}


TTable combineTruthTable(const TTable &t1, const TTable &t2,
                         const DBitset &t1Mask, const DBitset &t2Mask,
                         const TTable &method) {
    assert(t1.inputSize == t1Mask.count());
    assert(t2.inputSize == t2Mask.count());
    assert(t1Mask == ~t2Mask);
    size_t combinedInput = t1.inputSize + t2.inputSize;
    DBitset result1 = t1.expandTo(t1Mask);
    DBitset result2 = t2.expandTo(t2Mask);
    if (method == XOR_TABLE)
        return TTable(result1 ^ result2, combinedInput);
    if (method == AND_TABLE)
        return TTable(result1 & result2, combinedInput);
    if (method == OR_TABLE)
        return TTable(result1 & result2, combinedInput);

    assert(0);
}

TTable TTable::cofactor(size_t n, bool t) const{
    assert(inputSize > 1);
    DBitset result(1ul << (inputSize - 1));
    int counter = 0;
    for (size_t i = 0; i < (1 << inputSize); i++) {
        if (DBitset(inputSize, i).test(n) == t){
            result[counter++] = data[i];
        } else
            continue;
    }
    assert(counter == result.size());
    return TTable(result, inputSize - 1);
}


TTable TTable::project(std::unordered_map<size_t, bool> condition) const {
    assert(condition.size() < inputSize);
    size_t resultSize = condition.size() - inputSize;
    DBitset result(1ul << resultSize);
    int counter = 0;
    for (size_t i = 0; i < (1 << inputSize); i++) {
        bool take = true;
        for (auto &&item : condition) {
            bool current = DBitset(inputSize, i).test(item.first);
            bool expected = item.second;
            if (current != expected) {
                take = false;
                break;
            }
        }
        if(take) result[counter++] = data[i];
    }
    assert(counter == result.size());
    return TTable(result, inputSize - 1);
}
