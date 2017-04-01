//
// Created by tripack on 17-3-11.
//

#include "lut4.h"
#include "common.h"

#include <cinttypes>
#include <vector>

using std::vector;

void do4input(uint8_t* lut4,
              const uint8_t* lut3, const int nLut3) {
    // Decompose as 1 vs 3
    for (int i = 0; i < nLUT1; ++i) {
        uint64_t left = lut1[i];
        for (int j = 0; j < nLut3; ++j) {
            uint64_t right = lut3[j];
            vector<uint64_t> llist{
                    expand<4>(1, left, bin2int({1, 0, 0, 0})),
                    expand<4>(1, left, bin2int({0, 1, 0, 0})),
                    expand<4>(1, left, bin2int({0, 0, 1, 0})),
                    expand<4>(1, left, bin2int({0, 0, 0, 1})),
            };
            vector<uint64_t> rlist{
                    expand<4>(3, right, bin2int({0, 1, 1, 1})),
                    expand<4>(3, right, bin2int({1, 0, 1, 1})),
                    expand<4>(3, right, bin2int({1, 1, 0, 1})),
                    expand<4>(3, right, bin2int({1, 1, 1, 0})),
            };
            for (int k = 0; k < 4; ++k) {
                uint16_t l = (uint16_t)(llist.at(k));
                uint16_t r = (uint16_t)(rlist.at(k));
                uint16_t And = l & r;
                uint16_t nAnd = ~And;
                uint16_t Xor = l ^ r;
                uint16_t nXor = ~Xor;

                lut4[l] = 1;
                lut4[r] = 1;
                lut4[And] = 1;
                lut4[nAnd] = 1;
                lut4[Xor] = 1;
                lut4[nXor] = 1;
            }
        }
    }

    for (int i = 0; i < nLUT2; ++i) {
        uint16_t left = lut2[i];
        for (int j = 0; j < nLUT2; ++j) {
            uint16_t right = lut2[j];
            vector<uint64_t> llist{
                    expand<4>(2, left, bin2int({1, 0, 0, 1})),
                    expand<4>(2, left, bin2int({1, 0, 1, 0})),
                    expand<4>(2, left, bin2int({1, 1, 0, 0})),
                    expand<4>(2, left, bin2int({0, 1, 0, 1})),
                    expand<4>(2, left, bin2int({0, 1, 1, 0})),
                    expand<4>(2, left, bin2int({0, 0, 1, 1})),
            };
            vector<uint64_t> rlist{
                    expand<4>(2, right, bin2int({0, 1, 1, 0})),
                    expand<4>(2, right, bin2int({0, 1, 0, 1})),
                    expand<4>(2, right, bin2int({0, 0, 1, 1})),
                    expand<4>(2, right, bin2int({1, 0, 1, 0})),
                    expand<4>(2, right, bin2int({1, 0, 0, 1})),
                    expand<4>(2, right, bin2int({1, 1, 0, 0})),
            };
            for (int k = 0; k < 6; ++k) {
                uint16_t l = (uint16_t)(rlist.at(k));
                uint16_t r = (uint16_t)(rlist.at(k));
                uint16_t And = l & r;
                uint16_t nAnd = ~And;
                uint16_t Xor = l ^ r;
                uint16_t nXor = ~Xor;

                lut4[l] = 1;
                lut4[r] = 1;
                lut4[And] = 1;
                lut4[nAnd] = 1;
                lut4[Xor] = 1;
                lut4[nXor] = 1;
            }
        }
    }
}
