//
// Created by tripack on 17-4-2.
//

#ifndef VE490_URANDOM_H
#define VE490_URANDOM_H

#include <vector>
#include <cstddef>
#include <bitset>

class Random {
public:
    static int randomInt();
    static std::vector<int> randomIntVector(int size);
    static std::vector<int> randomBitVector(int size);
    template<size_t l> static std::bitset<l> randomBitset();
};

#endif //VE490_URANDOM_H
