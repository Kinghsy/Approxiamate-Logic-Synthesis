//
// Created by king on 17-3-31.
//

#include "gtest/gtest.h"
#include "../../util/precomp/interface.h"
#include "../../util/precomp/common.h"

using std::vector;
using std::tuple;
using std::get;
using std::move;

TEST(PreDecomp, TC1) {
    //std::tuple<std::vector<uint8_t>, std::vector<uint16_t >, std::vector<uint32_t > > result = preDecomp();
    std::vector<uint8_t > lut3;
    std::vector<uint16_t > lut4;
    std::vector<uint32_t > lut5;
    std::tie(lut3, lut4, lut5) = preDecomp();
    std::cout << std::endl;
    std::cout << "lut3.size():" << lut3.size() << std::endl;
    std::cout << "lut4.size():" << lut4.size() << std::endl;
    std::cout << "lut5.size():" << lut5.size() << std::endl;
    ASSERT_EQ(1, 1);
}