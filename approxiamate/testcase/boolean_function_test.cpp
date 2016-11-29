//
// Created by king on 16-11-8.
//
#include <iostream>
#include <cstdlib>
#include <memory>

#include "../src/boolean_function.h"
#include "../src/conts.h"

#include "gtest/gtest.h"

using namespace std;

/*int main() {

    int portName[]={1,0,1,0,1,1};
    int portSize=6;
    int truthTable[]={0,1,0,0, 1,0,1,0, 1,1,1,0, 1,0,1,0};

    unique_ptr<BooleanFunction> bf(new BooleanFunction(portName, portSize, truthTable));
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    int **map1=new int*[2];
    int map10[8]={0, 0, 1, 1, 1, 1, 1, 1};
    int map11[8]={1, 0, 0, 0, 1, 0, 0, 0};
    map1[0]=map10;
    map1[1]=map11;
    const int hi1=2, wi1=8;
    KMap kmap1_corr(hi1, wi1, map1);
    kmap1_corr.display();
    delete[] map1;

    unique_ptr<KMap> kmap1=std::move(bf->getKMap(part1, part2));
    kmap1->display();

    TEST(TEST_KMAP, TEST_CASE_1) {
        ASSERT_EQ()
    }


    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> result = bf->divide(part1, part2);

    unique_ptr<BooleanFunction> p1=move(get<0>(result));
    unique_ptr<BooleanFunction> p2=move(get<1>(result));
    int error=get<2>(result);
    int oper=get<3>(result);

    cout << p1->toString() << endl;
    cout << p2->toString() << endl;
    cout << error << endl;
    cout << oper << endl;


    cout << error << endl;


}*/

TEST(TEST_KMAP, TEST_CASE_1) {
    int portName[]={1,0,1,0,1,1};
    int portSize=6;
    int truthTable[]={0,1,0,0, 1,0,1,0, 1,1,1,0, 1,0,1,0};

    unique_ptr<BooleanFunction> bf(new BooleanFunction(portName, portSize, truthTable));
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    int **map1=new int*[2];
    int map10[8]={0, 0, 1, 1, 1, 1, 1, 1};
    int map11[8]={1, 0, 0, 0, 1, 0, 0, 0};
    map1[0]=map10;
    map1[1]=map11;
    const int hi1=2, wi1=8;
    KMap kmap1_corr(hi1, wi1, map1);
    kmap1_corr.display();
    delete[] map1;

    unique_ptr<KMap> kmap1=std::move(bf->getKMap(part1, part2));
    kmap1->display();
    ASSERT_EQ((kmap1_corr == (*kmap1)), true);

}

TEST(TEST_DEVIDE, TEST_CASE_1) {
    int portName[]={1,0,1,0,1,1};
    int portSize=6;
    int truthTable[]={0,1,0,0, 1,0,1,0, 1,1,1,0, 1,0,1,0};

    unique_ptr<BooleanFunction> bf(new BooleanFunction(portName, portSize, truthTable));
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};

    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> result= bf->divide(part1, part2);

    unique_ptr<BooleanFunction> p1=move(get<0>(result));
    unique_ptr<BooleanFunction> p2=move(get<1>(result));
    int error=get<2>(result);
    int oper=get<3>(result);

    int resultPart2[]={0,0,1,1,1,1,1,1};
    BooleanFunction result2(part2, 6, resultPart2);
    int resultPart1[]={1,0};
    BooleanFunction result1(part1, 6, resultPart1);
    int resultErr=2;
    int resultOper=OPERATION_AND;

    ASSERT_EQ( (error==resultErr) && (oper==resultOper) && (*p1 == result1) && (*p2 == result2) , true);

}

TEST(TEST_COMBINE, TEST_CASE_1) {
    int truthTable1[]={1,0};
    int truthTable2[]={0,0,1,1,1,1,1,1};
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    BooleanFunction p1(part1, 6, truthTable1);
    BooleanFunction p2(part2, 6, truthTable2);

    unique_ptr<BooleanFunction> resAnd=move(p1.combine(p2, OPERATION_AND));

    int finalPart[]={1,0,1,0,1,1};
    int resCorrect[]={0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0};
    BooleanFunction resBFCorrect(finalPart, 6, resCorrect);

    ASSERT_EQ( (resBFCorrect == *resAnd), true );
}

TEST(TEST_COMBINE, TEST_CASE_2) {
    int truthTable1[]={1,0};
    int truthTable2[]={0,0,1,1,1,1,1,1};
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    BooleanFunction p1(part1, 6, truthTable1);
    BooleanFunction p2(part2, 6, truthTable2);

    unique_ptr<BooleanFunction> resOr=move(p1.combine(p2, OPERATION_OR));

    int finalPart[]={1,0,1,0,1,1};
    int resCorrect[]={1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1};
    BooleanFunction resBFCorrect(finalPart, 6, resCorrect);

    ASSERT_EQ( (resBFCorrect == *resOr), true );
}

TEST(TEST_COMBINE, TEST_CASE_3) {
    int truthTable1[]={1,0};
    int truthTable2[]={0,0,1,1,1,1,1,1};
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    BooleanFunction p1(part1, 6, truthTable1);
    BooleanFunction p2(part2, 6, truthTable2);

    unique_ptr<BooleanFunction> resXor=move(p1.combine(p2, OPERATION_XOR));

    int finalPart[]={1,0,1,0,1,1};
    int resCorrect[]={1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1};
    BooleanFunction resBFCorrect(finalPart, 6, resCorrect);

    ASSERT_EQ( (resBFCorrect == *resXor), true );
}

TEST(TEST_COMBINE, TEST_CASE_4) {
    int truthTable1[]={1,0};
    int truthTable2[]={0,0,1,1,1,1,1,1};
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    BooleanFunction p1(part1, 6, truthTable1);
    BooleanFunction p2(part2, 6, truthTable2);

    unique_ptr<BooleanFunction> resDrop=move(p1.combine(p2, OPERATION_DROP));

    int finalPart[]={1,0,1,0,1,1};
    int resCorrect[]={1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
    BooleanFunction resBFCorrect(finalPart, 6, resCorrect);

    ASSERT_EQ( (resBFCorrect == *resDrop), true );
}