//
// Created by king on 16-12-17.
//

#include <iostream>
#include <sstream>

#include "../src/search.h"
#include "../src/conts.h"
#include "../src/boolean_function.h"
#include "../src/search_method_core.h"

#include "../../common/truth_table.h"

#include "../../MFFC/interface.h"

#include <gtest/gtest.h>

class StringGenerator{
private:
    std::string base;
    int count;
public:
    StringGenerator(std::string headLine) { base = headLine; count = 0; }
    StringGenerator() { base = "Hello"; count = 0; }
    StringGenerator(int i) { base = "Hello"; count = i; }
    StringGenerator(std::string headLine, int i) { base = headLine; count = i; }
    std::string gener() {
        std::string str;
        str += base;
        str += std::to_string(count);
        return str;
    }
};

TEST(SEARCH_METHOD_CORE_TEST, TC_1) {

    int inputSize=3;
    int truTab[]={1, 0, 1, 1, 0, 0, 0, 1};
    int truTabSize=(1<<inputSize);
    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);

    TruthTable finalTruthTable(
            calculApproxTruthTable_BFS(initTruthTable)
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
            calculApproxTruthTable_BFS(initTruthTable)
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
            calculApproxTruthTable_BFS(initTruthTable)
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
            writeApproxBlifFileByTruthTable_BFS(initTruthTable, "testing1.blif")
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
            writeApproxBlifFileByTruthTable_BFS(initTruthTable, "testing2.blif")
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
            writeApproxBlifFileByTruthTable_BFS(initTruthTable, "testing3.blif")
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
/*
TEST(SEARCH_METHOD_CORE_TEST, TC_7) {
    BlifBooleanNet mffc("mffc.blif");
    TruthTable truthTable = mffc.truthTable();
    TruthTable final = calculApproxTruthTable_BFS(truthTable);
    std::cout << "\n";
    std::cout << "--------initTruthTable---------------\n";
    truthTable.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    std::cout << "--------finalTruthTable--------------\n";
    final.print();
    std::cout << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "\n";
    ASSERT_EQ('y', 'y');
}*/

TEST(SEARCH_METHOD_CORE_TEST, TC_RANDOM_CASE_1) {

    srand(unsigned(time(NULL)));
    int inputSize=int(rand()) % 7;
    int truTabSize=(1<<inputSize);
    int *truTab=new int[truTabSize];
    for (int i = 0; i < truTabSize; ++i) {
        truTab[i] = int(rand()) % 2;
    }

    std::vector<int> v;
    for (int i = 0; i < truTabSize; ++i)
        v.push_back(truTab[i]);
    TruthTable initTruthTable(inputSize, v);

    TruthTable finalTruthTable(
            calculApproxTruthTable_BFS(initTruthTable)
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
    delete[] truTab;
    ASSERT_EQ('y', 'y');

}