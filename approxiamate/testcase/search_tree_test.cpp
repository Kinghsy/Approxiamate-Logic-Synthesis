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

/*
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
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_1_BFS) {
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
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
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
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_2_BFS) {
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
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
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
    //delete truthTable; ( P3 [ 1 0 ]
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_3_BFS) {
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
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_4) {
    int portName[]={1,1,1};
    int portSize=3;
    int truthTable[]={1, 0, 1, 1, 0, 0, 0, 1};
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
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_4_BFS) {
    int portName[]={1,1,1};
    int portSize=3;
    int truthTable[]={1, 0, 1, 1, 0, 0, 0, 1};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_5) {
    int portName[]={1,1,1,1};
    int portSize=4;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0,
                      1, 0, 1, 0, 0, 0, 0, 0};
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
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}

TEST(TEST_SEARCH_TREE, TC_5_BFS) {
    int portName[]={1,1,1,1};
    int portSize=4;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0,
                      1, 0, 1, 0, 0, 0, 0, 0};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}*/
/*
TEST(TEST_SEARCH_TREE, TC_6) {
    ActivedModeApplied = FULL_SEARCH;
    int portName[]={1,1,1,1,1};
    int portSize=5;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0,
                      1, 0, 1, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 1,
                      0, 0, 1, 0, 0, 1, 0, 0};
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
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}*/

/*
TEST(TEST_SEARCH_TREE, TC_6_BFS) {
    int portName[]={1,1,1,1,1};
    int portSize=5;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0,
                      1, 0, 1, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 1,
                      0, 0, 1, 0, 0, 1, 0, 0};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace_BFS())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}
*/
/*
TEST(TEST_SEARCH_TREE, TC_7_random_cases_for_5_inputs) {
    int portName[5]={1,1,1,1,1};
    int portSize=5;
    int casesNo = 0;
    srand(time(nullptr));
    while (casesNo < (1<<5)) {
        casesNo++;
        int truthTable[32];
        cout << "---------------------------------\n";
        cout << "case " << casesNo << endl;
        for (int i = 0; i < 32; ++i) {
            truthTable[i] = rand() % 2;
        }

        BooleanFunction initBF(portName, portSize, truthTable);
        SearchTree whole(initBF);
        SearchSpacePtr ssPtr;
        whole.getRootSpace()->printSearchSpace();
        while ((ssPtr = whole.getNextSearchSpace_Full()) != nullptr) {
        }
        ssPtr = whole.getBestSpace();
        ssPtr->printSearchSpace();
        cout << "---------------------------------\n";
    }

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}*/

TEST(TEST_SEARCH_TREE, TC_1_XOR_IGNORE) {
    ActivedModeApplied = FULL_SEARCH | XOR_IGNORE ;
    int portName[]={1,1,1,1,1};
    int portSize=5;
    int truthTable[]={0, 0, 0, 1, 1, 0, 1, 0,
                      1, 0, 1, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 1,
                      0, 0, 1, 0, 0, 1, 0, 0};
    BooleanFunction initBF(portName, portSize, truthTable);
    //delete portName;
    //delete truthTable;
    SearchTree whole(initBF);
    SearchSpacePtr ssPtr;
    cout << "---------------------------------\n";
    whole.getRootSpace()->printSearchSpace();
    while ((ssPtr=whole.getNextSearchSpace_Full())!= nullptr) {
        ssPtr->printSearchSpace();
    }
    cout << "---------------------------------\n";
    ssPtr=whole.getBestSpace();
    ssPtr->printSearchSpace();
    cout << "---------------------------------\n";

    cout << "above is result, please check. (y/n)" << endl;
    char ch;
    cin >> ch;
    ASSERT_EQ(ch,'y');
}


