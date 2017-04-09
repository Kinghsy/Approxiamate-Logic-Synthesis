//
// Created by tripack on 17-4-1.
//

#ifndef VE490_DO_LUT_H
#define VE490_DO_LUT_H

#include "common.h"

PRECOM_RESULT(3) do3input();

PRECOM_RESULT(4) do4input(
        const PRECOM_RESULT(3)& lut3Rst);

PRECOM_RESULT(5) do5input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst);

PRECOM_RESULT(6) do6input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst,
        const PRECOM_RESULT(5)& lut5Rst);

PRECOM_RESULT(7) do7input(
        const PRECOM_RESULT(3)& lut3Rst,
        const PRECOM_RESULT(4)& lut4Rst,
        const PRECOM_RESULT(5)& lut5Rst,
        const PRECOM_RESULT(6)& lut6Rst);

#endif //VE490_DO_LUT_H
