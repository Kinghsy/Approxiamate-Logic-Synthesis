//
// Created by tripack on 16-11-8.
//

#ifndef VE490_INTERFACE_H
#define VE490_INTERFACE_H

#include "../lib/cudd-2.5.0/cudd/cudd.h"
#include "../lib/libblif/bnet.h"
#include "memorize.h"
#include "../circuit_profile/profile.h"

#include <string>
#include <vector>
#include <truth_table.h>
#include <set>
#include <map>
#include <unordered_map>


class BlifBooleanNet {
public:
    typedef std::string BnetNodeID;

    struct FFC {
        BnetNodeID name;
        int depth2Input;
        int depth2Output;
        int minDepth2Input;
        int minDepth2Output;
        std::set<BnetNodeID> inputNode;
        std::set<BnetNodeID> nodeSet;
        std::set<BnetNodeID> totalSet;
    };

    struct SimulationResult {

        size_t nSamples;

        std::vector<std::vector<int> > inputResult;
        std::vector<std::vector<int> > outputResult;
        std::vector<std::vector<int> > internalResult;

        std::vector<std::string> inputName;
        std::vector<std::string> outputName;
        std::vector<std::string> internalName;

        SimulationResult(const BlifBooleanNet& net,
                         size_t nSamples);
    };

    struct CompareResult {

        size_t nSamples;

        size_t nErrors;

    };

private:
    struct NodeAttribute {
        int null;
        //std::map<BnetNodeID, int> depth2input;
        //std::map<BnetNodeID, int> depth2output;
    }attribute;

    mutable Memorized<DdManager*> ddmanager;

    void prepareBDD() const;

    BnetNetwork *net;
    std::string filename;

    mutable Memorized<int> nNode;
    mutable Memorized<int> nGates;
    mutable Memorized<std::vector<BnetNodeID> > inputNodes;
    mutable Memorized<std::set<BnetNodeID> > inputNodesSet;
    mutable Memorized<std::vector<BnetNodeID> > outputNodes;
    mutable Memorized<std::set<BnetNodeID> > outputNodesSet;
    mutable Memorized<std::set<BnetNodeID> > totalNodes;
    mutable Memorized<std::set<BnetNodeID> > internalNodes;
    mutable Memorized<std::vector<BnetNodeID> > topSortedNodes;
    mutable Memorized<void*> simulationContext;

    BnetNode* getNodeByName(const std::string& name) const;

    void prepareDepth2Input(BnetNode *node);
    void prepareDepth2Output(BnetNode *node);
    void prepareDepths();

    void* getSimulationContext() const;
    void releaseSimulationContext();

    std::map<BnetNodeID, std::set<BnetNodeID> >getMffcSet() const;
    std::map<BnetNodeID, std::set<BnetNodeID> >getFaninSet() const;
    std::set<BnetNodeID> getInputFromSet(const std::set<BnetNodeID>& set) const;

public:

    BlifBooleanNet(const std::string& file);

    const std::string& name() const;
    const std::string  netName() const;

    int nInputs() const;
    int nOutputs() const;
    int nodeCount() const;
    int gateCount() const;

    // Note inputs and output lists, vecs are not! are ORDERED!!
    const std::set<BnetNodeID> & inputNodeSet() const;
    const std::vector<BnetNodeID> & inputNodeList() const;
    const std::set<BnetNodeID> & outputNodeSet() const;
    const std::vector<BnetNodeID> & outputNodeList() const;
    const std::set<BnetNodeID> & internalNodeSet() const;
    const std::set<BnetNodeID> & totalNodeSet() const;

    int getMinDepths2Input(const std::set<BnetNodeID>& s) const;
    int getMinDepths2Output(const std::set<BnetNodeID>& s) const;

    TruthTable truthTable() const;

    const std::vector<BnetNodeID>& topologicalSort() const;

    //BlifBooleanNet getMFFC(int minInput, int maxInput) const;

    std::map<BnetNodeID, FFC> getFFC() const;

    int evalAt(const std::vector<int>& v,
               const std::string& node = "") const;
    std::vector<int> evalAllOutputAt(const std::vector<int>& v) const;

    CircuitProfile profile(int samples);

    void exportBlifToFile(const std::string& fname) const;
    void exportFfcToBlifFile(const FFC& ffc,const std::string& filename) const;
    void exportGraphViz(const std::string& fname) const;
    void exportGraphVizwithHighlight(const std::string& fname,
                                     const std::set<std::string >& HLNodes,
                                     const std::string color) const;

    void exportToCpp(const std::string& fname) const;

    void verifySimulator(int samples);

    SimulationResult profileBySimulation(int samples);

    CompareResult compareBySimulation(const BlifBooleanNet& net2,
                                      size_t nSamples);

    ~BlifBooleanNet();
};



#endif //VE490_INTERFACE_H
