//
// Created by king on 17-4-11.
//

#ifndef VE490_KMAP_H
#define VE490_KMAP_H

#include<vector>

#include "boolean_function.h"
#include "ttable.h"
#include "const.h"
#include "../../circuit_profile/sim_profile.h"

class Kmap {

private:

    size_t height, width;
    std::vector<TTable > kmap;
    std::vector<NodeName > heightName;
    std::vector<NodeName > widthName;

public:

    struct BestApprox {
        BooleanFunction leftFunc;
        BooleanFunction rightFunc;
        int errorCount;
        TTable method;
    };

    Kmap(const BooleanFunction& BF,
         std::vector<NodeName > heightName,
         std::vector<NodeName > widthName);
    ~Kmap();

    bool operator== (const Kmap& initKmap);  // without name
        // notice that == operation for Kmap in no-named,
        //  == operation for TruthTable is no-named,
        //  == operation for BooleanFunction is named.
        // FIXME
    size_t operator^ (const Kmap& initKmap);
    TTable& operator[] (const int& i);

    BestApprox divide(SimulationResult& simData);

    size_t getHeight();
    size_t getWidth();
    NodeName getHeightName(const int& i);
    NodeName getWidthName(const int& j);


};

#endif //VE490_KMAP_H
