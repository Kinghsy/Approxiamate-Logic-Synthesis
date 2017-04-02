//
// Created by king on 17-3-31.
//

#include "gtest/gtest.h"
#include "../../util/precomp/interface.h"
#include "../../util/precomp/common.h"

#include <cinttypes>
#include <iostream>
#include <array>
#include <numeric>
#include <bitset>
#include <truth_table.h>
#include "../src/search_method_core.h"
#include "../src/search.h"

using std::vector;
using std::tuple;
using std::get;
using std::move;

/*
TEST(PreDecomp, TC1) {

    std::cout << std::endl;

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
}*/

TEST(PreDecomp, COMBINATION) {

    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << std::bitset<32>(255) << std::endl;
    std::cout << std::bitset<32>(bin2int({1,0,1,1,0,1,1})) << std::endl;
    ASSERT_EQ(1, 1);

}

TEST(PreDecomp, Random_Case_3_Input) {

    std::cout << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "Fail tests for 3-input: " << std::endl;
    std::cout << "------------------------" << std::endl;

    std::vector<uint8_t > lut3;
    std::vector<uint16_t > lut4;
    std::vector<uint32_t > lut5;
    std::tie(lut3, lut4, lut5) = preDecomp();

    std::string fileName("To_Be_Deleted");

    int arr[8] = {0};

    for (int i = 0; i < (1<<8); ++i) {

        int j = 7;
        arr[j]++;
        while (arr[j] == 2) {
            arr[j] = 0;
            arr[--j] ++;
        }

        uint8_t rec = bin2int(arr, 8);
        int minDiff1 = 1<<8;
        for (std::vector<uint8_t >::iterator ite = lut3.begin(); ite != lut3.end() ; ++ite) {
            uint8_t diff = (*ite) ^ rec;
            int countDiff = 0;
            while ( diff > 0 ) {
                countDiff += diff % 2;
                diff = diff >> 1;
            }
            minDiff1 = std::min(countDiff, minDiff1);
        }

        std::vector<int > truthTabVec;
        for (int l = 0; l < 8; ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(3, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < 8; ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }


        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            std::cout << std::bitset<8>( bin2int(arr,8) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        std::cout << std::bitset<8>( bin2int(arr,8) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    ASSERT_EQ(1, 1);
}

TEST(PreDecomp, Random_Case_4_Input) {

    std::cout << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "Fail tests for 4-input: " << std::endl;
    std::cout << "------------------------" << std::endl;

    std::vector<uint8_t > lut3;
    std::vector<uint16_t > lut4;
    std::vector<uint32_t > lut5;
    std::tie(lut3, lut4, lut5) = preDecomp();

    std::string fileName("To_Be_Deleted");

    int arr[16] = {0};

    for (int i = 0; i < (1<<16); ++i) {

        int j = 15;
        arr[j]++;
        while (arr[j] == 2) {
            arr[j] = 0;
            arr[--j] ++;
        }

        uint16_t rec = bin2int(arr, 16);
        int minDiff1 = 1<<16;
        for (std::vector<uint16_t >::iterator ite = lut4.begin(); ite != lut4.end() ; ++ite) {
            uint16_t diff = (*ite) ^ rec;
            int countDiff = 0;
            while ( diff > 0 ) {
                countDiff += diff % 2;
                diff = diff >> 1;
            }
            minDiff1 = std::min(countDiff, minDiff1);
        }

        std::vector<int > truthTabVec;
        for (int l = 0; l < 16; ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(4, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < 16; ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }


        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            std::cout << std::bitset<16>( bin2int(arr,16) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        std::cout << std::bitset<16>( bin2int(arr,16) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    ASSERT_EQ(1, 1);
}