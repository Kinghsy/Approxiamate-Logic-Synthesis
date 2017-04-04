//
// Created by tripack on 17-4-1.
//

#include "common.h"

PRECOM_RESULT(1) getLut1Result() {
    PrecompResultAttribute<1> rstRelavant;
    rstRelavant.maskLeft = std::bitset<1>("1");
    rstRelavant.maskRight = std::bitset<1>("0");
    rstRelavant.maskDiscard = std::bitset<1>("0");
    rstRelavant.leftFun = "NONE";
    rstRelavant.rightFun = "NONE";

    PrecompResultAttribute<1> rstIrrelavant = rstRelavant;
    rstIrrelavant.maskDiscard = std::bitset<1>("1");

    return PRECOM_RESULT(1) {
//            { FUN(1)("00"), rstIrrelavant },
            { FUN(1)("01"), rstRelavant },
//            { FUN(1)("11"), rstIrrelavant },
            { FUN(1)("10"), rstRelavant },
    };
}

PRECOM_RESULT(1) lut1Rst = getLut1Result();

PRECOM_RESULT(2) getLut2Result() {
    PRECOM_RESULT(2) result;

    auto comb = selectWithin<2>(1);
    for (const auto& lpair : lut1Rst) {
//        FUN(kOut) left = upCast<kIn1, kOut>(lpair.first());
        for (const auto& rpair : lut1Rst) {
//            FUN(kOut) right = upCast<kIn2, kOut>(rpair.first());
            for (auto c : comb) {
                PrecompResultAttribute<2> attr;

                auto l = expand<2, 1>(lpair.first, c);
                auto r = expand<2, 1>(rpair.first, ~c);
                auto And = l & r;
                auto nAnd = ~And;
                auto Xor = l ^ r;
                auto nXor = ~Xor;

                attr.maskLeft = c;
                attr.maskRight = ~c;
                attr.leftFun = lpair.first.to_string();
                attr.rightFun = rpair.first.to_string();
                attr.maskDiscard =
                        std::bitset<2>(
                                lpair.second.maskDiscard.to_string() +
                                rpair.second.maskDiscard.to_string()
                        );

                attr.combine = "LEFT";
                result.insert(std::make_pair(l, attr));
                attr.combine = "RIGHT";
                result.insert(std::make_pair(r, attr));
                attr.combine = "AND";
                result.insert(std::make_pair(And, attr));
                attr.combine = "NAND";
                result.insert(std::make_pair(nAnd, attr));
                attr.combine = "XOR";
                result.insert(std::make_pair(Xor, attr));
                attr.combine = "NXOR";
                result.insert(std::make_pair(nXor, attr));
            }
        }
    }

    return result;
}


PRECOM_RESULT(2) lut2Rst = getLut2Result();