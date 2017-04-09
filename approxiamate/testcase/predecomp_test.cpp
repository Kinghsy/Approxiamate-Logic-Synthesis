//
// Created by king on 17-3-31.
//

#include "gtest/gtest.h"
#include "../../util/precomp/common.h"
#include "../../util/precomp/do_lut.h"

#include <cinttypes>
#include <iostream>
#include <array>
#include <numeric>
#include <bitset>
#include <truth_table.h>
#include "../src/search_method_core.h"
#include "../src/search.h"
#include <fstream>

using std::vector;
using std::tuple;
using std::get;
using std::move;
using std::cout;
using std::endl;
using std::string;

/*
TEST(PreDecomp, TC1) {

    std::cout << std::endl;

    PRECOM_RESULT(3) lut3 = do3input();
    PRECOM_RESULT(4) lut4 = do4input(lut3);
    auto lut5 = do5input(lut3, lut4);
    auto lut6 = do6input(lut3, lut4, lut5);

    cout << "Total " << lut3.size() << " decomposable LUT3s." << endl;
    cout << "Total " << lut4.size() << " decomposable LUT4s." << endl;
    cout << "Total " << lut5.size() << " decomposable LUT5s." << endl;
    cout << "Total " << lut6.size() << " decomposable LUT6s" << endl;

    for (const auto& pair: lut3) {
        // cout << pair.first << endl;
    }

    ASSERT_EQ(1, 1);
}
*/

void bitArrange(int *arr, int n);

