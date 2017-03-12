
// Created by tripack on 17-3-11.
//

#include "common.h"
#include "lut3.h"
#include "lut4.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;

int main() {
    uint8_t* lut3 = new uint8_t[256];
    uint8_t* lut4 = new uint8_t[65536];
    uint8_t* lut5 = new uint8_t[65536ll * 65536ll];

    std::vector<uint8_t> decomp_lut3;
    std::vector<uint16_t> decomp_lut4;
    std::vector<uint32_t> decomp_lut5;

    do3input(lut3);

    for (int i = 0; i < 256; ++i) {
        if (lut3[i] > 0)
            decomp_lut3.push_back(i);
    }

    cout << "Total " << decomp_lut3.size() << " decomposable LUT3s." << endl;

    do4input(lut4, decomp_lut3.data(), decomp_lut3.size());

    for (int i = 0; i < 65536; ++i) {
        if (lut4[i] > 0)
            decomp_lut4.push_back(i);
    }

    cout << "Total " << decomp_lut4.size() << " decomposable LUT4s." << endl;

    do5input(lut5,
             decomp_lut4.data(), decomp_lut4.size(),
             decomp_lut3.data(), decomp_lut3.size());

    for (int64_t i = 0; i < 65536ll * 65536ll; i++) {
        if (lut5[i] > 0)
            decomp_lut5.push_back(i);
    }

    cout << "Total " << decomp_lut5.size() << " decomposable LUT5s." << endl;

    return 0;
}
