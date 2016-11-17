//
// Created by tripack on 16-11-18.
//

#ifndef VE490_TRUTH_TABLE_H
#define VE490_TRUTH_TABLE_H

#include <vector>
#include <string>

class BoolVariable {
public:
    BoolVariable() : state(FALSE) {};
    BoolVariable(const BoolVariable& b) = default;
    BoolVariable(const BoolVariable&& b) = default;
    explicit BoolVariable(const bool& b) { copy(b); };
    explicit BoolVariable(const int& i) { copy(i); }

    BoolVariable& operator=(const BoolVariable& b) = default;
    BoolVariable& operator=(const BoolVariable&& b) = default;
    explicit BoolVariable& operator=(const bool& b) {
        return copy(b);
    }
    explicit BoolVariable& operator=(const int& i) {
        return copy(i);
    }

    explicit operator bool() const {
        if (state == TRUE) return true;
    }
    explicit operator int() const {
        if (state == TRUE) return 1;
        if (state == DC) return -1;
        return 0;
    }

    explicit operator const char*() const {
        if (state == TRUE) return "true";
        if (state == DC) return "dont_care";
        return "false";
    }

    std::string toString() const {
        if (state == TRUE) return "true";
        if (state == DC) return "dont_care";
        return "false";
    }

private:
    BoolVariable& copy(const bool& b) {
        if (b) {state = TRUE; return *this;}
        state = FALSE;
    }

    BoolVariable& copy(const int& i) {
        if (i) {state = TRUE; return * this;}
        state = FALSE;
    }

    enum State {FALSE, TRUE, DC};
    State state;
};

class TruthTable {
public:
    TruthTable(int inputSize);
    TruthTable(int inputSize, std::vector<int>& v);
    TruthTable(int inputSize, std::vector<int>&& v);

    TruthTable(const TruthTable&) = default;
    TruthTable(const TruthTable&&) = default;

    TruthTable& operator=(const TruthTable&) = default;
    TruthTable& operator=(const TruthTable&&) = default;

    const BoolVariable& operator[](int i) const;
    BoolVariable& operator[](int i);

    int numInput() {return this->inputSize;}
    int Size() {return 1 << inputSize;}

private:
    int inputSize;
    std::vector<BoolVariable> data;

    void setByVector(std::vector<int>&& v);
};


#endif //VE490_TRUTH_TABLE_H
