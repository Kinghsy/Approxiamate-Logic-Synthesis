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
    };

    Kmap(const BooleanFunction& BF,
         const std::vector<NodeName > &heightName,
         const std::vector<NodeName > &widthName);
    ~Kmap();

    bool operator== (const Kmap& initKmap) const;  // without name
        // notice that == operation for Kmap in no-named,
        //  == operation for TruthTable is no-named,
        //  == operation for BooleanFunction is named.
        // FIXME
    size_t operator^ (const Kmap& initKmap) const;

    TTable& operator[] (size_t i) {return kmap[i];}
    const TTable& operator[] (size_t i) const {return kmap[i];}

    BestApprox divide(const SimulationResult& simData);

    size_t getHeight() const {return height;}
    size_t getWidth() const {return width;}
    NodeName getHeightName(int i) const;
    NodeName getWidthName(int j) const;


};

#endif //VE490_KMAP_H
