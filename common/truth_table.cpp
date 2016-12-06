//
// Created by tripack on 16-11-18.
//

#include "truth_table.h"
#include "exception.h"
#include <iostream>
#include <cassert>

using namespace std;

TruthTable::TruthTable(int inputSize) {
    if(inputSize < 0)
        throw ExceptionTruthTableInvalidSize();
    this->inputSize = inputSize;
    for (int i = 0; i < inputSize; ++i) {
        name.push_back(std::string("in") + std::to_string(i));
    }
}


TruthTable::TruthTable(int inputSize, std::vector<int> &v) {
    if(inputSize < 0)
        throw ExceptionTruthTableInvalidSize();
    this->inputSize = inputSize;
    setByVector(v);
    for (int i = 0; i < inputSize; ++i) {
        name.push_back(std::string("in") + std::to_string(i));
    }
}

const BoolVariable &TruthTable::operator[](int i) const {
    return data.at(i);
}

BoolVariable &TruthTable::operator[](int i) {
    return data.at(i);
}

void TruthTable::setByVector(std::vector<int>& v) {
    if (this->size() != v.size()) {
        cerr << "TruthTable: "
                "Error during vector assignment, "
                "size mismatch." << endl;
        throw ExceptionInvalidVectorAssignment{};
    }
    this->data.clear();
    for (int i = 0; i < size(); ++i) {
        this->data.push_back(BoolVariable(v[i]));
    }
    return;
}

void TruthTable::setNameByVector(std::vector<std::string> &name) {
    for (int i = 0; i < name.size(); ++i) {
        this->name[i] = name[i];
        if (i == inputSize) break;
    }
}

std::string &TruthTable::getName(int i) {
    if (i > inputSize) assert(0);
    return this->name[i];
}

void TruthTable::print() {
    cout << "Inputs: " << endl;
    for (auto& n : this->name)
        cout << n << "\t";
    cout << endl << "Table: " << endl;
    for (auto& i : this->data)
        cout << (int)i << " ";
}
