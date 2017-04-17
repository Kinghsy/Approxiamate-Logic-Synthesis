
// Created by tripack on 17-3-11.
//

#include "common.h"
#include <common.h>
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
    StopWatch sw;
    sw.start();

    PRECOM_RESULT(3) lut3 = do3input(); sw.take();

    cout << "Total " << lut3.size() << " decomposable LUT3s." << endl;

    PRECOM_RESULT(4) lut4 = do4input(lut3); sw.take();

    cout << "Total " << lut4.size() << " decomposable LUT4s." << endl;

    auto lut5 = do5input(lut3, lut4); sw.take();

    
    cout << "Total " << lut5.size() << " decomposable LUT5s." << endl;

    auto lut6 = do6input(lut3, lut4, lut5); sw.take();
//
    cout << "Total " << lut6.size() << " decomposable LUT6s" << endl;
//
//    auto s = lut6.begin();
//    for (int i = 0; i < 1400; ++i) s++;

    FileClass pdb("pcdb");

    std::ofstream dbfile(PrecompDbPath / pdb("db"));

    dbfile << "102" << "\n"; // Database version

    for (const auto& e : lut3) {
        dbfile << e.first.size() << "\t" << e.first << "\t";
        const auto& attr = e.second;
        dbfile << attr.maskLeft.to_string() << "\t"
               << attr.maskRight.to_string() << "\t"
               << attr.leftFun << "\t"
               << attr.rightFun << "\t"
               << attr.combine << "\t"
               << attr.maskDiscard.to_string() << "\t";
        dbfile << std::endl;
    }

    for (const auto& e : lut4) {
        dbfile << e.first.size() << "\t" << e.first << "\t";
        const auto& attr = e.second;
        dbfile << attr.maskLeft.to_string() << "\t"
               << attr.maskRight.to_string() << "\t"
               << attr.leftFun << "\t"
               << attr.rightFun << "\t"
               << attr.combine << "\t"
               << attr.maskDiscard.to_string() << "\t";
        dbfile << std::endl;
    }

    for (const auto& e : lut5) {
        dbfile << e.first.size() << "\t" << e.first << "\t";
        const auto& attr = e.second;
        dbfile << attr.maskLeft.to_string() << "\t"
               << attr.maskRight.to_string() << "\t"
               << attr.leftFun << "\t"
               << attr.rightFun << "\t"
               << attr.combine << "\t"
               << attr.maskDiscard.to_string() << "\t";
        dbfile << std::endl;
    }

    for (const auto& e : lut6) {
        dbfile << e.first.size() << "\t" << e.first << "\t";
        const auto& attr = e.second;
        dbfile << attr.maskLeft.to_string() << "\t"
               << attr.maskRight.to_string() << "\t"
               << attr.leftFun << "\t"
               << attr.rightFun << "\t"
               << attr.combine << "\t"
               << attr.maskDiscard.to_string() << "\t";
        dbfile << std::endl;
    }

    sw.take();
    sw.report();
    return 0;
}
