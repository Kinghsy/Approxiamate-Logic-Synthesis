//
// Created by king on 17-4-11.
//

#ifndef VE490_NEW_KMAP_H
#define VE490_NEW_KMAP_H

#include<vector>

#include "const.h"
#include "bool_function.h"

class BooleanFunction;
class TTable;
class SimulationResult;

class Kmap {

private:

    size_t height, width;
    std::vector<TTable> kmap;
    std::vector<NodeName> heightName;
    std::vector<NodeName> widthName;

public:

    struct BestApprox {
        BooleanFunction leftFunc;
        BooleanFunction rightFunc;
        int errorCount;
        TTable method;
        BestApprox() :
                leftFunc(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                rightFunc(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                method(NORMAL_1_INPUT) {
            errorCount = MAX_VALUE;
        }
    };

    Kmap(const BooleanFunction& BF,
         const std::vector<NodeName > &heightName,
         const std::vector<NodeName > &widthName);
    ~Kmap();

    bool operator== (const Kmap& initKmap) const;  // without name
        // notice that == operation for Kmap in no-named,
        //  == operation for TruthTable is no-named,
        //  == operation for BooleanFunction is named.
    size_t operator^ (const Kmap& initKmap) const;

    inline TTable& operator[] (size_t i) {
        return kmap[i];
    }
    inline const TTable& operator[] (size_t i) const {
        return kmap[i];
    }

    BestApprox divide(const SimulationResult& simData);

    inline size_t getHeight() const {
        return height;
    }
    inline size_t getWidth() const {
        return width;
    }
    inline NodeName getHeightName(int i) const {
        if (i >= height) assert(0);
        return heightName[i];
    }
    inline NodeName getWidthName(int j) const {
        if (j >= width) assert(0);
        return widthName[j];
    }


};

std::string num2string(size_t bits, size_t num);

#endif //VE490_KMAP_H
