//
// Created by tripack on 17-3-9.
//
#include "common.h"
#include "lut3.h"
#include <vector>
#include <unordered_set>
#include <iostream>
#include <bitset>

using std::vector;

std::unordered_set<uint16_t>
do3input() {
    std::unordered_set<uint16_t> lut3;
    std::vector<uint16_t> comb;

    comb = selectWithin(3, 1);
    for (int i = 0; i < nLUT1; ++i) {
        uint8_t left= lut1[i];
        for (int j = 0; j < nLUT2; ++j) {
            uint8_t right = lut2[j];
            for (auto c : comb) {
                uint8_t l = (uint8_t)(expand<3>(1, left, c));
                uint8_t r = (uint8_t)(expand<3>(2, right, ~c));
//                std::cout << std::bitset<8>(l) << " | "
//                          << std::bitset<8>(r) << std::endl;

                uint8_t And = l & r;
                uint8_t nAnd = ~And;
                uint8_t Xor = l ^ r;
                uint8_t nXor = ~Xor;

                lut3.insert(l);
                lut3.insert(r);
                lut3.insert(And);
                lut3.insert(nAnd);
                lut3.insert(Xor);
                lut3.insert(nXor);
            }
        }
    }
    return lut3;
}