//
// Created by tripack on 17-3-11.
//

#include "lut5.h"
#include "common.h"

#include <cinttypes>
#include <vector>
#include <unordered_set>

using std::vector;

std::unordered_set<uint32_t>
do5input(const std::unordered_set<uint16_t>& lut4,
         const std::unordered_set<uint16_t>& lut3) {

    std::vector<uint16_t> comb;
    std::unordered_set<uint32_t> lut5;


    // Decompose as 1 vs 3
    comb = selectWithin(5, 1);
    for (int i = 0; i < nLUT1; ++i) {
        uint32_t left = lut1[i];
        for (uint32_t right : lut4) {
            for (auto c : comb) {
                uint32_t l = expand<5>(1, left, c);
                uint32_t r = expand<5>(4, right, ~c);
                uint32_t And = l & r;
                uint32_t nAnd = ~And;
                uint32_t Xor = l ^ r;
                uint32_t nXor = ~Xor;

                lut5.insert(l)   ;
                lut5.insert(r)   ;
                lut5.insert(And) ;
                lut5.insert(nAnd);
                lut5.insert(Xor) ;
                lut5.insert(nXor);
            }
        }
    }

    // Decompose as 2 vs 3
    comb = selectWithin(5, 2);
    for (int i = 0; i < nLUT2; ++i) {
        uint32_t left = lut2[i];
        for (uint32_t right : lut3) {
            for (auto c : comb) {
                uint32_t l = expand<5>(2, left, c);
                uint32_t r = expand<5>(3, right, ~c);
                uint32_t And = l & r;
                uint32_t nAnd = ~And;
                uint32_t Xor = l ^ r;
                uint32_t nXor = ~Xor;

                lut5.insert(l)   ;
                lut5.insert(r)   ;
                lut5.insert(And) ;
                lut5.insert(nAnd);
                lut5.insert(Xor) ;
                lut5.insert(nXor);
            }
        }
    }

    return lut5;
}