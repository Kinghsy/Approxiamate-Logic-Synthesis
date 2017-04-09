//
// Created by tripack on 17-4-1.
//

#include "common.h"


PRECOM_RESULT(3) do3input() {
    PRECOM_RESULT(3) result;

    enumerate<3, 1, 2>(result, lut1Rst, lut2Rst);

    return result;
}

PRECOM_RESULT(4) do4input(const PRECOM_RESULT(3)& lut3Rst) {

    PRECOM_RESULT(4) result;

    enumerate<4, 1, 3>(result, lut1Rst, lut3Rst);
    enumerate<4, 2, 2>(result, lut2Rst, lut2Rst);

    return result;
}

PRECOM_RESULT(5) do5input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst) {

    PRECOM_RESULT(5) result;

    enumerate<5, 1, 4>(result, lut1Rst, lut4Rst);
    enumerate<5, 2, 3>(result, lut2Rst, lut3Rst);

    return result;
}

PRECOM_RESULT(6) do6input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst,
        const PRECOM_RESULT(5)& lut5Rst) {

    PRECOM_RESULT(6) result;

    enumerate<6, 1, 5>(result, lut1Rst, lut5Rst);
    enumerate<6, 2, 4>(result, lut2Rst, lut4Rst);
    enumerate<6, 3, 3>(result, lut3Rst, lut3Rst);

    return result;
}

PRECOM_RESULT(7) do7input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst,
        const PRECOM_RESULT(5)& lut5Rst,
        const PRECOM_RESULT(6)& lut6Rst) {

    PRECOM_RESULT(7) result;

    std::cout << "begin" << std::endl;
    //enumerate<7, 1, 6>(result, lut1Rst, lut6Rst);
    std::cout << "16" << std::endl;
    enumerate<7, 2, 5>(result, lut2Rst, lut5Rst);
    std::cout << "25" << std::endl;
    enumerate<7, 3, 4>(result, lut3Rst, lut4Rst);
    std::cout << "34" << std::endl;

    return result;
}