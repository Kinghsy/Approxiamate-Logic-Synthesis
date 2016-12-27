//
// Created by tripack on 16-11-8.
//

#ifndef VE490_INTERFACE_H
#define VE490_INTERFACE_H

#include "exception.h"
#include "../lib/cudd-2.5.0/cudd/cudd.h"
#include "bnet.h"
#include "memorize.h"
#include "basics.h"

#include <string>
#include <vector>
#include <truth_table.h>
#include <set>


class BlifBooleanNet {
public:
    typedef std::string BnetNodeID;

    struct FFC {
        BnetNodeID name;
        int depth2Input;
        int depth2Output;
        std::set<BnetNodeID> inputNode;
        std::set<BnetNodeID> nodeSet;
    };

private:
    struct NodeAttribute {
        std::map<BnetNodeID, int> depth2input;
        std::map<BnetNodeID, int> depth2output;
    }attribute;

    DdManager *ddmanager;
    BnetNetwork *net;
    std::string filename;

    mutable Memorized<int> nNode;
    mutable Memorized<int> nGates;
    mutable Memorized<std::set<string> > inputNodes;
    mutable Memorized<std::set<string> > outputNodes;
    mutable Memorized<std::set<string> > totalNodes;
    mutable Memorized<std::vector<string> > topSortedNodes;

    BnetNode* getNodeByName(const std::string& name) const;

    void prepareDepth2Input(BnetNode *node);
    void prepareDepth2Output(BnetNode *node);
    void prepareDepths();

    std::map<BnetNodeID, std::set<BnetNodeID> >getMffcSet() const;
    std::map<BnetNodeID, std::set<BnetNodeID> >getFaninSet() const;
    std::set<BnetNodeID> getInputFromSet(const std::set<BnetNodeID>& set) const;

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

    const std::vector<string>& topologicalSort() const;


    BlifBooleanNet getMFFC(int minInput,
                           int maxInput) const;

    std::map<BnetNodeID, FFC> getFFC() const;

    int evalAt(const std::vector<int>& v,
               const std::string& node = "") const;
    std::vector<int> evalAllOutputAt(const std::vector<int>& v) const;

    void exportBlifToFile(const std::string& fname) const;

    void exportFfcToBlifFile(const FFC& ffc,const std::string& filename) const;

    void exportGraphViz(const std::string& fname) const;
    void exportGraphVizwithHighlight(const std::string& fname, const std::set<std::string >& HLNodes, const std::string color) const;


    ~BlifBooleanNet();
};



#endif //VE490_INTERFACE_H
