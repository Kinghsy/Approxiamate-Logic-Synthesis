//
// Created by king on 16-12-17.
//

#include <iostream>

#include "../src/search.h"
#include "../src/conts.h"
#include "../src/boolean_function.h"
#include "../src/search_method_core.h"

#include "../../common/truth_table.h"

#include <gtest/gtest.h>

TEST(SEARCH_METHOD_CORE_TEST, TC_1) {

    int inputSize=3;
    int truTab[]={1, 0, 1, 1, 0, 0, 0, 1};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);

    TruthTable finalTruthTable(
            calculApproxTruthTable(initTruthTable)
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y','y');

}

TEST(SEARCH_METHOD_CORE_TEST, TC_2) {

    int inputSize=4;
    int truTab[]={1, 0, 1, 1, 0, 0, 0, 1,
                  0, 1, 0, 0, 0, 0, 1, 0};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);

    TruthTable finalTruthTable(
            calculApproxTruthTable(initTruthTable)
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y', 'y');

}

TEST(SEARCH_METHOD_CORE_TEST, TC_3) {

    int inputSize=5;
    int truTab[]={1, 0, 1, 1, 0, 0, 0, 1,
                  0, 1, 0, 0, 0, 0, 1, 0,
                  1, 0, 0, 1, 0, 1, 0, 1,
                  0, 1, 0, 0, 1, 0, 1, 0};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);

    TruthTable finalTruthTable(
            calculApproxTruthTable(initTruthTable)
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y', 'y');

}

TEST(SEARCH_METHOD_CORE_TEST, TC_4) {

    int inputSize=3;
    int truTab[]={0, 0, 0, 0, 0, 0, 0, 0};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);
    initTruthTable.getName(0)="Blink.";
    initTruthTable.getName(1)="Hao";
    initTruthTable.getName(2)="Ferrari_430";
    initTruthTable.outName="w33";

    for (int i = 0; i < initTruthTable.numInput(); ++i) {
        std::cout << " " << initTruthTable.getName(i) << " ";
    }
    std::cout << std::endl;
    std::cout << initTruthTable.outName << std::endl;

    TruthTable finalTruthTable(
            writeApproxBlifFileByTruthTable(initTruthTable, "testing1.blif")
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y', 'y');

}

TEST(SEARCH_METHOD_CORE_TEST, TC_5) {

    int inputSize=5;
    int truTab[]={1, 0, 1, 1, 0, 0, 0, 1,
                  0, 1, 0, 0, 0, 0, 1, 0,
                  1, 0, 0, 1, 0, 1, 0, 1,
                  0, 1, 0, 0, 1, 0, 1, 0};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);
    initTruthTable.getName(0)="p0";
    initTruthTable.getName(1)="p1";
    initTruthTable.getName(2)="p2";
    initTruthTable.getName(3)="p3";
    initTruthTable.getName(4)="p77";
    initTruthTable.outName="Out33";

    for (int i = 0; i < initTruthTable.numInput(); ++i) {
        std::cout << " " << initTruthTable.getName(i) << " ";
    }
    std::cout << std::endl;
    std::cout << initTruthTable.outName << std::endl;

    TruthTable finalTruthTable(
            writeApproxBlifFileByTruthTable(initTruthTable, "testing2.blif")
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y', 'y');

}

TEST(SEARCH_METHOD_CORE_TEST, TC_6) {

    int inputSize=3;
    int truTab[]={1, 1, 1, 1, 1, 1, 1, 1};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);
    initTruthTable.getName(0)="Sakata";
    initTruthTable.getName(1)="YYF";
    initTruthTable.getName(2)="820";
    initTruthTable.outName="Zhou";

    for (int i = 0; i < initTruthTable.numInput(); ++i) {
        std::cout << " " << initTruthTable.getName(i) << " ";
    }
    std::cout << std::endl;
    std::cout << initTruthTable.outName << std::endl;

    TruthTable finalTruthTable(
            writeApproxBlifFileByTruthTable(initTruthTable, "testing3.blif")
    );
    std::cout << "\n";
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    initTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    finalTruthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";

    std::cout << "above is result, please check. (y/n)" << std::endl;
    char ch;
    //std::cin >> ch;
    ASSERT_EQ('y', 'y');

}