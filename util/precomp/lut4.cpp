//
// Created by tripack on 17-3-11.
//

#include "lut4.h"
#include "common.h"

#include <cinttypes>
#include <vector>
#include <unordered_set>

using std::vector;

PRECOM_RESULT(4) do4input(const PRECOM_RESULT(3)& lut3Rst) {

    PRECOM_RESULT(4) result;

    enumerate<4, 1, 3>(result, lut1Rst, lut3Rst);
    enumerate<4, 2, 2>(result, lut2Rst, lut2Rst);

    return result;
}
