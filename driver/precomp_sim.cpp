//
// Created by tripack on 4/16/17.
//

#include "header.h"
#include <common.h>

#include <memory>

typedef PreDecomp::DbEntry DbEntry;

using std::vector;

BlifBuilder
BuildCircuitFromMatch::operator()(const DbEntry& entry,
                                  const vector<BnetNodeID>& node) {
   return this->build(entry, node, mffcNode);
}

BlifBuilder
BuildCircuitFromMatch::build(const PreDecomp::DbEntry &entry,
                             const std::vector<BnetNodeID> &node,
                             const BnetNodeID &thisNode) {
    assert(entry.nInputs == node.size());

    const auto& preDecomp = PreDecomp::getInstance();
    const auto& leftFun = entry.left;
    const auto& rightFun = entry.right;

    BlifBuilder leftBlif("none");
    BlifBuilder rightBlif("none");

    if (entry.combine == ALL_IRR_TABLE_0) {
        return BlifBuilder::BuildConst(thisNode, false);
    } else if (entry.combine == ALL_IRR_TABLE_1) {
        return BlifBuilder::BuildConst(thisNode, true);
    }

    if (leftFun.nInputs() == 1) {
        size_t nodeNum = entry.leftMask.find_first();
        leftBlif = BlifBuilder(node.at(nodeNum),
                               entry.left == NOT_1_INPUT);
    } else if (leftFun.nInputs() == 2) {
        size_t leftNum = entry.leftMask.find_first();
        size_t rightNum = entry.leftMask.find_next(leftNum);
        auto b1 = BlifBuilder(node.at(leftNum));
        auto b2 = BlifBuilder(node.at(rightNum));
        leftBlif = combineBilfBuilder(b1, b2, entry.left, thisNode);
    } else {
        const auto leftMatch = preDecomp.getMatch(leftFun.cdata(), leftFun.nInputs());
        leftBlif = build(leftMatch, pickByDbitset(node, entry.leftMask),
                         nameGenerator.generate());
    }

    if (rightFun.nInputs() == 1) {
        size_t nodeNum = entry.rightMask.find_first();
        rightBlif = BlifBuilder(node.at(nodeNum),
                                entry.right == NOT_1_INPUT);
    } else if (rightFun.nInputs() == 2) {
        size_t leftNum = entry.rightMask.find_first();
        size_t rightNum = entry.rightMask.find_next(leftNum);
        auto b1 = BlifBuilder(node.at(leftNum));
        auto b2 = BlifBuilder(node.at(rightNum));
        rightBlif = combineBilfBuilder(b1, b2, entry.right,
                                       nameGenerator.generate());
    } else {
        const auto rightMatch = preDecomp.getMatch(rightFun.cdata(), rightFun.nInputs());
        rightBlif = build(rightMatch,
                          pickByDbitset(node, entry.rightMask),
                          nameGenerator.generate());
    }

    return combineBilfBuilder(leftBlif, rightBlif, entry.combine, thisNode);
}

