//
// Created by tripack on 16-12-27.
//

#ifndef VE490_POLICY_H
#define VE490_POLICY_H

#include "../MFFC/interface.h"

struct FfcSelectPolicy {
    int minInput;
    int maxInput;

    FfcSelectPolicy();
};

BlifBooleanNet::FFC
selectFFC(const std::map<BlifBooleanNet::BnetNodeID, BlifBooleanNet::FFC> &ffcCollection,
          const FfcSelectPolicy &policy = FfcSelectPolicy());

#endif //VE490_POLICY_H
