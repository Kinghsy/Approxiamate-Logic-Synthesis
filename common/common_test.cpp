//
// Created by patri on 2016/10/13.
//

#include <gtest/gtest.h>

#include <iostream>
#include <common.h>

using namespace std;

TEST(DBITSET, BASIC) {

    ASSERT_EQ(vec2bitset<size_t>({0, 0, 1, 0}),
              DBitset(string("0100")));

    ASSERT_EQ(DBitset(string("01011100")), DBitset(8, 0x5Cul));
    ASSERT_EQ(DBitset(string("01011100"))[2], 1);
    ASSERT_EQ(DBitset(string("01011100"))[4], 1);
    ASSERT_EQ(DBitset(string("01011100"))[0], 0);
    ASSERT_EQ(DBitset(string("01011100"))[7], 0);
}

TEST(BIT_MANIP, BASIC) {
    ASSERT_EQ(getLSB(DBitset(std::string("1"))), 0);
    ASSERT_EQ(getLSB(DBitset(std::string("1000"))), 3);
    ASSERT_EQ(getLSB(DBitset(std::string("1010"))), 1);

    ASSERT_EQ(bitset2Str(ones(10, 2)), "0000000011");
    ASSERT_EQ(bitset2Str(ones(10, 0)), "0000000000");
    ASSERT_EQ(bitset2Str(ones(10, 10)), "1111111111");

    ASSERT_EQ(
            fillWith(16, DBitset(string("0010010001110110"))),
            DBitset(string("0010010001110110")));
    ASSERT_EQ(
            fillWith(8, DBitset(string("0010010001110110"))),
            DBitset(string("0111011001110110")));
    ASSERT_EQ(
            fillWith(3, DBitset(string("0010010001110110"))),
            DBitset(string("0110110110110110")));
    ASSERT_EQ(
            fillWith(1, DBitset(string("0010010001110110"))),
            DBitset(string("0000000000000000")));

    ASSERT_EQ(catBitset(DBitset(string("10100")), DBitset(string("10110"))),
              DBitset(string("1011010100")));

    ASSERT_EQ(DBitset(string("10100")) + DBitset(string("10110")),
              DBitset(string("1011010100")));

}

TEST(BIT_MANIP, EXTRACT) {
    string rfun = "010010101101111";
                      //1110000110;
    ASSERT_EQ(
            extract(DBitset(rfun), {0, 2, 4, 6, 8}),
            DBitset(string("11011"))
    );
    ASSERT_EQ(
            extract(DBitset(rfun), DBitset(string("101010101"))),
            DBitset(string("11011"))
    );
    ASSERT_EQ(
            extract(DBitset(rfun), {1, 2, 7, 8, 9}),
            DBitset(string("01011"))
    );
    ASSERT_EQ(
            extract(DBitset(rfun), DBitset(string("1110000110"))),
            DBitset(string("01011"))
    );
}

TEST(BIT_MANIP, BREAKDOWN) {
    const string rfun = "01101101110010011010010101101111";
    auto fun = DBitset(string(rfun.rbegin(), rfun.rend()));
    //std::cout << fun << std::endl;
    std::vector<DBitset> bd
            = breakdown(fun,
                        DBitset(string("10010")), // ROW
                        DBitset(string("01101"))); // COL
    ASSERT_EQ(bd[std::bitset<3>("000").to_ulong()], DBitset(string("1110")));
    ASSERT_EQ(bd[std::bitset<3>("001").to_ulong()], DBitset(string("0001")));
    ASSERT_EQ(bd[std::bitset<3>("010").to_ulong()], DBitset(string("0001")));
    ASSERT_EQ(bd[std::bitset<3>("011").to_ulong()], DBitset(string("1111")));
    ASSERT_EQ(bd[std::bitset<3>("100").to_ulong()], DBitset(string("1001")));
    ASSERT_EQ(bd[std::bitset<3>("101").to_ulong()], DBitset(string("0101")));
    ASSERT_EQ(bd[std::bitset<3>("110").to_ulong()], DBitset(string("1101")));
    ASSERT_EQ(bd[std::bitset<3>("111").to_ulong()], DBitset(string("1110")));
}

TEST(TTABLE, BASIC) {
    TTable fun("10110010");
    ASSERT_EQ(fun.nInputs(), 3);
    ASSERT_EQ(fun.toString(), "10110010");
    ASSERT_EQ((TTable("10110010").count()), 4);
    ASSERT_EQ(TTable(DBitset(string("10110010"))), TTable("10110010"));
    ASSERT_EQ(TTable(0xB2ull, 3), TTable("10110010"));
    ASSERT_EQ(fun[vec2bitset<size_t>({0, 0, 0})], 0);
    ASSERT_EQ(fun[vec2bitset<size_t>({0, 0, 1})], 1);
    ASSERT_EQ(fun[vec2bitset<size_t>({0, 1, 0})], 0);
    ASSERT_EQ(fun[vec2bitset<size_t>({0, 1, 1})], 0);
    ASSERT_EQ(fun[vec2bitset<size_t>({1, 0, 0})], 1);
    ASSERT_EQ(fun[vec2bitset<size_t>({1, 0, 1})], 1);
    ASSERT_EQ(fun[vec2bitset<size_t>({1, 1, 0})], 0);
    ASSERT_EQ(fun[vec2bitset<size_t>({1, 1, 1})], 1);

    ASSERT_EQ(fun[0], 0);
    ASSERT_EQ(fun[1], 1);
    ASSERT_EQ(fun[2], 0);
    ASSERT_EQ(fun[3], 0);
    ASSERT_EQ(fun[4], 1);
    ASSERT_EQ(fun[5], 1);
    ASSERT_EQ(fun[6], 0);
    ASSERT_EQ(fun[7], 1);

    ASSERT_EQ(fun[DBitset(string("000"))], 0);
    ASSERT_EQ(fun[DBitset(string("001"))], 1);
    ASSERT_EQ(fun[DBitset(string("010"))], 0);
    ASSERT_EQ(fun[DBitset(string("011"))], 0);
    ASSERT_EQ(fun[DBitset(string("100"))], 1);
    ASSERT_EQ(fun[DBitset(string("101"))], 1);
    ASSERT_EQ(fun[DBitset(string("110"))], 0);
    ASSERT_EQ(fun[DBitset(string("111"))], 1);

}


TEST(TTABLE, COMBINE) {
    TTable fun1("10110010");
    TTable fun2("1101");

    auto combinedTable =
            combineTruthTable(fun1, fun2,
                              DBitset(string("00111")),
                              DBitset(string("11000")), OR_TABLE);

    ASSERT_EQ(combinedTable.toString(),
              "11111111111111111011001011111111");

    combinedTable =
            combineTruthTable(fun1, fun2,
                              DBitset(string("00111")),
                              DBitset(string("11000")), AND_TABLE);

    ASSERT_EQ(combinedTable.toString(),
              "10110010101100100000000010110010");

    combinedTable =
            combineTruthTable(fun1, fun2,
                              DBitset(string("00111")),
                              DBitset(string("11000")), XOR_TABLE);

    ASSERT_EQ(combinedTable.toString(),
              "01001101010011011011001001001101");
}
