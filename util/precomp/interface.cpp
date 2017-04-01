//
// Created by king on 17-3-31.
//

#include "common.h"
#include "interface.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;
using std::tuple;


std::tuple<std::vector<uint8_t>, std::vector<uint16_t >, std::vector<uint32_t > > preDecomp() {
    uint8_t* lut3 = new uint8_t[256];
    for (int i = 0; i < 256; ++i) {
        lut3[i] = 0;
    }
    uint8_t* lut4 = new uint8_t[65536];
    for (int i = 0; i < 65536; ++i) {
        lut4[i] = 0;
    }
    uint8_t* lut5 = new uint8_t[65536ll * 65536ll];
    for (int64_t i = 0; i < 65536ll * 65536ll; i++) {
        lut5[i] = 0;
    }

    std::vector<uint8_t> decomp_lut3;
    std::vector<uint16_t> decomp_lut4;
    std::vector<uint32_t> decomp_lut5;

    do3input(lut3);

    for (int i = 0; i < 256; ++i) {
        if (lut3[i] > 0)
            decomp_lut3.push_back(i);
    }

    do4input(lut4, decomp_lut3.data(), decomp_lut3.size());

    for (int i = 0; i < 65536; ++i) {
        if (lut4[i] > 0)
            decomp_lut4.push_back(i);
    }

    do5input(lut5,
             decomp_lut4.data(), decomp_lut4.size(),
             decomp_lut3.data(), decomp_lut3.size());

    for (int64_t i = 0; i < 65536ll * 65536ll; i++) {
        if (lut5[i] > 0)
            decomp_lut5.push_back(i);
    }
    //std::cout << std::endl;
    //std::cout << "lut3.size():" << decomp_lut3.size() << std::endl;
    //std::cout << "lut4.size():" << decomp_lut4.size() << std::endl;
    //std::cout << "lut5.size():" << decomp_lut5.size() << std::endl;
    delete[] lut3;
    delete[] lut4;
    delete[] lut5;

    return std::make_tuple(std::move(decomp_lut3), std::move(decomp_lut4), std::move(decomp_lut5));
}