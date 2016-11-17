//
// Created by tripack on 16-11-18.
//

#include "truth_table.h"
#include "exception.h"
#include <iostream>

using namespace std;

TruthTable::TruthTable(int inputSize) {
    if(inputSize < 0)
        throw ExceptionTruthTableInvalidSize();
    this->inputSize = inputSize;
}

TruthTable::TruthTable(int inputSize, std::vector<int>&& v) {
    if(inputSize < 0)
        throw ExceptionTruthTableInvalidSize();
    this->inputSize = inputSize;
    setByVector(v);
}

TruthTable::TruthTable(int inputSize, std::vector<int> &v) {
    if(inputSize < 0)
        throw ExceptionTruthTableInvalidSize();
    this->inputSize = inputSize;
    setByVector(std::move(v));
}

const BoolVariable &TruthTable::operator[](int i) const {
    return data.at(i);
}

BoolVariable &TruthTable::operator[](int i) {
    return data.at(i);
}

void TruthTable::setByVector(std::vector<int>&& v) {
    int size = v.size();
    int nInput = this->inputSize;
    if (1 << nInput != size) {
        cerr << "TruthTable: "
                "Error during vector assignment, "
                "size mismatch." << endl;
        throw ExceptionInvalidVectorAssignment{};
    }
    for (int i = 0; i < size; ++i) {
        this->data[i] = v[i];
    }
    return;
}
