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
public:
    typedef std::string BnetNodeID;

private:
    DdManager *ddmanager;
    BnetNetwork *net;
    std::string filename;

    mutable Memorized<int> nNode;
    mutable Memorized<int> nGates;
    mutable Memorized<std::set<string> > inputNodes;
    mutable Memorized<std::set<string> > outputNodes;
    mutable Memorized<std::set<string> > totalNodes;
    mutable Memorized<std::vector<string> > topSortedNodes;

    BnetNode* getNodeByName(const std::string& name);

public:

    BlifBooleanNet(const std::string& file);

    const std::string& name() const;
    const std::string  netName() const;

    int nInputs() const;
    int nOutputs() const;

    const std::set<string> & inputNodeSet() const;
    const std::set<string> & outputNodeSet() const;
    const std::set<string> & totalNodeSet() const;

    int nodeCount() const;
    int gateCount() const;

    TruthTable truthTable() const;

    const std::vector<string>& topologicalSort();

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
