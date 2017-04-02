//
// Created by tripack on 17-4-1.
//

#include "lut6.h"
#include "common.h"

#include <cinttypes>
#include <vector>
#include <bitset>
#include <unordered_set>

using std::vector;

std::unordered_set<uint64_t>
do6input(const std::unordered_set<uint32_t>& lut5,
         const std::unordered_set<uint16_t>& lut4,
         const std::unordered_set<uint16_t>& lut3) {

    std::vector<uint16_t> comb;
    std::unordered_set<uint64_t> lut6;

    vector<uint16_t> selectors = selectWithin(6, 1);

    // Decompose as 1 vs 5
    comb = selectWithin(6, 1);
    for (int i = 0; i < nLUT1; ++i) {
        uint64_t left = lut1[i];
        for (uint64_t right : lut5) {
            for (auto c : comb) {
                uint64_t l = expand<6>(1, left, c);
                uint64_t r = expand<6>(5, right, ~c);
                uint64_t And = l & r;
                uint64_t nAnd = ~And;
                uint64_t Xor = l ^r;
                uint64_t nXor = ~Xor;

                lut6.insert(l);
                lut6.insert(r);
                lut6.insert(And);
                lut6.insert(nAnd);
                lut6.insert(Xor);
                lut6.insert(nXor);
            }
        }
    }

    // Decompose as 2 vs 4
    comb = selectWithin(6, 2);
    for (int i = 0; i < nLUT2; ++i) {
        uint64_t left = lut2[i];
        for (uint64_t right : lut4) {
            for (auto c : comb) {
                uint64_t l = expand<6>(2, left, c);
                uint64_t r = expand<6>(4, right, ~c);
                uint64_t And = l & r;
                uint64_t nAnd = ~And;
                uint64_t Xor = l ^r;
                uint64_t nXor = ~Xor;

                lut6.insert(l);
                lut6.insert(r);
                lut6.insert(And);
                lut6.insert(nAnd);
                lut6.insert(Xor);
                lut6.insert(nXor);
            }
        }
    }

    // Decompose as 3 vs 3
    comb = selectWithin(6, 3);
    for (uint64_t left : lut3) {
        for (uint64_t right : lut3) {
            for (auto c : comb) {
                uint64_t l = expand<6>(3, left, c);
                uint64_t r = expand<6>(3, right, ~c);
                uint64_t And = l & r;
                uint64_t nAnd = ~And;
                uint64_t Xor = l ^r;
                uint64_t nXor = ~Xor;

                lut6.insert(l);
                lut6.insert(r);
                lut6.insert(And);
                lut6.insert(nAnd);
                lut6.insert(Xor);
                lut6.insert(nXor);
            }
        }
    }

    return lut6;
}