//
// Created by tripack on 16-12-27.
//

#include <cstdio>
#include <unordered_set>
#include <iostream>
#include "policy.h"

using std::cout;
using std::cerr;
using std::endl;

typedef BlifBooleanNet::BnetNodeID ID;
typedef BlifBooleanNet::FFC Ffc;
typedef std::map<ID, Ffc> NodeFfcMap;
typedef std::vector<Ffc> FfcCollection;

FfcSelectPolicy::FfcSelectPolicy()
        : minInput(4), maxInput(8), numChoose(0) {}

void FfcSelectPolicy::numAdd() { numChoose++; return; }

FfcCollection filterCollection(const NodeFfcMap &collection,
                               const FfcSelectPolicy &policy);



BlifBooleanNet::FFC
selectFFC(const NodeFfcMap &ffcCollection,
          FfcSelectPolicy &policy) {
    FfcCollection c = filterCollection(ffcCollection, policy);
    FfcCollection::iterator optimumFfcIte;
    //const Ffc *optimumFfc;
    for (int i = 0; i <= policy.numChoose; ++i) {
        if (c.size() == 0) {
            cerr << "No enough candidate" << endl;
            assert(0);
        }
        optimumFfcIte = c.begin();
        for(FfcCollection::iterator it = c.begin(); it != c.end(); it++ ) {
            //if (optimumFfcIte == nullptr) optimumFfc = &ffc;
            if ((*optimumFfcIte).nodeSet.size() < (*it).nodeSet.size())
                optimumFfcIte = it;
        }
        if ( i!= policy.numChoose) c.erase(optimumFfcIte);
    }
    policy.numAdd();
    //assert(optimumFfc != nullptr);
    return *optimumFfcIte;
}

FfcCollection filterCollection(const NodeFfcMap &collection,
                               const FfcSelectPolicy &policy) {
    FfcCollection c;
    for (const auto& ffc_pair : collection) {
        const Ffc& ffc = ffc_pair.second;
        if (ffc.inputNode.size() < policy.minInput) continue;
        if (ffc.inputNode.size() > policy.maxInput) continue;
        // For an n input circuit, it must have at least:
        // 1 output node, n input nodes, n - 2 intermedian nodes.
        // FFC with less than that number of nodes
        // is not possible for further reduction
        if (ffc.nodeSet.size() <= ffc.inputNode.size() - 1) continue;

        c.push_back(ffc_pair.second);
    }
    return c;
}


