//
// Created by king on 17-4-11.
//

#ifndef VE490_NEW_BOOLEAN_FUNCTION_H
#define VE490_NEW_BOOLEAN_FUNCTION_H

#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <map>

#include "const.h"
#include "../../common/ttable.h"

class BoolFunction {
private:
    size_t inputSize;
    TTable truthTab;
    std::vector<NodeName> portName;
    NodeName outPortName;

public:

    BoolFunction(size_t inputSize,
                    const TTable& truthTab,
                    const std::vector<NodeName>& portName,
                    const NodeName& outPortName);

    void operator= (const BoolFunction &initBF);
    bool operator== (const BoolFunction &initBF);
    int operator^(const BoolFunction &initBF);

    friend BoolFunction combineBooleanFunction(
            const BoolFunction& bf1,
            const BoolFunction& bf2,
            const TTable& method,
            const NodeName& outName);

    inline size_t getInputSize() const {
        return inputSize;
    }
    inline bool getVal(const DBitset& term) const {
        return truthTab[term];
    }
    inline bool getVal(const size_t term) const {
        return truthTab[term];
    }
    bool isAll0s() const;
    bool isAll1s() const;

    TTable getTTable() const {
        return truthTab;
    }

    inline NodeName getOutPortName() const {
        return outPortName;
    }
    inline NodeName getPortName(size_t i) const {
        return portName.at(i);
    }
    int getPortNum(const NodeName& name) const;
    void display() const;
};

BoolFunction combineBooleanFunction(
        const BoolFunction& bf1,
        const BoolFunction& bf2,
        const TTable& method,
        const NodeName& outName
);

#endif //VE490_BOOLEAN_FUNCTION_H
