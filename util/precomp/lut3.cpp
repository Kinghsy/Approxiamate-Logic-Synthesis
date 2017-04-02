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

PRECOM_RESULT(3) do3input() {
    PRECOM_RESULT(3) result;

    enumerate<3, 1, 2>(result, lut1Rst, lut2Rst);

    return result;
}