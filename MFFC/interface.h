//
// Created by tripack on 16-11-8.
//

#ifndef VE490_INTERFACE_H
#define VE490_INTERFACE_H

#include "exception.h"
#include "../lib/cudd-2.5.0/cudd/cudd.h"
#include "bnet.h"
#include "memorize.h"

#include <string>
#include <vector>
#include <truth_table.h>
#include <set>


class BlifBooleanNet {
    DdManager *ddmanager;
    BnetNetwork *net;
    std::string filename;

    mutable Memorized<int> nNode;
    mutable Memorized<int> nGates;
    mutable Memorized<std::set<string> > inputNodes;
    mutable Memorized<std::set<string> > outputNodes;
    mutable Memorized<std::set<string> > totalNodes;

    BnetNode* getNodeByName(const std::string& name);

public:
    BlifBooleanNet(const std::string& file);

    const std::string& name() const;
    const std::string  netName() const;

    int nInputs() const;
    int nOutputs() const;

    std::set<std::string> inputNodeSet() const;
    std::set<std::string> outputNodeSet() const;
    std::set<std::string> totalNodeSet() const;

    int nodeCount() const;
    int gateCount() const;

    TruthTable truthTable() const;

    BlifBooleanNet getMFFC(int minInput,
                           int maxInput) const;

    int evalAt(const std::vector<int>& v,
               const std::string& node = "") const;
    std::vector<int> evalAllOutputAt(const std::vector<int>& v) const;

    void exportBlifToFile(const std::string& fname) const;

    void exportGraphViz(const std::string& fname) const;

    ~BlifBooleanNet();
};



#endif //VE490_INTERFACE_H
