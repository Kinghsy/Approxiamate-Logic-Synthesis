//
// Created by tripack on 16-12-27.
//

#ifndef VE490_POLICY_H
#define VE490_POLICY_H

#include "../circuit/interface.h"

struct FfcSelectPolicy {
    int minInput;
    int maxInput;
    int numChoose;

    FfcSelectPolicy();
    void numAdd();
};

BlifBooleanNet::FFC
selectFFC(const std::map<BlifBooleanNet::BnetNodeID, BlifBooleanNet::FFC> &ffcCollection,
          FfcSelectPolicy &policy);

#endif //VE490_POLICY_H
