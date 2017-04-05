//
// Created by patri on 2016/10/13.
//

#include <gtest/gtest.h>

#include "generator.h"
#include "OrderedQueue.h"

#include <iostream>

using namespace std;

static const int seq1[] = {1, 2, 3, 4, 5, 6, 7, 8};
static const int d2[] = {-47, -21, -33, -18, -25, 38, -25, 42, 37, 43,
                         40, -10, 21, -36, -27, -46, -17, 8, 34, 45,};
static const int d3[] = {17, 24, -48, 40, 44, 10, 23, -33, -7, -11,
                         -14, -4, -50, 4, 28};
static const int d4[] = {6, 5, 4, 3, 2, 1};
static const int d5[] = {1};


TEST(GENERATOR, INT_SEQ) {
    StaticSequenceGenerator<int> g(seq1);
    StaticSequenceGenerator<int> g2(seq1);
    for (int i = 0; i < 8; ++i) {
        ASSERT_EQ(g.generate(), i + 1);
        ASSERT_EQ(g2.generate(), i + 1);
    }
}


TEST(OrderedQueue, TestCase1) {
    int data[]={1,6,2,4,5,3,9,7,8};
    int size = 9;
    OrderedQueue<int > q;
    for (int i = 0; i < size; ++i) {
        q.push(data[i]);
    }
    for (int i = 0; i < size; ++i) {
        ASSERT_EQ(i+1, q.val_front());
        q.pop_front();
    }
}

typedef int* intPtr;
class IntPtrWhole {
public:
    intPtr ip;
    friend bool operator>(const IntPtrWhole &a, const IntPtrWhole &b) {
        return (*(a.ip) < *(b.ip));
    }
    friend bool operator<(const IntPtrWhole &a, const IntPtrWhole &b) {
        return (*(a.ip) > *(b.ip));
    }
    void operator=(const IntPtrWhole &a) {
        ip = a.ip;
    }
    IntPtrWhole() {}
    IntPtrWhole(const intPtr& intPointer) {
        ip = intPointer;
    }
    int val() { return *ip; }
};

TEST(OrderedQueue, TestCase2) {
    int data[]={1,6,2,4,5,3,9,7,8};
    int size = 9;
    OrderedQueue<IntPtrWhole > q;
    for (int i = 0; i < size; ++i) {
        q.push(IntPtrWhole(&(data[i])));
    }
    for (int i = size; i > 0; i--) {
        ASSERT_EQ(i, q.val_front().val());
        q.pop_front();
    }
}
