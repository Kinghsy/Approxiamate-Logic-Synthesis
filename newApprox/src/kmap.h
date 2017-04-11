//
// Created by king on 17-4-11.
//

#ifndef VE490_KMAP_H
#define VE490_KMAP_H

#include<vector>

#include "boolean_function.h"
#include "truth_table.h"
#include "const.h"
#include "../../circuit_profile/sim_profile.h"

class Kmap {

private:

    int height, width;
    std::vector<TruthTable > kmap;
    std::vector<std::string > heightName;
    std::vector<std::string > widthName;

public:

    struct BestApprox {
        BooleanFunction leftFunc;
        BooleanFunction rightFunc;
        int errorCount;
        combineMethod method;
    };

    Kmap(const BooleanFunction& BF,
         std::vector<std::string > heightName,
         std::vector<std::string > widthName);
    ~Kmap();

    bool operator== (const Kmap& initKmap);  // without name
    size_t operator^ (const Kmap& initKmap);
    TruthTable operator[] (const int& i);

    BestApprox divide(const SimulationResult& simData);

    int getHeight();
    int getWidth();
    std::string getHeightName(const int& i);
    std::string getWidthName(const int& j);


}

#endif //VE490_KMAP_H
