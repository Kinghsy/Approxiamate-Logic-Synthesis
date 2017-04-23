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

void filterCurrentMffc(std::map<BnetNodeID, FFC>& ffc, const FFC& curr);

#endif //VE490_HEADER_H
