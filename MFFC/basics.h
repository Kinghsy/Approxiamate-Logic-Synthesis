#ifndef BASICS_H
#define BASICS_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include "bnet.h"

using namespace std;

typedef std::string BnetNodeID;

vector<BnetNodeID> topSort(BnetNetwork **net);

void get_MFFC(BnetNetwork *net,
              const vector<BnetNodeID> &sort_list,
              map<BnetNodeID, set<BnetNodeID> > &TFI_set_char,
              map<BnetNodeID, set<BnetNodeID> > &MFFC_set);

void findMffcInputNodes(BnetNetwork *net,
                     const map<BnetNodeID, set<BnetNodeID> > &MFFC_set,
                     map<BnetNodeID, set<BnetNodeID> > &insig_MFFC);

void write_MFFC(BnetNetwork *net,
                ostream &fout,
                BnetNodeID cnode,
                set<BnetNodeID> &Mffc,
                set<BnetNodeID> &inputNodes);

BnetNode* getNodeFromNet(BnetNetwork **net, const BnetNodeID& nodeID);

#endif
