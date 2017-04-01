//
// Created by tripack on 17-3-7.
//

/*
 * Precomputation Program
 * Pre-computes Decomposition results for 2-5 input patterns
 * Also used for observing the techs.
 */

#include <cinttypes>
#include <iostream>
#include <initializer_list>
#include <array>
#include <numeric>
#include <bitset>

#include "common.h"

uint8_t lut3[256] = {0};
uint8_t lut3_2[256] = {0};

using std::cout;
using std::endl;
using std::bitset;

int main() {
    std::vector<uint16_t> t = selectWithout(4, 2);
    std::cout << std::bitset<32>(std::bitset<32>("101010")) << std::endl;

    for (auto x : t) {
        cout << std::bitset<4>(x) << endl;
    }

//    do3input(lut3);
//    do3inputs2(lut3_2);
//    int count = 0;
//    int sum = 0;
//    for (int i = 0; i < 256; i++) {
//        std::cout << std::bitset<8>(i) << " : " << (int)(lut3[i]) << std::endl;
//        if (lut3[i] > 0) count++;
//        sum += lut3[i];
//    }
//    std::cout << count << " " << sum << std::endl;
//    for (int i = 0; i < 256; i++) {
//        bool x = lut3[i] > 0;
//        bool y = lut3_2[i] > 0;
//        //cout << x << " " << y << endl;
//        if (x != y) cout << i << endl;
//    }

    return 0;
}


