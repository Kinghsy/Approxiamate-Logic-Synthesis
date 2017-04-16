//
// Created by tripack on 4/16/17.
//

#ifndef VE490_DRIVER_HEADER_H
#define VE490_DRIVER_HEADER_H

#include <map>
#include "../circuit/interface.h"
#include "../pre_decomp/pre_decomp.h"


void filterMffcBySize(map<BnetNodeID, FFC>& mffc);

void filterMffcContainOutput(map<BnetNodeID, FFC>& mffc,
                             const vector<BnetNodeID>& output);



#endif //VE490_HEADER_H
