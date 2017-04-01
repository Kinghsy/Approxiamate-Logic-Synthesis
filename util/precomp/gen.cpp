
// Created by tripack on 17-3-11.
//

#include "common.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;
using std::unordered_set;

std::unordered_set<uint16_t>
do3input();

std::unordered_set<uint16_t>
do4input(const std::unordered_set<uint16_t>& lut3);

std::unordered_set<uint32_t>
do5input(const std::unordered_set<uint16_t>& lut4,
         const std::unordered_set<uint16_t>& lut3);

std::unordered_set<uint64_t>
do6input(const std::unordered_set<uint32_t>& lut5,
         const std::unordered_set<uint16_t>& lut4,
         const std::unordered_set<uint16_t>& lut3);

int main() {

    auto lut3 = do3input();

    cout << "Total " << lut3.size() << " decomposable LUT3s." << endl;

    auto lut4 = do4input(lut3);

    cout << "Total " << lut4.size() << " decomposable LUT4s." << endl;

    auto lut5 = do5input(lut4, lut3);

    
    cout << "Total " << lut5.size() << " decomposable LUT5s." << endl;

    auto lut6 = do6input(lut5, lut4, lut3);

    cout << "Total " << lut6.size() << " decomposable LUT6s" << endl;

    return 0;
}
