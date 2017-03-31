// Created by tripack on 16-12-26.
//
#include <cstdlib>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <algorithm>

#include "interface.h"

using std::string;
using std::vector;
using std::list;
using std::map;
using std::set;

const std::vector<string> &BlifBooleanNet::topologicalSort() const {
    if (topSortedNodes.isValid())
        return topSortedNodes.get();

    vector<BnetNodeID> sortedList;
    list<BnetNodeID> sortingQueue;
    map<BnetNodeID, int> indegreeTable;

    for (BnetNode *nd = net->nodes; nd != NULL; nd = nd->next)
    {
        indegreeTable[nd->name] = nd->ninp;
        if(nd->ninp == 0) sortingQueue.push_back(string(nd->name));
    }

    while(!sortingQueue.empty())
    {
        BnetNodeID pnode = sortingQueue.front();
        sortingQueue.pop_front();
        BnetNode *tmp = getNodeByName(pnode);
        sortedList.push_back(pnode);
        for(int i = 0; i < tmp->nfo; i++) {
            BnetNodeID outnode = tmp->fanouts[i];
            indegreeTable.at(outnode) -= 1;
            if (indegreeTable[outnode] == 0)
                sortingQueue.push_back(outnode);
        }
    }

    topSortedNodes.setData(sortedList);
    return topSortedNodes.get();
}

void BlifBooleanNet::prepareDepth2Input(BnetNode *node) {
//    int thisDepth = attribute.depth2input.at(node->name);
//    for (int i = 0; i < node->nfo; ++i) {
//        char* name = node->fanouts[i];
//        if (attribute.depth2input.count(name)) {
//            int& currentDepth = attribute.depth2input.at(name);
//            if (currentDepth > thisDepth + 1)
//                currentDepth = thisDepth + 1;
//        } else {
//            attribute.depth2input.insert(
//                    std::make_pair(name, thisDepth + 1)
//            );
//        }
//        prepareDepth2Input(getNodeByName(name));
//    }
}

void BlifBooleanNet::prepareDepth2Output(BnetNode *node) {
//    int thisDepth = attribute.depth2output.at(node->name);
//    for (int i = 0; i < node->ninp; ++i) {
//        char* name = node->inputs[i];
//        if (attribute.depth2output.count(name)) {
//            int& currentDepth = attribute.depth2output.at(name);
//            if (currentDepth > thisDepth + 1)
//                currentDepth = thisDepth + 1;
//        } else {
//            attribute.depth2output.insert(
//                    std::make_pair(name, thisDepth + 1)
//            );
//        }
//        prepareDepth2Output(getNodeByName(name));
//    }
}

void BlifBooleanNet::prepareDepths() {
//    for (int i = 0; i < net->ninputs; ++i) {
//        BnetNode* node = getNodeByName(net->inputs[i]);
//        attribute.depth2input[net->inputs[i]] = 0;
//        prepareDepth2Input(node);
//    }
//    for (int i = 0; i < net->noutputs; ++i) {
//        BnetNode* node = getNodeByName(net->outputs[i]);
//        attribute.depth2output[net->outputs[i]] = 0;
//        prepareDepth2Output(node);
//    }
}

std::map<BlifBooleanNet::BnetNodeID, BlifBooleanNet::FFC>
BlifBooleanNet::getFFC() const {
    std::map<BnetNodeID, std::set<BnetNodeID> > mffc = getMffcSet();
    std::map<BnetNodeID, BlifBooleanNet::FFC> allFfc;
    for (auto& mffc_pair : mffc) {
        BlifBooleanNet::FFC ffc;
        ffc.nodeSet = mffc_pair.second;
        ffc.inputNode = getInputFromSet(mffc_pair.second);
        ffc.name = mffc_pair.first;
//        ffc.depth2Input = attribute.depth2input.at(ffc.name);
//        ffc.depth2Output = attribute.depth2output.at(ffc.name);
        ffc.totalSet = ffc.nodeSet;
        ffc.totalSet.insert(
                ffc.inputNode.begin(),
                ffc.inputNode.end()
        );
        ffc.minDepth2Input = getMinDepths2Input(ffc.totalSet);
        ffc.minDepth2Output = getMinDepths2Output(ffc.totalSet);
        allFfc.insert(std::make_pair(ffc.name, ffc));
    }
    return allFfc;
}

