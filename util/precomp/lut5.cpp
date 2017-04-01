//
// Created by tripack on 17-3-11.
//

#include "lut5.h"
#include "common.h"

#include <cinttypes>
#include <vector>

using std::vector;

void do5input(uint8_t* lut5,
              const uint16_t* lut4, const int nLut4,
              const uint8_t* lut3, const int nLut3) {

    // Decompose as 1 vs 3
    for (int i = 0; i < nLUT1; ++i) {
        uint64_t left = lut1[i];
        for (int j = 0; j < nLut4; ++j) {
            uint64_t right = lut4[j];
            vector<uint64_t> llist{
                    expand<5>(1, left, bin2int({1, 0, 0, 0, 0})),
                    expand<5>(1, left, bin2int({0, 1, 0, 0, 0})),
                    expand<5>(1, left, bin2int({0, 0, 1, 0, 0})),
                    expand<5>(1, left, bin2int({0, 0, 0, 1, 0})),
                    expand<5>(1, left, bin2int({0, 0, 0, 0, 1})),
            };
            vector<uint64_t> rlist{
                    expand<5>(4, right, bin2int({0, 1, 1, 1, 1})),
                    expand<5>(4, right, bin2int({1, 0, 1, 1, 1})),
                    expand<5>(4, right, bin2int({1, 1, 0, 1, 1})),
                    expand<5>(4, right, bin2int({1, 1, 1, 0, 1})),
                    expand<5>(4, right, bin2int({1, 1, 1, 1, 0})),
            };
            for (int k = 0; k < 5; ++k) {
                uint32_t l = llist.at(k);
                uint32_t r = rlist.at(k);
                uint32_t And = l & r;
                uint32_t nAnd = ~And;
                uint32_t Xor = l ^ r;
                uint32_t nXor = ~Xor;

                lut5[l] = 1;
                lut5[r] = 1;
                lut5[And] = 1;
                lut5[nAnd] = 1;
                lut5[Xor] = 1;
                lut5[nXor] = 1;
            }
        }
    }

    // Decompose as 2 vs 3
    for (int i = 0; i < nLUT2; ++i) {
        uint64_t left = lut2[i];
        for (int j = 0; j < nLut3; ++j) {
            uint64_t right = lut3[j];
            vector<uint64_t> llist{
                    expand<5>(2, left, bin2int({1, 0, 0, 0, 1})),
                    expand<5>(2, left, bin2int({1, 0, 0, 1, 0})),
                    expand<5>(2, left, bin2int({1, 0, 1, 0, 0})),
                    expand<5>(2, left, bin2int({1, 1, 0, 0, 0})),
                    expand<5>(2, left, bin2int({0, 1, 0, 0, 1})),
                    expand<5>(2, left, bin2int({0, 1, 0, 1, 0})),
                    expand<5>(2, left, bin2int({0, 1, 1, 0, 0})),
                    expand<5>(2, left, bin2int({0, 0, 1, 0, 1})),
                    expand<5>(2, left, bin2int({0, 0, 1, 1, 0})),
                    expand<5>(2, left, bin2int({0, 0, 0, 1, 1})),
            };
            vector<uint64_t> rlist{
                    expand<5>(3, right, bin2int({0, 1, 1, 1, 0})),
                    expand<5>(3, right, bin2int({0, 1, 1, 0, 1})),
                    expand<5>(3, right, bin2int({0, 1, 0, 1, 1})),
                    expand<5>(3, right, bin2int({0, 0, 1, 1, 1})),
                    expand<5>(3, right, bin2int({1, 0, 1, 1, 0})),
                    expand<5>(3, right, bin2int({1, 0, 1, 0, 1})),
                    expand<5>(3, right, bin2int({1, 0, 0, 1, 1})),
                    expand<5>(3, right, bin2int({1, 1, 0, 1, 0})),
                    expand<5>(3, right, bin2int({1, 1, 0, 0, 1})),
                    expand<5>(3, right, bin2int({1, 1, 1, 0, 0})),
            };
            for (int k = 0; k < 10; ++k) {
                uint64_t l = (rlist.at(k));
                uint32_t r = (rlist.at(k));
                uint32_t And = l & r;
                uint32_t nAnd = ~And;
                uint32_t Xor = l ^ r;
                uint32_t nXor = ~Xor;

                lut5[l] = 1;
                lut5[r] = 1;
                lut5[And] = 1;
                lut5[nAnd] = 1;
                lut5[Xor] = 1;
                lut5[nXor] = 1;
            }
        }
    }
}