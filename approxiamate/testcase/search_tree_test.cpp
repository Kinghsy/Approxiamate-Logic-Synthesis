//
// Created by king on 16-12-16.
//
#include <iostream>
#include <cstdlib>

#include "../src/search.h"
#include "../src/conts.h"
#include "../src/boolean_function.h"

#include <gtest/gtest.h>

using namespace std;

TEST(TEST_SEARCH_TREE, TC_1) {
    int portName[]={1,1};
    int portSize=2;
    int truthTable[]={0, 0, 0, 0};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    SearchSpacePtr ssPtr;
    while ((ssPtr=whole.getNextSearchSpace())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}



TEST(TEST_SEARCH_TREE, TC_2) {
    int portName[]={1,1};
    int portSize=2;
    int truthTable[]={0, 0, 0, 1};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_3) {
    int portName[]={1,1,1};
    int portSize=3;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}
