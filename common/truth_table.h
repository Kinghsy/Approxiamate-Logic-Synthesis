//
// Created by tripack on 16-11-18.
//

#ifndef VE490_TRUTH_TABLE_H
#define VE490_TRUTH_TABLE_H

#include <vector>
#include <string>

class BoolVariable {
public:
    BoolVariable() : state(BoolFALSE) {};
    BoolVariable(const BoolVariable& b) = default;
    BoolVariable(BoolVariable&& b) = default;
    explicit BoolVariable(const bool& b) { copy(b); };
    explicit BoolVariable(const int& i) { copy(i); }

    BoolVariable& operator=(const BoolVariable& b) = default;
    BoolVariable& operator=(BoolVariable&& b) = default;
    BoolVariable& operator=(const bool& b) {
        return copy(b);
    }
    BoolVariable& operator=(const int& i) {
        return copy(i);
    }

    explicit operator bool() const {
        if (state == BoolTRUE) return true;
    }
    explicit operator int() const {
        if (state == BoolTRUE) return 1;
        if (state == DC) return -1;
        return 0;
    }

    explicit operator const char*() const {
        if (state == BoolTRUE) return "true";
        if (state == DC) return "dont_care";
        return "false";
    }

    std::string toString() const {
        if (state == BoolTRUE) return "true";
        if (state == DC) return "dont_care";
        return "false";
    }

private:
    BoolVariable& copy(const bool& b) {
        if (b) {state = BoolTRUE; return *this;}
        state = BoolFALSE;
    }

    BoolVariable& copy(const int& i) {
        if (i) {state = BoolTRUE; return * this;}
        state = BoolFALSE;
    }

    enum State {BoolFALSE, BoolTRUE, DC};
    State state;
};

class TruthTable {
public:
    TruthTable(int inputSize);
    TruthTable(int inputSize, std::vector<int>& v);

    TruthTable(const TruthTable&) = default;
    TruthTable(TruthTable&&) = default;

    TruthTable& operator=(const TruthTable&) = default;
    TruthTable& operator=(TruthTable&&) = default;

    const BoolVariable& operator[](int i) const;
    BoolVariable& operator[](int i);

    int numInput() {return this->inputSize;}
    int size() {return 1 << inputSize;}
    std::string& getName(int i);
    void print();

    std::string outName;
private:
    int inputSize;

    std::vector<std::string> name;
    std::vector<BoolVariable> data;

    void setByVector(std::vector<int>& v);
    void setNameByVector(std::vector<std::string> &name);

};


#endif //VE490_TRUTH_TABLE_H
