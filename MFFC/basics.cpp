#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cmath>
#include <cassert>
#include <ctime>
#include <sys/timeb.h>
#include "queue.h"
#include "bnet.h"
#include "basics.h"

using namespace std;

/*
functions in this file:
1. void topSort(BnetNetwork **net, vector<char*> &sort_list)
2. void get_MFFC(BnetNetwork *net, vector<char*> &sort_list, map<char*, set<char*> > &TFI_set_char, map<char*, set<char*> > &MFFC_set)
3. void find_insig_MFFC(BnetNetwork *net, map<char*, set<char*> > &MFFC_set, multimap<double, char*> &sp_MFFC, map<char*, map<char*, char*> > &insig_MFFC)
*/

//Global variables and external variables

BnetNode* getNodeFromNet(BnetNetwork **net, const BnetNodeID& nodeID) {
    BnetNode *tmp = NULL;
    st_lookup((*net)->hash, (void*)(nodeID.c_str()), &tmp);
    assert(tmp != NULL);
    return tmp;
}


/*topSort()*/
vector<BnetNodeID> topSort(BnetNetwork **net)
{

    vector<BnetNodeID> sortedList;
    list<BnetNodeID> sortingQueue;
    map<BnetNodeID, int> indegreeTable;

    for (BnetNode *nd = (*net)->nodes; nd != NULL; nd = nd->next)
    {
        indegreeTable[nd->name] = nd->ninp;
        if(nd->ninp == 0) sortingQueue.push_back(string(nd->name));
    }

    while(!sortingQueue.empty())
    {
        BnetNodeID pnode = sortingQueue.front();
        sortingQueue.pop_front();
        BnetNode *tmp = getNodeFromNet(net, pnode);
        sortedList.push_back(pnode);
        for(int i = 0; i < tmp->nfo; i++) {
            BnetNodeID outnode = tmp->fanouts[i];
            indegreeTable.at(outnode) -= 1;
            if (indegreeTable[outnode] == 0)
                sortingQueue.push_back(outnode);
        }
    }
    return sortedList;

    /*
	BnetNode *nd, *auxnd, *tmp;
	queue sort_queue;
	vector<BnetNodeID> sort_list;
    map<char*, int> indegree_list;
	map<char*, int>::iterator itrm_si;
	nd = (*net)->nodes;
	int i = 0;
	string name_str;
	while (nd != NULL) {
		indegree_list.insert(pair<char*, int>(nd->name, nd->ninp));
		if(nd->ninp == 0)
			sort_queue.push(nd->name);
		nd = nd->next;
	}

	while(!sort_queue.empty()) {
		sort_queue.traverse();
		char *pnode = sort_queue.pop();
		st_lookup((*net)->hash, pnode, &tmp);
		sort_list.push_back(tmp->name);
		for(int i = 0; i < tmp->nfo; i++) {
			char *outnode = tmp->fanouts[i];
			st_lookup((*net)->hash, outnode, &auxnd);
			itrm_si = indegree_list.find(auxnd->name);
			if(itrm_si == indegree_list.end()) {
				exit(1);
			} else {
				itrm_si->second = itrm_si->second - 1;
				if(itrm_si->second == 0)
					sort_queue.push(auxnd->name);
			}
		}
	}
    return sort_list;
    */
}


/*MFFC()*/
void get_MFFC(BnetNetwork *net,
              const vector<BnetNodeID> &sort_list,
              map<BnetNodeID, set<BnetNodeID> > &TFI_set_char,
              map<BnetNodeID, set<BnetNodeID> > &MffcDirectory)
{
    // Assign a unique index to each node in the sorted list
    map<int, BnetNodeID> index2Node;
    map<BnetNodeID, int> node2Index;
    for(int i = 0; i < sort_list.size(); i++) {
		index2Node.insert(pair<int, BnetNodeID>(i, sort_list[i]));
		node2Index.insert(pair<BnetNodeID, int>(sort_list[i], i));
	}

/*	cout << "ind_sort" << endl;
	for(itrm_ic = ind_sort.begin(); itrm_ic != ind_sort.end(); itrm_ic++)
		cout << itrm_ic->first << ", " << itrm_ic->second << endl;
*/
    map<BnetNodeID, map<int, BnetNodeID> > fanInNodeDirectory;
	for(auto nodePair : index2Node) {
        BnetNodeID id = nodePair.second;
        BnetNode *nd = getNodeFromNet(&net, id);
        //st_lookup(net->hash, cnode, &nd);
        //cout << "cnode = " << cnode << endl;
        map<int, BnetNodeID> currentFanInList;
        currentFanInList.insert(nodePair);
        for(int j = 0; j < nd->ninp; j++) {
        	BnetNodeID inputNode = nd->inputs[j];
            map<int, BnetNodeID> predFanInList =
                    fanInNodeDirectory.at(inputNode);
			for(auto node : predFanInList) currentFanInList.insert(node);
        }
        fanInNodeDirectory.insert(std::make_pair(id, currentFanInList));

        set<BnetNodeID> fanInNodeNameSet;
        for(auto node : currentFanInList) fanInNodeNameSet.insert(node.second);
        MffcDirectory.insert(std::make_pair(nd->name, fanInNodeNameSet));
        TFI_set_char.insert(std::make_pair(nd->name, fanInNodeNameSet));
    }
    
    for(auto mffcOutNodeID : sort_list) {
        map<int, BnetNodeID> currentFanInList =
                fanInNodeDirectory.at(mffcOutNodeID);
        map<int, BnetNodeID> toBeDeletedNodeList;
        if(currentFanInList.size() > 1) {
            // Verify the fanout for every node in the potential list
            for (auto itrm_ic = currentFanInList.rbegin();
                 itrm_ic != currentFanInList.rend();
                 itrm_ic++) { // Reverse traverse the list. important!!
                BnetNodeID checkingNodeID = itrm_ic->second;
                if (mffcOutNodeID == checkingNodeID) continue; // Skip the output node (current node)
                //cout << "checking node " << mffcOutNode << endl;
                BnetNode *checkingNode = getNodeFromNet(&net, checkingNodeID);
                for (int j = 0; j < checkingNode->nfo; j++) {
                    BnetNodeID fanoutID = checkingNode->fanouts[j];
//	        		//cout << "outnode " << outnode << endl;
                    BnetNode *fanout = getNodeFromNet(&net, fanoutID);
                    int fanoutInd = node2Index.find(fanout->name)->second;
                    auto itrm_ic1 = currentFanInList.find(fanoutInd);
                    auto itrm_ic2 = toBeDeletedNodeList.find(fanoutInd);
                    if ((itrm_ic1 == currentFanInList.end())
                        || (itrm_ic2 != toBeDeletedNodeList.end())) {
                        map<int, BnetNodeID> delete_tfi =
                                fanInNodeDirectory.at(checkingNode->name);
                        for (auto node : delete_tfi) toBeDeletedNodeList.insert(node);
/*
                         //cout << "delete_list: " << endl;
						for(itrm_ic3 = delete_list.begin(); itrm_ic3 != delete_list.end(); itrm_ic3++)
							//cout << itrm_ic3->second << " ";
						//cout << endl;	*/
                        break;
                    }
                }
            }
        }

        auto itrm_cs1 = MffcDirectory.find(mffcOutNodeID);
        for(auto node : toBeDeletedNodeList) {
        	BnetNodeID dnode = node.second;
            auto itrs = itrm_cs1->second.find(dnode);
            if(itrs != itrm_cs1->second.end())
                itrm_cs1->second.erase(itrs);
        }
  	}
}


