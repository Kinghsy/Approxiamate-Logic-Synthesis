//
// Created by king on 17-4-14.
//

#include <gtest/gtest.h>
#include <vector>

#include "../src/kmap.h"
#include "../src/bool_function.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;


TEST(TEST_NEW_KMAP, test_case1) {

    vector<string> inputs;
    inputs.push_back("x1");
    inputs.push_back("x2");
    inputs.push_back("x3");
    string output("cc");
    BoolFunction fun(3, TTable("01011101"), inputs, output);

    vector<string> part1;
    part1.push_back(string("x2"));
    vector<string> part2;
    part2.push_back(string("x1"));
    part2.push_back(string("x2"));

    Kmap map(fun, part1, part2);
    map.display();

    ASSERT_EQ(1, 1);
}