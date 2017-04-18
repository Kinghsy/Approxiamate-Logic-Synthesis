//
// Created by king on 17-4-11.
//

#ifndef VE490_NEW_KMAP_H
#define VE490_NEW_KMAP_H

#include<vector>

#include "const.h"
#include "bool_function.h"
#include "../../common/ttable.h"
#include "../../circuit_profile/sim_profile.h"

class BoolFunction;
class TTable;
class SimulationResult;

class Kmap {

public:
    struct BestApprox {
        BoolFunction leftFunc;
        BoolFunction rightFunc;
        size_t errorCount;
        TTable method;
        BestApprox() :
                leftFunc(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                rightFunc(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                method(NORMAL_1_INPUT) {
            errorCount = MAX_VALUE;
        }
    };

private:

    size_t height, width;
    std::vector<TTable> kmap;
    std::vector<NodeName> heightName;
    std::vector<NodeName> widthName;

    TTable getColumnPattern(
            const FocusedSimulationResult& focusSim,
            const TTable& columnPattern,
            const TTable& rowPattern,
            const TTable& combineMethod
    );

    TTable getRowPattern(
            const FocusedSimulationResult& focusSim,
            const TTable& columnPattern,
            const TTable& rowPattern,
            const TTable& combineMethed
    );

    size_t errorCountRowColumnPattern(
            const size_t column,
            const size_t row,
            const bool columnVal,
            const bool rowVal,
            const TTable& combineMethod
    );

    Kmap::BestApprox errorCountWhole(
            const FocusedSimulationResult& focusSim,
            const TTable& columnPattern,
            const TTable& rowPattern,
            const TTable& combineMethod
    );

public:

    Kmap(const BoolFunction& BF,
         const std::vector<NodeName > &heightName,
         const std::vector<NodeName > &widthName);

    bool operator== (const Kmap& initKmap) const;  // without name
        // notice that == operation for Kmap in no-named,
        //  == operation for TruthTable is no-named,
        //  == operation for BoolFunction is named.
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

    void display() const;

};

void num2Dbits(DBitset& db, size_t bits,
               size_t start, size_t num);

#endif //VE490_KMAP_H
