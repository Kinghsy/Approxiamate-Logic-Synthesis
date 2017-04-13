//
// Created by king on 17-4-11.
//

#ifndef VE490_DECOMPOSITION_INFO_H
#define VE490_DECOMPOSITION_INFO_H

#include<map>
#include<set>

#include "ttable.h"
#include "boolean_function.h"
#include "kmap.h"
#include "const.h"

class BlifBuilder
{
    struct Connection
    {
        NodeName out;
        NodeName in1;
        NodeName in2;
        TTable method;

        std::string toBlifString();
    };

    std::vector<Connection> data;
    std::unordered_map<NodeName, bool> input;

    NodeName outputNodeName;

    BlifBuilder() {}

public:

    BlifBuilder(const NodeName& node, bool flip=false);

    friend BlifBuilder
    combineBilfBuilder(const BlifBuilder &d1,
                       const BlifBuilder &d2,
                       const TTable &table,
                       const NodeName &newOutput);
    // for real program convenience
    // return combineDecompositionInfo(combineBilfBuilder(d1, d2) , d3)

    NodeName outputNode();

    void exportBlif(const std::string& filename);
    std::string exportBlifBody();

    std::set<NodeName> inputName();
};

BlifBuilder
combineBilfBuilder(const BlifBuilder &d1,
                   const BlifBuilder &d2,
                   const TTable &table,
                   const NodeName &newOutput);

#endif //VE490_DECOMPOSITION_INFO_H
