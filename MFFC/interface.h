//
// Created by tripack on 16-11-8.
//

#ifndef VE490_INTERFACE_H
#define VE490_INTERFACE_H

#include "exception.h"
#include "../lib/cudd-2.5.0/cudd/cudd.h"
#include "bnet.h"
#include <string>
#include <vector>

#include <truth_table.h>

class BlifBooleanNet {
    DdManager *ddmanager;
    BnetNetwork *net;
    std::string filename;

public:
    BlifBooleanNet(const std::string& file);

    const std::string& name() const;
    const std::string  netName() const;

    int nInputs() const;

    int nodeCount() const;
    int gateCount() const;

    TruthTable truthTable() const;

    BlifBooleanNet getMFFC(int minInput,
                           int maxInput) const;

    int evalAt(const std::vector<int>& v) const;

    ~BlifBooleanNet();
};



#endif //VE490_INTERFACE_H
