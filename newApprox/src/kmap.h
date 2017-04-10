//
// Created by king on 17-4-10.
//

#ifndef VE490_KMAP_H
#define VE490_KMAP_H

#include <vector>

#include "../../circuit_profile/sim_profile.h"
#include "bool_function.h"

class Kmap {

private:

    std::vector<std::vector<int> > kmap;
    std::vector<std::string > widthName;
    std::vector<std::string > heightName;

    FocusedSimulationResult simRes;

    int divideMode; // e.g.  divideMode = XOR_IGNORE

public:

    struct bestDivision {
        BoolFunction leftFun;
        BoolFunction rightFun;
        int errorCaseNo;  // maybe double type? not sure yet. FIXME
        int opera;
    };

    bestDivision getBestDiv();

    void operator= (const Kmap& initkmap);
    bool operator== (const Kmap& initkmap);
    std::vector<int> operator[] (const int& i);

    Kmap(const Kmap& initkmap);
    Kmap();
    ~Kmap();

    void display();

};


#endif //VE490_KMAP_H
