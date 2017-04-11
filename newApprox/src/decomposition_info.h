//
// Created by king on 17-4-11.
//

#ifndef VE490_DECOMPOSITION_INFO_H
#define VE490_DECOMPOSITION_INFO_H

#include<map>
#include<set>

#include "boolean_function.h"
#include "kmap.h"
#include "const.h"

class DecompositionInfo
{
    std::map<nodeName, connection> data;
    nodeName outputNodeName;

public:

    DecompositionInfo();
    DecompositionInfo(const nodeName& nodeName1,
            const connection& connection1);

    friend DecompositionInfo combineDecompositionInfo(
            const DecompositionInfo & d1,
            const DecompositionInfo & d2);
    friend DecompositionInfo combineDecompositionInfo(
            const DecompositionInfo & d1,
            const DecompositionInfo & d2,
            const DecompositionInfo & d3);
    // for real program convenience
    // return combineDecompositionInfo(combineDecompositionInfo(d1, d2) , d3)

    BooleanFunction buildDecompInfo();

    struct connection
    {
        nodeName n1;
        nodeName n2;
        combineMethod method;
    };

    nodeName outputNodeName();
    void exportBlif(const std::string& filename);
    std::set<nodeName> outputName();
};



#endif //VE490_DECOMPOSITION_INFO_H
