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

    vector<string> inputss;
    inputss.push_back("x2");
    inputss.push_back("x1");
    inputss.push_back("x3");
    string outss("cc");
    BoolFunction fun5(3, TTable("00111011"), inputss, outss);
    ASSERT_EQ(true, fun == fun5);
    cout << "pass 5" << endl;
    fun.display();

    cout << "reorder:" << endl;
    fun.display();
    inputs.pop_back(); inputs.push_back(string("x3"));
    cout << fun.reorder(inputs, output) << endl;
    fun.display();
    BoolFunction fun6(3, TTable("00111001"), inputss, outss);
    int q;
    cout << (q = fun ^ fun6) << endl;

    ASSERT_EQ(q, 1);
}

TEST(TEST_NEW_BOOL_FUNCTION, test_case_2) {

    vector<string> inputs1;
    inputs1.push_back("x1");
    inputs1.push_back("x2");
    string output1("c1");
    BoolFunction fun1(2, TTable("1110"), inputs1, output1);
    fun1.display();

    vector<string> inputs2;
    inputs2.push_back("x3");
    inputs2.push_back("x4");
    string output2("c2");
    BoolFunction fun2(2, TTable("1110"), inputs2, output2);
    fun2.display();

    BoolFunction fun3(1,TTable("01"), inputs1, output1);

    fun3 = combineBooleanFunction(fun1, fun2, AND_TABLE, string("out"));
    fun3.display();

    int g;
    cout << (g = fun3.getPortNum(string("x2"))) << endl;
    ASSERT_EQ(g, 1);

    string temp;
    cout << (temp = fun3.getPortName(2)) << endl;
    ASSERT_EQ(temp, string("x3"));

    cout << (temp = fun3.getOutPortName()) << endl;
    ASSERT_EQ(temp, string("out"));

}