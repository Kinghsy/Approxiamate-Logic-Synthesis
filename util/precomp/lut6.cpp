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
do6input(const uint32_t *lut5, const int nLut5,
         const uint16_t *lut4, const int nLut4,
         const uint8_t *lut3, const int nLut3) {

    std::unordered_set<uint64_t> lut6;

    // Decompose as 1 vs 3
    for (int i = 0; i < nLUT1; ++i) {
        uint64_t left = lut1[i];
        for (int j = 0; j < nLut5; ++j) {
            uint64_t right = lut5[j];
            vector<uint64_t> llist{
                    expand<6>(1, left, bin2int({1, 0, 0, 0, 0, 0})),
                    expand<6>(1, left, bin2int({0, 1, 0, 0, 0, 0})),
                    expand<6>(1, left, bin2int({0, 0, 1, 0, 0, 0})),
                    expand<6>(1, left, bin2int({0, 0, 0, 1, 0, 0})),
                    expand<6>(1, left, bin2int({0, 0, 0, 0, 1, 0})),
                    expand<6>(1, left, bin2int({0, 0, 0, 0, 0, 1})),
            };
            vector<uint64_t> rlist{
                    expand<6>(5, right, bin2int({0, 1, 1, 1, 1, 1})),
                    expand<6>(5, right, bin2int({1, 0, 1, 1, 1, 1})),
                    expand<6>(5, right, bin2int({1, 1, 0, 1, 1, 1})),
                    expand<6>(5, right, bin2int({1, 1, 1, 0, 1, 1})),
                    expand<6>(5, right, bin2int({1, 1, 1, 1, 0, 1})),
                    expand<6>(5, right, bin2int({1, 1, 1, 1, 0, 1})),
            };
            for (int k = 0; k < 6; ++k) {
                uint64_t l = llist.at(k);
                uint64_t r = rlist.at(k);
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
    for (int i = 0; i < nLUT2; ++i) {
        uint64_t left = lut2[i];
        for (int j = 0; j < nLut4; ++j) {
            uint64_t right = lut4[j];
            vector<uint64_t> llist{
                    expand<6>(2, left, bin2int({1, 0, 0, 0, 0, 1})),
                    expand<6>(2, left, bin2int({1, 0, 0, 0, 1, 0})),
                    expand<6>(2, left, bin2int({1, 0, 0, 1, 0, 0})),
                    expand<6>(2, left, bin2int({1, 0, 1, 0, 0, 0})),
                    expand<6>(2, left, bin2int({1, 1, 0, 0, 0, 0})),

                    expand<6>(2, left, bin2int({0, 1, 0, 0, 0, 1})),
                    expand<6>(2, left, bin2int({0, 1, 0, 0, 1, 0})),
                    expand<6>(2, left, bin2int({0, 1, 0, 1, 0, 0})),
                    expand<6>(2, left, bin2int({0, 1, 1, 0, 0, 0})),

                    expand<6>(2, left, bin2int({0, 0, 1, 0, 0, 1})),
                    expand<6>(2, left, bin2int({0, 0, 1, 0, 1, 0})),
                    expand<6>(2, left, bin2int({0, 0, 1, 1, 0, 0})),

                    expand<6>(2, left, bin2int({0, 0, 0, 1, 0, 1})),
                    expand<6>(2, left, bin2int({0, 0, 0, 1, 1, 0})),

                    expand<6>(2, left, bin2int({0, 0, 0, 0, 1, 1})),
            };
            vector<uint64_t> rlist{
                    expand<6>(4, right, bin2int({0, 1, 1, 1, 1, 0})),
                    expand<6>(4, right, bin2int({0, 1, 1, 1, 0, 1})),
                    expand<6>(4, right, bin2int({0, 1, 1, 0, 1, 1})),
                    expand<6>(4, right, bin2int({0, 1, 0, 1, 1, 1})),
                    expand<6>(4, right, bin2int({0, 0, 1, 1, 1, 1})),

                    expand<6>(4, right, bin2int({1, 0, 1, 1, 1, 0})),
                    expand<6>(4, right, bin2int({1, 0, 1, 1, 0, 1})),
                    expand<6>(4, right, bin2int({1, 0, 1, 0, 1, 1})),
                    expand<6>(4, right, bin2int({1, 0, 0, 1, 1, 1})),

                    expand<6>(4, right, bin2int({1, 1, 0, 1, 1, 0})),
                    expand<6>(4, right, bin2int({1, 1, 0, 1, 0, 1})),
                    expand<6>(4, right, bin2int({1, 1, 0, 0, 1, 1})),

                    expand<6>(4, right, bin2int({1, 1, 1, 0, 1, 0})),
                    expand<6>(4, right, bin2int({1, 1, 1, 0, 0, 1})),

                    expand<6>(4, right, bin2int({1, 1, 1, 1, 0, 0})),
            };
            for (int k = 0; k < 15; ++k) {
                uint64_t l = (rlist.at(k));
                uint64_t r = (rlist.at(k));
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
    for (int i = 0; i < nLut3; ++i) {
        uint64_t left = lut3[i];
        for (int j = 0; j < nLut3; ++j) {
            uint64_t right = lut3[j];
            vector<uint64_t> llist{
                    expand<6>(3, left, bin2int({1, 1, 0, 0, 0, 1})),
                    expand<6>(3, left, bin2int({1, 1, 0, 0, 1, 0})),
                    expand<6>(3, left, bin2int({1, 1, 0, 1, 0, 0})),
                    expand<6>(3, left, bin2int({1, 1, 1, 0, 0, 0})),

                    expand<6>(3, left, bin2int({1, 0, 1, 0, 0, 1})),
                    expand<6>(3, left, bin2int({1, 0, 1, 0, 1, 0})),
                    expand<6>(3, left, bin2int({1, 0, 1, 1, 0, 0})),
                    
                    expand<6>(3, left, bin2int({1, 0, 0, 1, 0, 1})),
                    expand<6>(3, left, bin2int({1, 0, 0, 1, 1, 0})),
                    
                    expand<6>(3, left, bin2int({1, 0, 0, 0, 1, 1})),

                    expand<6>(3, left, bin2int({0, 1, 1, 0, 0, 1})),
                    expand<6>(3, left, bin2int({0, 1, 1, 0, 1, 0})),
                    expand<6>(3, left, bin2int({0, 1, 1, 1, 0, 0})),

                    expand<6>(3, left, bin2int({0, 1, 0, 1, 0, 1})),
                    expand<6>(3, left, bin2int({0, 1, 0, 1, 1, 0})),
                    
                    expand<6>(3, left, bin2int({0, 1, 0, 0, 1, 1})),

                    expand<6>(3, left, bin2int({0, 0, 1, 1, 0, 1})),
                    expand<6>(3, left, bin2int({0, 0, 1, 1, 1, 0})),
                    
                    expand<6>(3, left, bin2int({0, 0, 1, 0, 1, 1})),
                    
                    expand<6>(3, left, bin2int({0, 0, 0, 1, 1, 1})),
            };
            vector<uint64_t> rlist{
                    expand<6>(3, left, bin2int({0, 0, 1, 1, 1, 0})),
                    expand<6>(3, left, bin2int({0, 0, 1, 1, 0, 1})),
                    expand<6>(3, left, bin2int({0, 0, 1, 0, 1, 1})),
                    expand<6>(3, left, bin2int({0, 0, 0, 1, 1, 1})),

                    expand<6>(3, left, bin2int({0, 1, 0, 1, 1, 0})),
                    expand<6>(3, left, bin2int({0, 1, 0, 1, 0, 1})),
                    expand<6>(3, left, bin2int({0, 1, 0, 0, 1, 1})),

                    expand<6>(3, left, bin2int({0, 1, 1, 0, 1, 0})),
                    expand<6>(3, left, bin2int({0, 1, 1, 0, 0, 1})),

                    expand<6>(3, left, bin2int({0, 1, 1, 1, 0, 0})),

                    expand<6>(3, left, bin2int({1, 0, 0, 1, 1, 0})),
                    expand<6>(3, left, bin2int({1, 0, 0, 1, 0, 1})),
                    expand<6>(3, left, bin2int({1, 0, 0, 0, 1, 1})),

                    expand<6>(3, left, bin2int({1, 0, 1, 0, 1, 0})),
                    expand<6>(3, left, bin2int({1, 0, 1, 0, 0, 1})),

                    expand<6>(3, left, bin2int({1, 0, 1, 1, 0, 0})),

                    expand<6>(3, left, bin2int({1, 1, 0, 0, 1, 0})),
                    expand<6>(3, left, bin2int({1, 1, 0, 0, 0, 1})),

                    expand<6>(3, left, bin2int({1, 1, 0, 1, 0, 0})),

                    expand<6>(3, left, bin2int({1, 1, 1, 0, 0, 0})),
            };
            for (int k = 0; k < 20; ++k) {
                uint64_t l = (rlist.at(k));
                uint64_t r = (rlist.at(k));
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