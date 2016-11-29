//
// Created by tripack on 16-11-8.
//

#ifndef VE490_BOOLEAN_TREE_H
#define VE490_BOOLEAN_TREE_H

#include "BinaryTree.h"
#include "LUT2.h"
#include <string>

enum BOOLEAN_NODE_TYPE {
    INPUT, OUTPUT, PASS, UNKNOWN_TYPE
};

class BooleanNode {
public:
    BooleanNode(std::string name_,
                BOOLEAN_NODE_TYPE type_,
                LUT2 foo_) :
            name(name_),
            type(type_),
            fun(foo_) {};
public:
    std::string name;
    BOOLEAN_NODE_TYPE type;
    LUT2 fun;
};

class BooleanTree : public BinaryTree<BooleanNode> {
public:
    BooleanTree(BooleanNode* node)
            : BinaryTree<BooleanNode>::BinaryTree(node) {}

private:
};


#endif //VE490_BOOLEAN_TREE_H
