//
// Created by tripack on 17-3-11.
//

#include "lut4.h"
#include "common.h"

#include <cinttypes>
#include <vector>
#include <unordered_set>

using std::vector;

std::unordered_set<uint16_t>
do4input(const std::unordered_set<uint16_t>& lut3) {
    std::unordered_set<uint16_t> lut4;
    std::vector<uint16_t> comb;

    // Decompose as 1 vs 3
    comb = selectWithin(4, 1);
    for (int i = 0; i < nLUT1; ++i) {
        uint64_t left = lut1[i];
        for (uint16_t right : lut3) {
            for (auto c : comb) {
                uint16_t l = (uint16_t)(expand<4>(1, left, c));
                uint16_t r = (uint16_t)(expand<4>(3, right, ~c));
                uint16_t And = l & r;
                uint16_t nAnd = ~And;
                uint16_t Xor = l ^ r;
                uint16_t nXor = ~Xor;

                lut4.insert(l);
                lut4.insert(r);
                lut4.insert(And);
                lut4.insert(nAnd);
                lut4.insert(Xor);
                lut4.insert(nXor);
            }
        }
    }

    comb = selectWithin(4, 2);
    for (int i = 0; i < nLUT2; ++i) {
        uint16_t left = lut2[i];
        for (int j = 0; j < nLUT2; ++j) {
            uint16_t right = lut2[j];
            for (auto c : comb) {
                uint16_t l = (uint16_t)(expand<4>(2, left, c));
                uint16_t r = (uint16_t)(expand<4>(2, right, ~c));
                uint16_t And = l & r;
                uint16_t nAnd = ~And;
                uint16_t Xor = l ^ r;
                uint16_t nXor = ~Xor;

                lut4.insert(l);
                lut4.insert(r);
                lut4.insert(And);
                lut4.insert(nAnd);
                lut4.insert(Xor);
                lut4.insert(nXor);
            }
        }
    }

    return lut4;
}
