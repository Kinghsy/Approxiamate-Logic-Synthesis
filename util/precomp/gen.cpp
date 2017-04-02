
// Created by tripack on 17-3-11.
//

#include "common.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

#include "do_lut.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::vector;
using std::unordered_set;

int main() {

    PRECOM_RESULT(3) lut3 = do3input();

    cout << "Total " << lut3.size() << " decomposable LUT3s." << endl;

    PRECOM_RESULT(4) lut4 = do4input(lut3);

    cout << "Total " << lut4.size() << " decomposable LUT4s." << endl;

    auto lut5 = do5input(lut3, lut4);

    
    cout << "Total " << lut5.size() << " decomposable LUT5s." << endl;

    auto lut6 = do6input(lut3, lut4, lut5);

    cout << "Total " << lut6.size() << " decomposable LUT6s" << endl;

    auto s = lut6.begin();
    for (int i = 0; i < 1400; ++i) s++;

    return 0;
}
