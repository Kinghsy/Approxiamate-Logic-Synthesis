// Created by tripack on 16-12-26.
//
#include <cstdlib>
#include <list>
#include <map>

#include "interface.h"

const std::vector<string> &BlifBooleanNet::topologicalSort() {
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