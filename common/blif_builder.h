//
// Created by king on 17-4-11.
//

#ifndef VE490_DECOMPOSITION_INFO_H
#define VE490_DECOMPOSITION_INFO_H

#include <unordered_map>
#include <set>
#include <ostream>
#include "ttable.h"

typedef std::string NodeName;
typedef boost::dynamic_bitset<> DBitset;

class BlifBuilder
{
    struct Connection
    {
        NodeName out;
        NodeName in1;
        NodeName in2;
        TTable method;

        Connection(const NodeName& out_,
                   const NodeName& i1_, const NodeName& i2_,
                   const TTable& method_);

        friend std::ostream &operator<<(std::ostream &os,
                                        const Connection &connection);

        std::string toBlifString() const;
    };

    std::vector<Connection> data;
    std::unordered_map<NodeName, bool> input;
    std::unordered_map<NodeName, bool> constant;

    NodeName outputNodeName;

    BlifBuilder() {}

public:
    std::ostream& printBody(std::ostream &os) const;

    static BlifBuilder BuildConst(const NodeName& node, bool value);

    bool isConst() {return input.empty();}

    BlifBuilder(const NodeName& node, bool flip=false);

    friend BlifBuilder
    combineBilfBuilder(const BlifBuilder &d1,
                       const BlifBuilder &d2,
                       const TTable &table,
                       const NodeName &newOutput);
    NodeName outputNode() const;

    void exportBlif(const std::string& filename) const;

    std::set<NodeName> inputName() const;

    friend std::ostream &operator<<(std::ostream &os,
                                    const Connection &connection);
    friend std::ostream &operator<<(std::ostream &os,
                                    const BlifBuilder &builder);
};

BlifBuilder
combineBilfBuilder(const BlifBuilder &d1,
                   const BlifBuilder &d2,
                   const TTable &table,
                   const NodeName &newOutput);

#endif //VE490_DECOMPOSITION_INFO_H