/*find_insig_MFFC()*/
void findMffcInputNodes(BnetNetwork *net,
                        const map<BnetNodeID, set<BnetNodeID> > &MFFC_set,
                        map<BnetNodeID, set<BnetNodeID> > &insig_MFFC) {
    BnetNode *nd = net->nodes;
	while(nd != NULL) {
        // FIXME : Why does output node don't have MFFCs?
    	//if(nd->type == BNET_OUTPUT_NODE) {
    	//	nd = nd->next;
        //    continue;
    	//}

        //cout << endl << "cnode: " << nd->name << endl;
        set<BnetNodeID> cMFFC = MFFC_set.at(nd->name);
        //cout << "cMFFC: ";
        //for(auto node : cMFFC) cout << node << " ";
        //cout << endl;

        // No useable MFFC
        if(cMFFC.size() == 1) {
        	nd = nd->next;
            continue;     
        }   

		set<BnetNodeID> in_sig;
        for(auto itrs = cMFFC.begin(); itrs != cMFFC.end(); itrs++) {
            BnetNodeID cnode = *itrs;
//        	//cout << "MFFC node: " << cnode << endl;
        	BnetNode *tmp = getNodeFromNet(&net, cnode);
        	for(int i = 0; i < tmp->ninp; i++) {
        		BnetNodeID innodeID = tmp->inputs[i];
//        		//cout << "innode: " << innode << endl;
        		BnetNode* innode = getNodeFromNet(&net, innodeID);
        		if(innode->type == BNET_INPUT_NODE)
        			in_sig.insert(innode->name);
        		else {
        			auto itrs1 = cMFFC.find(innode->name);
        			if(itrs1 == cMFFC.end())
                        in_sig.insert(innode->name);
        		}
        	}
        }
        
        //cout << "this insig: " << endl;
        //for(auto itrm_cc = in_sig.begin(); itrm_cc != in_sig.end(); itrm_cc++)
        //	cout << *itrm_cc << " ";
        //cout << endl;
        insig_MFFC.insert(std::make_pair(nd->name, in_sig));
        nd = nd->next;
    }
}



void write_MFFC(BnetNetwork *net,
				ostream &fout,
                BnetNodeID cnode,
                set<BnetNodeID> &Mffc,
                set<BnetNodeID> &inputNodes) {

    BnetNode *nd = getNodeFromNet(&net, cnode);

    fout << ".model bigNode" << endl;
    fout << ".inputs ";
    for(auto nodeId : inputNodes) fout << nodeId  << " ";
	fout << endl;
    fout << ".outputs " << cnode << endl;

    for(auto itrs = Mffc.begin(); itrs != Mffc.end(); itrs++) {
    	BnetNodeID node = *itrs;
        BnetNode *tmp = getNodeFromNet(&net, node);
        if(tmp->type == BNET_INPUT_NODE) continue;
        fout << ".names "; 
        for(int i = 0 ; i < tmp->ninp; i++)
        	fout << tmp->inputs[i] << " ";
        fout << node << endl;       
 		BnetTabline *t = tmp->f;
		while(t != NULL) {
			if(t->values != NULL) 
				fout << t->values << " " << 1 - tmp->polarity << endl;
			else
				fout << 1 - tmp->polarity << endl;
			t = t->next;
		}               
    }
    fout << ".end" << endl;
}