/*
TEST(PreDecomp, COMBINATION) {

    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << std::bitset<32>(255) << std::endl;
    std::cout << std::bitset<32>(bin2int({1,0,1,1,0,1,1})) << std::endl;
    ASSERT_EQ(1, 1);

}

TEST(PreDecomp, Some_Tool) {
    std::bitset<8> a("10110101");
    std::bitset<8> b(73);
    size_t count = a.count();
    cout << endl;
    cout << a << endl;
    a[0] = 0;
    a[1] = 1;
    cout << a << endl;
    cout << b << endl;
    cout << count << endl;
    size_t diffCount = (a^b).count();
    cout << diffCount << endl;
    cout << (1<<3) << endl;
    cout << endl;
}

TEST(PreDecomp, All_Cases_3_Input) {

    std::ofstream outFile("3_input_test.txt");
    int inputSize = 3;

    int worstCase1 = 0;
    int totalErr1 = 0;
    int worstCase2 = 0;
    int totalErr2 = 0;

    outFile << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "All tests for 3-input: " << std::endl;
    outFile << "------------------------" << std::endl;

    PRECOM_RESULT(3) lut3 = do3input();
    PRECOM_RESULT(4) lut4 = do4input(lut3);
    //auto lut5 = do5input(lut3, lut4);
    //auto lut6 = do6input(lut3, lut4, lut5);

    std::string fileName("To_Be_Deleted");

    //int* arr = new int( 1 << inputSize );
    int arr[8]={0};
    for (int i = 0; i < (1<<inputSize); ++i) { arr[i] = 0 ;}
    arr[(1<<inputSize) - 1] = -1;

    for (int i = 0; i < ( 1<< (1 << inputSize) ); ++i) {

        int j = (1 << inputSize) - 1;
        arr[j]++;
        while (arr[j] == 2) {
            arr[j] = 0;
            arr[--j] ++;
        }

        fun3 rec( bin2int(arr, 1 << inputSize) );
        int minDiff1 = 1 << inputSize;
        for (const auto& ite : lut3) {
            const auto& itePf = ite.first;
            size_t countDiff = (itePf ^ rec).count();
            minDiff1 = std::min((int)(countDiff), minDiff1);
        }

        worstCase1 = std::max(minDiff1, worstCase1);
        totalErr1 += minDiff1;

        std::vector<int > truthTabVec;
        for (int l = 0; l < (1<<inputSize); ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(inputSize, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < (1<<inputSize); ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }
        worstCase2 = std::max(minDiff2, worstCase2);
        totalErr2 += minDiff2;

        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            outFile << fun3( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        outFile << fun3( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    outFile << "worstCase for predecomp: " << worstCase1 << endl;
    outFile << "average error for predecomp: " << (double)(totalErr1) / ( 1<< (1 << inputSize) ) << endl;
    outFile << "worstCase for approximation: " << worstCase2 << endl;
    outFile << "average error for approximation: " << (double)(totalErr2) / ( 1<< (1 << inputSize) ) << endl;

    //delete[] arr;

    outFile.close();
    ASSERT_EQ(1, 1);
}

TEST(PreDecomp, All_Cases_4_Input) {

    std::ofstream outFile("4_input_test.txt");

    int inputSize = 4;

    int worstCase1 = 0;
    int totalErr1 = 0;
    int worstCase2 = 0;
    int totalErr2 = 0;

    outFile << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "Fail tests for 4-input: " << std::endl;
    outFile << "------------------------" << std::endl;

    PRECOM_RESULT(3) lut3 = do3input();
    PRECOM_RESULT(4) lut4 = do4input(lut3);
    //auto lut5 = do5input(lut3, lut4);
    //auto lut6 = do6input(lut3, lut4, lut5);

    std::string fileName("To_Be_Deleted");

    //int* arr = new int( 1 << inputSize );
    int arr[16]={0};
    for (int i = 0; i < (1<<inputSize); ++i) { arr[i] = 0 ;}
    arr[(1<<inputSize) - 1] = -1;

    for (int i = 0; i < ( 1<< (1 << inputSize) ); ++i) {

        int j = (1 << inputSize) - 1;
        arr[j]++;
        while (arr[j] == 2) {
            arr[j] = 0;
            arr[--j] ++;
        }

        fun4 rec( bin2int(arr, 1 << inputSize) );
        int minDiff1 = 1 << inputSize;
        for (const auto& ite : lut4) {
            const auto& itePf = ite.first;
            size_t countDiff = (itePf ^ rec).count();
            minDiff1 = std::min((int)(countDiff), minDiff1);
        }

        worstCase1 = std::max(worstCase1, minDiff1);
        totalErr1 += minDiff1;

        std::vector<int > truthTabVec;
        for (int l = 0; l < (1<<inputSize); ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(inputSize, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < (1<<inputSize); ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }

        worstCase2 = std::max(worstCase2, minDiff2);
        totalErr2 += minDiff2;

        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            outFile << fun4( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        outFile << fun4( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    outFile << "worstCase for predecomp: " << worstCase1 << endl;
    outFile << "average error for predecomp: " << (double)(totalErr1) / ( 1<< (1 << inputSize) ) << endl;

    outFile << "worstCase for approximation: " << worstCase2 << endl;
    outFile << "average error for approximation: " << (double)(totalErr2) / ( 1<< (1 << inputSize) ) << endl;

    //delete[] arr;

    outFile.close();
    ASSERT_EQ(1, 1);
}

TEST(PreDecomp, Random_Cases_5_Input) {

    std::ofstream outFile("5_input_test.txt");
    srand((unsigned)time(0));
    int inputSize = 5;

    int worstCase1 = 0;
    int totalErr1 = 0;
    int worstCase2 = 0;
    int totalErr2 = 0;

    outFile << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "Fail tests for 5-input: " << std::endl;
    outFile << "------------------------" << std::endl;

    PRECOM_RESULT(3) lut3 = do3input();
    PRECOM_RESULT(4) lut4 = do4input(lut3);
    auto lut5 = do5input(lut3, lut4);
    //auto lut6 = do6input(lut3, lut4, lut5);

    std::string fileName("To_Be_Deleted");

    //int* arr = new int( 1 << inputSize );
    int arr[32]={0};

    for (int i = 0; i < (1 << 16); ++i) {

        //cout << i << endl;

        int j = (1 << inputSize) - 1;

        bitArrange(arr, (1<<inputSize));

        fun5 rec( bin2int(arr, 1 << inputSize) );
        int minDiff1 = 1 << inputSize;
        for (const auto& ite : lut5) {
            const auto& itePf = ite.first;
            size_t countDiff = (itePf ^ rec).count();
            minDiff1 = std::min((int)(countDiff), minDiff1);
        }

        worstCase1 = std::max(worstCase1, minDiff1);
        totalErr1 += minDiff1;

        std::vector<int > truthTabVec;
        for (int l = 0; l < (1<<inputSize); ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(inputSize, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < (1<<inputSize); ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }

        worstCase2 = std::max(worstCase2, minDiff2);
        totalErr2 += minDiff2;

        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            outFile << i << " : " << fun5( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        outFile << i << " : " << fun5( bin2int(arr,1<<inputSize) ) << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    outFile << "worstCase for predecomp: " << worstCase1 << endl;
    outFile << "average error for predecomp: " << (double)(totalErr1) / ( 1<< 16 ) << endl;
    outFile << "worstCase for approximation: " << worstCase2 << endl;
    outFile << "average error for approximation: " << (double)(totalErr2) / ( 1<< 16 ) << endl;

    //delete[] arr;
    outFile.close();

    ASSERT_EQ(1, 1);
}
*/
TEST(PreDecomp, Random_Cases_6_Input) {

    std::ofstream outFile("6_input_test.txt");
    srand((unsigned)time(0));
    int inputSize = 6;

    int worstCase1 = 0;
    int totalErr1 = 0;
    int worstCase2 = 0;
    int totalErr2 = 0;

    outFile << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "------------------------" << std::endl;
    outFile << "Fail tests for 6-input: " << std::endl;
    outFile << "------------------------" << std::endl;

    PRECOM_RESULT(3) lut3 = do3input();
    PRECOM_RESULT(4) lut4 = do4input(lut3);
    auto lut5 = do5input(lut3, lut4);
    auto lut6 = do6input(lut3, lut4, lut5);

    std::string fileName("To_Be_Deleted");

    //int* arr = new int( 1 << inputSize );
    int arr[64]={0};

    for (int i = 0; i < (1 << 14); ++i) {

        //cout << i << endl;

        int j = (1 << inputSize) - 1;

        bitArrange(arr, (1<<inputSize));

        fun6 rec( bin2int(arr, 1 << inputSize) );
        for (int m = 0; m < (1<<inputSize); ++m) {
            rec[m] = arr[m];
        }
        //outFile << rec << endl;

        int minDiff1 = 1 << inputSize;
        for (const auto& ite : lut6) {
            const auto& itePf = ite.first;
            size_t countDiff = (itePf ^ rec).count();
            minDiff1 = std::min((int)(countDiff), minDiff1);
        }

        worstCase1 = std::max(worstCase1, minDiff1);
        totalErr1 += minDiff1;

        std::vector<int > truthTabVec;
        for (int l = 0; l < (1<<inputSize); ++l) {
            truthTabVec.push_back(arr[l]);
        }
        TruthTable initTruthTab(inputSize, truthTabVec);
        TruthTable finalTruthTab = writeApproxBlifFileByTruthTable(initTruthTab, fileName, FULL_SEARCH);
        int minDiff2 = 0;
        for (int k = 0; k < (1<<inputSize); ++k) {
            minDiff2 += ( int(initTruthTab[k]) != int(finalTruthTab[k]) ? 1 : 0);
        }

        worstCase2 = std::max(worstCase2, minDiff2);
        totalErr2 += minDiff2;

        if ( ( minDiff1 == 0 ) && ( minDiff2 == 0 ) ) continue;
        if ( ( minDiff1 == 0 ) || ( minDiff2 == 0 ) ) {
            outFile << i << " : " <<  rec << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
            continue;
        }
        if ( ( minDiff1 == minDiff2) ) continue;
        outFile << i << " : " <<  rec << " " << minDiff1 << " " << minDiff2 <<  " " << std::endl;
        continue;

    }

    outFile << "worstCase1 for predecomp: " << worstCase1 << endl;
    outFile << "average error for predecomp: " << (double)(totalErr1) / ( 1 << 14 ) << endl;

    outFile << "worstCase1 for approximation: " << worstCase2 << endl;
    outFile << "average error for approximation: " << (double)(totalErr2) / ( 1 << 14 ) << endl;

    //delete[] arr;
    outFile.close();

    ASSERT_EQ(1, 1);
}

void bitArrange(int* arr, int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 2;
    }
}