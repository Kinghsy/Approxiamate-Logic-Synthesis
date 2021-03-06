//
// Created by tripack on 16-11-8.
//

#ifndef VE490_CIRCUIT_INTERFACE_H
#define VE490_CIRCUIT_INTERFACE_H

#include "memorize.h"
#include "../circuit_profile/profile.h"
#include "../circuit_profile/sim_profile.h"

#include <string>
#include <vector>
#include "../approxiamate/approx_common/truth_table.h"
#include <set>
#include <map>
#include <unordered_map>

struct DdManager;
struct BnetNetwork;
struct BnetNode;
class BlifBuilder;
class BoolFunction;

class BlifBooleanNet {
public:
    typedef std::string BnetNodeID;

    struct FFC {
        BnetNodeID name;
//        int depth2Input;
//        int depth2Output;
//        int minDepth2Input;
//        int minDepth2Output;
        std::set<BnetNodeID> inputNode;
        std::set<BnetNodeID> nodeSet;
        std::set<BnetNodeID> totalSet;
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
    mutable Memorized<std::unordered_map<BnetNodeID, size_t> > d2out;

    BnetNode* getNodeByName(const std::string& name) const;

    void prepareDepth2Input(BnetNode *node);
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

    size_t nInputs() const;
    size_t nOutputs() const;
    size_t nodeCount() const;
    size_t gateCount() const;

    // Note inputs and output lists, vecs are not! are ORDERED!!
    const std::set<BnetNodeID> & inputNodeSet() const;
    const std::vector<BnetNodeID> & inputNodeList() const;
    const std::set<BnetNodeID> & outputNodeSet() const;
    const std::vector<BnetNodeID> & outputNodeList() const;
    const std::set<BnetNodeID> & internalNodeSet() const;
    const std::set<BnetNodeID> & totalNodeSet() const;

    const std::unordered_map<BnetNodeID, size_t>& depth2Output() const;
//    int getMinDepths2Input(const std::set<BnetNodeID>& s) const;

    TTable truthTable() const;

    const std::vector<BnetNodeID>& topologicalSort() const;

    //BlifBooleanNet getMFFC(int minInput, int maxInput) const;

    std::map<BnetNodeID, FFC> getFFC() const;

    int evalAt(const std::vector<int>& v,
               const std::string& node = "") const;
    std::vector<int> evalAllOutputAt(const std::vector<int>& v) const;

    CircuitProfile profile(int samples);

    void exportBlifToFile(const std::string& fname) const;

    void exportPartialBlif(const std::string& fname,
                           std::set<BnetNodeID> omits) const;

    void exportReplacedBlif(const std::string& fname,
                            const FFC& ffc,
                            const BlifBuilder& blifBuilder);

    void exportReplacedBlif(const std::string& fname,
                            const std::vector<FFC>& ffcSet,
                            const std::vector<BlifBuilder>& blifSet);

    void exportFfcToBlifFile(const FFC& ffc,const std::string& filename) const;
    void exportGraphViz(const std::string& fname) const;
    void exportGraphVizwithHighlight(const std::string& fname,
                                     const std::set<std::string >& HLNodes,
                                     const std::string color) const;

    void exportToCpp(const std::string& fname) const;

    void verifySimulator(int samples);

    void prepareSimulator();

    SimulationResult profileBySimulation(int samples);

    CompareResult compareBySimulation(const BlifBooleanNet& net2,
                                      size_t nSamples);

    CompareResult localErrorSim(const BlifBooleanNet& net2,
                                size_t nSamples, NodeName node);

    ~BlifBooleanNet();
};




#endif //VE490_INTERFACE_H
