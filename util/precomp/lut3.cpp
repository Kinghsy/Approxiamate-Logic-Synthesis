//
// Created by tripack on 17-3-9.
//
#include "common.h"
#include "lut3.h"
#include <vector>

using std::vector;

inline uint8_t switch12(uint8_t boolf) {
    // 8 7 6 5 4 3 2 1 -> 8 7 4 3 6 5 1 2
    uint8_t cMask = bin2int({1, 1, 0, 0, 0, 0, 1, 1});
    uint8_t lMask = bin2int({0, 0, 1, 1, 0, 0, 0, 0});
    uint8_t rMask = bin2int({0, 0, 0, 0, 1, 1, 0, 0});
    return (boolf & cMask)
           | ((boolf & rMask) << 2)
           | ((boolf & lMask) >> 2);
}

inline uint8_t switch13(uint8_t boolf) {
    // 8 7 6 5 4 3 2 1 -> 8 4 6 2 7 3 5 1
    uint8_t cMask = bin2int({1, 0, 1, 0, 0, 1, 0, 1});
    uint8_t lMask = bin2int({0, 0, 0, 0, 1, 0, 1, 0});
    uint8_t rMask = bin2int({0, 1, 0, 1, 0, 0, 0, 0});
    return (boolf & cMask)
           | ((boolf & rMask) >> 3)
           | ((boolf & lMask) << 3);
}

//void do3inputs(uint8_t* lut3) {
//    /*
//     * 3 input LUT has 256 possible combinations.
//     * Each function corresponds to a unique 0-255 integer
//     * A 3 input LUT is decomposable iff
//     * The bottom up-approach:
//     * there are 3 possible structures:
//     * ((x1, x2), x3) (x1, (x2, x3)) ((x1, x3) x2))
//     *  For each one, the combing function can only be
//     * 1. Constant 1/0
//     * 2. x AND y, NOT (x AND y)
//     * 3. Just x or y
//     * 4. x XOR y, NOT (x XOR y)
//     * Where the first one only generates constant 1 or 2
//     * Essentially that leaves us 5 choices.
//     *
//     * The algorithm goes
//     * 1. Pick 2 functions to combine
//     * 2. Pick a combinator
//     * 3. Combines them together
//     */
//
//
//    lut3[bin2int({0, 0, 0, 0, 0, 0, 0, 0})] = 1;
//    lut3[bin2int({1, 1, 1, 1, 1, 1, 1, 1})] = 1;
//
//
//
//    // F1, 1 in 1 out, pick f(x) = x
//    // (x1, (x2, x3))
//    uint8_t left = bin2int({0, 0, 0, 0, 1, 1, 1, 1});
//    for (int i = 0; i < nLUT2; ++i) {
//        uint8_t right = lut2[i] | (lut2[i] << 4);
//
//        unsigned char rAnd = left & right;
//        unsigned char rXor = left ^ right;
//        unsigned char rnAnd = ~left & right;
//        unsigned char rnXor = ~left ^ right;
//
//        uint8_t ind[11] = {
//                rAnd, ~rAnd, rXor, ~rXor,
//                rnAnd, ~rnAnd, rnXor, ~rnXor,
//                left, ~left, right
//        };
//
//        for (int j = 0; j < 11; ++j) {
//            lut3[ind[j]]++;
//            lut3[switch12(ind[j])]++;
//            lut3[switch13(ind[j])]++;
//        }
//    }
//}


void do3inputs2(uint8_t* lut3) {
    for (int i = 0; i < nLUT1; ++i) {
        for (int j = 0; j < nLUT2; ++j) {
            uint8_t l1 = expand<3>(1, lut1[i], bin2int({1, 0, 0}));
            uint8_t l2 = expand<3>(1, lut1[i], bin2int({0, 1, 0}));
            uint8_t l3 = expand<3>(1, lut1[i], bin2int({0, 0, 1}));
            uint8_t r1 = expand<3>(2, lut2[j], bin2int({0, 1, 1}));
            uint8_t r2 = expand<3>(2, lut2[j], bin2int({1, 0, 1}));
            uint8_t r3 = expand<3>(2, lut2[j], bin2int({1, 1, 0}));

            uint8_t And1 = l1 & r1;
            uint8_t nAnd1 = ~And1;
            uint8_t Xor1 = l1 ^ r1;
            uint8_t nXor1 = ~Xor1;

            uint8_t And2 = l2 & r2;
            uint8_t nAnd2 = ~And2;
            uint8_t Xor2 = l2 ^ r2;
            uint8_t nXor2 = ~Xor2;

            uint8_t And3 = l3 & r3;
            uint8_t nAnd3 = ~And3;
            uint8_t Xor3 = l3 ^ r3;
            uint8_t nXor3 = ~Xor3;


            lut3[And1]++;
            lut3[nAnd1]++;
            lut3[Xor1]++;
            lut3[nXor1]++;
            lut3[l1]++;
            lut3[r1]++;

            lut3[And2]++;
            lut3[nAnd2]++;
            lut3[Xor2]++;
            lut3[nXor2]++;
            lut3[l2]++;
            lut3[r2]++;

            lut3[And3]++;
            lut3[nAnd3]++;
            lut3[Xor3]++;
            lut3[nXor3]++;
            lut3[l3]++;
            lut3[r3]++;
        }
    }
}

void do3input(uint8_t* lut3) {
    for (int i = 0; i < nLUT1; ++i) {
        uint8_t left= lut1[i];
        for (int j = 0; j < nLUT2; ++j) {
            uint8_t right = lut2[j];
            vector<uint64_t> llist{
                    expand<3>(1, left, bin2int({1, 0, 0})),
                    expand<3>(1, left, bin2int({0, 1, 0})),
                    expand<3>(1, left, bin2int({0, 0, 1})),
            };
            vector<uint64_t> rlist{
                    expand<3>(2, right, bin2int({0, 1, 1})),
                    expand<3>(2, right, bin2int({1, 0, 1})),
                    expand<3>(2, right, bin2int({1, 1, 0})),
            };
            for (int k = 0; k < 3; ++k) {
                uint8_t l = (uint8_t)(llist.at(k));
                uint8_t r = (uint8_t)(rlist.at(k));
                uint8_t And = l & r;
                uint8_t nAnd = ~And;
                uint8_t Xor = l ^ r;
                uint8_t nXor = ~Xor;

                lut3[l]++;
                lut3[r]++;
                lut3[And]++;
                lut3[nAnd]++;
                lut3[Xor]++;
                lut3[nXor]++;
            }
        }
    }
}