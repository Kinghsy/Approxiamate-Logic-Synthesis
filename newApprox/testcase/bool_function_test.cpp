//
// Created by king on 17-4-14.
//

#include <gtest/gtest.h>
#include <vector>

#include "../src/bool_function.h"
#include "../../common/ttable.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;


TEST(TEST_NEW_BOOL_FUNCTION, test_case_1) {
    vector<string> inputs;
    inputs.push_back("x1");
    inputs.push_back("x2");
    inputs.push_back("x3");
    string output("cc");
    BoolFunction fun(3, TTable("01011101"), inputs, output);
    fun.display();
    BoolFunction fun2(1, NORMAL_1_INPUT, vector<NodeName>(), string(""));
    fun2 = fun;
    fun2.display();
    cout << "pass 1" << endl;
    ASSERT_EQ(true, fun == fun2);
    cout << "pass 2" << endl;
    BoolFunction fun3(3, TTable("01011100"), inputs, output);
    ASSERT_EQ(false, fun == fun3);
    cout << "pass 3" << endl;
    inputs.pop_back();
    inputs.push_back("x4");
    BoolFunction fun4(3, TTable("01011101"), inputs, output);
    ASSERT_EQ(false, fun == fun4);
    cout << "pass 4" << endl;
    ASSERT_EQ(1, 1);
}