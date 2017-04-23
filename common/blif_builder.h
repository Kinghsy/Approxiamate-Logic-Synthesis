//
// Created by king on 17-4-11.
//

#ifndef VE490_DECOMPOSITION_INFO_H
#define VE490_DECOMPOSITION_INFO_H

#include <unordered_map>
#include <set>
#include <ostream>
#include <memory>
#include "ttable.h"

typedef std::string NodeName;
typedef boost::dynamic_bitset<> DBitset;

class BlifBuilder {
private:
    typedef std::shared_ptr<NodeName> NodeNamePtr;

    enum TYPE {
        INPUT, REVERSE_INPUT,
        CONST_0, CONST_1, NET
    };

    TYPE type;

    struct Connection
    {
        NodeNamePtr out = nullptr;
        NodeNamePtr in1 = nullptr;
        NodeNamePtr in2 = nullptr;
        TTable method = ALL_IRR_TABLE_0;

        friend std::ostream &operator<<(std::ostream &os,
                                        const Connection &connection);


    };

    std::vector<Connection> data;
    std::vector<NodeNamePtr> input;
    std::vector<NodeNamePtr> constant0;
    std::vector<NodeNamePtr> constant1;

    NodeNamePtr node;

    BlifBuilder() {}

    BlifBuilder(TYPE t, const NodeName& node)
        : type(t), node(std::make_shared<NodeName>(node)) {
        if (t == INPUT || t == REVERSE_INPUT)
            this->input.push_back(this->node);
        if (t == CONST_0) this->constant0.push_back(this->node);
        if (t == CONST_1) this->constant1.push_back(this->node);
    }

public:
    std::ostream& printBody(std::ostream &os) const;

    static BlifBuilder buildConst(const NodeName& node, bool value);
    static BlifBuilder buildInput(const NodeName& node, bool value);

    friend BlifBuilder
    combineBilfBuilder(const BlifBuilder &d1,
                       const BlifBuilder &d2,
                       const TTable &table,
                       const NodeName &newOutput);

    NodeName outputNode() const;
    
    void replaceInputOrder(const std::vector<NodeName>& n);

    size_t nNode() const {return this->data.size();}

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
