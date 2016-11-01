//
// Created by patri on 2016/10/13.
//

#include <gtest/gtest.h>

#include "generator.h"
#include "dataset.h"

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

TEST(DATASET, CONTENT) {
#define data_ d2
    StaticSequenceGenerator<int> g(data_);
    Dataset<int> dataset(g);
    const int *raw = dataset.getData();
    int size = dataset.getSize();
    ASSERT_EQ(size, g.getSize());
    for (int i = 0; i < size ; ++i) {
        ASSERT_EQ(data_[i], raw[i]);
    }
    int* copied = dataset.getCopied();
    for (int i = 0; i < size ; ++i) {
        ASSERT_EQ(data_[i], copied[i]);
    }
#undef data_
}

TEST(SORTABLEDATASET, SORT) {
#define data_ d5
    StaticSequenceGenerator<int> g(data_);
    SortableDataset<int> dataset(g);
    const int *raw = dataset.getData();
    int size = dataset.getSize();
    ASSERT_EQ(size, g.getSize());
    for (int i = 0; i < size ; ++i) {
        ASSERT_EQ(data_[i], raw[i]);
    }
    int* copied = dataset.getCopied();
    for (int i = 0; i < size ; ++i) {
        ASSERT_EQ(data_[i], copied[i]);
    }
    const int* sorted = dataset.getSorted();
    cout << endl;
    for (int i = 0; i < size ; ++i) {
        cout << sorted[i] << " ";
    }
    cout << endl;
#undef data_
}