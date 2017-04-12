//
// Created by king on 17-4-13.
//

#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <map>

#include "const.h"
#include "kmap.h"
#include "truth_table.h"

using std::string;
using std::vector;

BooleanFunction::BooleanFunction
        (const int& inSize,
         TTable& tTab,
         const vector<NodeName >& pName,
         const NodeName& oPName): truthTab(string("00"))
{

    inputSize =inSize;
    truthTab = tTab;
    portName = pName;
    outPortName = oPName;
}

BooleanFunction::~BooleanFunction() {
}

void BooleanFunction::operator=(const BooleanFunction &initBF) {
    this->inputSize = initBF.inputSize;
    this->truthTab = initBF.truthTab;
    this->portName = initBF.portName;
    this->outPortName = initBF.outPortName;
}

bool BooleanFunction::operator==(const BooleanFunction &initBF) {
    if (this->inputSize != initBF.inputSize)
        return false;

    if (this->outPortName != initBF.outPortName)
        return false;

    for (int i = 0; i < this->inputSize; ++i) {
        if (this->getProtNum( initBF.getPortName(i) ) == -1)
            return false;
    }

    for (int i = 0; i < (1 << (this->inputSize)); ++i) {
        size_t base = 0;
        size_t temp = i;
        for (int k = 0; k < inputSize; ++k) {
            base += (temp % 2) * ( 1 << (this->getProtNum( initBF.getPortName(k))));
            temp = temp / 2;
        }
        if (this->truthTab[base] != initBF.truthTab[i])
            return false;
    }

    return true;
}

int BooleanFunction::operator^(const BooleanFunction &initBF) {
    if (this->inputSize != initBF.inputSize)
        //assert(0);
        return -1;

    if (this->outPortName != initBF.outPortName)
        //assert(0);
        return -1;

    for (int i = 0; i < this->inputSize; ++i) {
        if (this->getProtNum( initBF.getPortName(i) ) == -1)
            //assert(0);
            return -1;
    }

    int count = 0;
    for (int i = 0; i < (1 << (this->inputSize)); ++i) {
        size_t base = 0;
        size_t temp = i;
        for (int k = 0; k < inputSize; ++k) {
            base += (temp % 2) * ( 1 << (this->getProtNum( initBF.getPortName(k))));
            temp = temp / 2;
        }
        if (this->truthTab[base] != initBF.truthTab[i])
            count ++;
    }

    return count;
}

BooleanFunction combineBooleanFunction(
        const BooleanFunction& bf1,
        const BooleanFunction& bf2,
        const TTable& method,
        const NodeName& outName )
{
    // build inputs' set
    vector<NodeName> nodeSet = bf1.portName;
    for (int i = 0; i < bf2.inputSize; ++i)
        nodeSet.push_back(bf2.getPortName(i));

    // inputSize;
    int newInputSize = bf1.inputSize + bf2.inputSize;

    // outPortName;
    NodeName outPortName = outName;

    // TTable;
    DBitset leftMask<>(newInputSize, 0);
    DBitset rightMask<>(newInputSize, 0);
    for (int i = 0; i < bf1.inputSize; ++i) {
        leftMask[i] = 1;
    }
    for (int i = bf1.inputSize; i < newInputSize; ++i) {
        rightMask[i] = 1;
    }
    TTable resTTable = combineTruthTable(
            bf1.truthTab, bf2.truthTab, leftMask, rightMask, method);

    return BooleanFunction(
            newInputSize, resTTable, nodeSet, outPortName
    );

}

int BooleanFunction::getInputSize() {
    return inputSize;
}

int BooleanFunction::getVal(const size_t term) {
    return truthTab[term];
}

int BooleanFunction::getVal(const DBitset &term) {
    return truthTab[term];
}

TTable& BooleanFunction::getTTable() {
    return truthTab;
}

int BooleanFunction::getProtNum(const NodeName &name) const  {
    for (int i = 0; i < inputSize; ++i) {
        if (portName[i] == name) return i;
    }
    return -1;
}

NodeName BooleanFunction::getPortName(const int &i) const {
    return portName[i];
}

NodeName BooleanFunction::getOutPortName() const {
    return outPortName;
}