std::map<BlifBooleanNet::BnetNodeID, std::set<BlifBooleanNet::BnetNodeID> >
BlifBooleanNet::getFaninSet() const{
    const vector<BnetNodeID> sortedList = topologicalSort();
    map<BnetNodeID, set<BnetNodeID> > faninNetwork;
    for (auto& nodeName : sortedList) {
        set<BnetNodeID> fanin;
        fanin.insert(nodeName);
        BnetNode *node = getNodeByName(nodeName);
        for (int i = 0; i < node->ninp; ++i) {
            char* name = node->inputs[i];
            const set<BnetNodeID>& predFanin = faninNetwork.at(name);
            fanin.insert(predFanin.begin(), predFanin.end());
        }
        faninNetwork.insert(std::make_pair(nodeName, fanin));
    }

    return faninNetwork;
}

std::map<BlifBooleanNet::BnetNodeID, std::set<BlifBooleanNet::BnetNodeID> >
BlifBooleanNet::getMffcSet() const{
    // Acquires the fanin network for each node
    vector<BnetNodeID> reverseSortedList(
            topologicalSort().rbegin(),
            topologicalSort().rend()
    );
    std::map<BnetNodeID, std::set<BnetNodeID> > faninNetwork = getFaninSet();

    for (const auto& nodeID : reverseSortedList) {
        set<BnetNodeID>& faninNet = faninNetwork.at(nodeID);
        set<BnetNodeID> toBeErased;
        for (auto it = faninNet.rbegin(); it != faninNet.rend(); ++it) {
            if (*it == nodeID) continue;
            // Node under inspection is the output, ignore.
            if (toBeErased.count(nodeID)) continue;
            // This node is already deleted.

            BnetNode* node = getNodeByName(*it);
            for (int i = 0; i < node->nfo; ++i) {
                std::string fout = node->fanouts[i];
                if (!faninNet.count(fout))
                    toBeErased.insert(
                            faninNetwork[*it].begin(),
                            faninNetwork[*it].end()
                    );
            }
        }

        for (auto it = faninNet.begin(); it != faninNet.end(); ) {
            if (toBeErased.count(*it))
                it = faninNet.erase(it);
            else
                it++;
        }
    }

    return faninNetwork;
}

std::set<BlifBooleanNet::BnetNodeID>
BlifBooleanNet::getInputFromSet(
        const std::set<BlifBooleanNet::BnetNodeID> &set) const{
    std::set<BnetNodeID> inputSet;
    for (const auto& nodeId : set) {
        BnetNode* node = getNodeByName(nodeId);
        if (node->ninp == 0) {
            inputSet.insert(nodeId);
            continue;
        }
        for (int i = 0; i < node->ninp; ++i) {
            BnetNodeID pred = node->inputs[i];
            BnetNode *predNode = getNodeByName(pred);
            if (predNode->type == BNET_INPUT_NODE) {
                inputSet.insert(pred);
            } else if (!set.count(pred)) {
                inputSet.insert(pred);
            }

        }
    }
    return inputSet;
}

int BlifBooleanNet::getMinDepths2Input(const std::set<BnetNodeID> &s) const {
//    std::vector<int> v;
//    v.resize(s.size());
//    std::transform(s.begin(), s.end(), v.begin(),
//                   [this] (const BnetNodeID& nodeID) -> int  {
//                       return attribute.depth2input.at(nodeID);
//                   }
//    );
//    return *min_element(v.begin(), v.end());
    return -1;
}

int BlifBooleanNet::getMinDepths2Output(const std::set<BnetNodeID> &s) const {
//    std::vector<int> v;
//    v.resize(s.size());
//    std::transform(s.begin(), s.end(), v.begin(),
//                   [this] (const BnetNodeID& nodeID) -> int {
//                       return attribute.depth2output.at(nodeID);
//                   }
//    );
//    return *min_element(v.begin(), v.end());
    return -1;
}
