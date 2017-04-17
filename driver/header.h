//
// Created by tripack on 4/16/17.
//

#ifndef VE490_DRIVER_HEADER_H
#define VE490_DRIVER_HEADER_H

#include "../circuit/interface.h"
#include "../pre_decomp/pre_decomp.h"

typedef BlifBooleanNet::BnetNodeID BnetNodeID;
typedef BlifBooleanNet::FFC FFC;

void filterMffcBySize(std::map<BnetNodeID, FFC>& mffc);

void filterMffcContainOutput(std::map<BnetNodeID, FFC>& mffc,
                             const std::vector<BnetNodeID>& output);

const FFC* findFirstFFC(std::map<BnetNodeID, FFC>& mffc,
                     std::function<bool(const FFC&)> test);

void filterMffcByIntersection(std::map<BnetNodeID, FFC>& mffc, const FFC& prev);

class BuildCircuitFromMatch {
    const BnetNodeID mffcNode;
    NodeNameGenerator nameGenerator;

    BlifBuilder build(const PreDecomp::DbEntry& entry,
                      const std::vector<BnetNodeID>& node,
                      const BnetNodeID& thisNode);

public:
    BuildCircuitFromMatch(const BnetNodeID& node)
            : mffcNode(node), nameGenerator(node + "_mffc") {}

    BlifBuilder operator()(const PreDecomp::DbEntry& entry,
                           const std::vector<BnetNodeID>& node);
};

#endif //VE490_HEADER_H
