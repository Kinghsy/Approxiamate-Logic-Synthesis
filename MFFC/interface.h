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

std::string getMFFC(std::string infile,
                    int minInput,
                    int maxInput);

TruthTable getTruthTableFromBlif(const std::string& blifContent);


class BlifBooleanNet {
    DdManager *ddmanager;
    BnetNetwork *net;
    std::string filename;

public:
    BlifBooleanNet(const std::string& file);

    const std::string& name() const;

    int nInputs() const;

    TruthTable truthTable() const;

    int evalAt(const std::vector<int>& v) const;
    int evalAt(int v);

    ~BlifBooleanNet();
};



#endif //VE490_INTERFACE_H
