//
// Created by tripack on 16-10-13.
//

#include "data.h"

static const int single[] = {1};
static const int repeated[] = {1, 2, 2, -1 ,-9, -9};
static const int sorted[] = {1, 2, 3, 4, 5};
static const int reverseSorted[] = {10, 8, 6, 4, -2, 0};
static const int d1[] = {13, -8, 36, 21, -34, -7, -5, -23, -13, -1};
static const int d2[] = {-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,};
static const int d3[] = {-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,};
static const int d4[] = {17, 24, -48, 40, 44, 10, 23, -33, -7, -11,
                         -14, -4, -50, 4, 28};
static const int d5[] = {6, 5, 4, 3, 2, 1};

StaticSequenceGenerator<int> gSingle(single);
StaticSequenceGenerator<int> gRepeated(repeated);
StaticSequenceGenerator<int> gSorted(sorted);
StaticSequenceGenerator<int> gReversedSorted(reverseSorted);
StaticSequenceGenerator<int> gD1(d1);
StaticSequenceGenerator<int> gD2(d2);
StaticSequenceGenerator<int> gD3(d3);
StaticSequenceGenerator<int> gD4(d4);
StaticSequenceGenerator<int> gD5(d5);


const Generator<int> *generators[] = {
        &gSingle, &gRepeated, &gSorted, &gReversedSorted,
        &gD1, &gD2, &gD3, &gD4, &gD5
};

const int numGenerators = sizeof(generators) / sizeof(*generators);
