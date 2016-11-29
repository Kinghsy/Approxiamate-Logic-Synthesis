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

vector<char*> topSort(BnetNetwork **net);

void get_MFFC(BnetNetwork *net,
              vector<char*> &sort_list,
              map<char*, set<char*> > &TFI_set_char,
              map<char*, set<char*> > &MFFC_set);

void find_insig_MFFC(BnetNetwork *net,
                     map<char*, set<char*> > &MFFC_set,
                     map<char*, map<char*, char*> > &insig_MFFC);

void write_MFFC(BnetNetwork *net,
                ostream &fout,
                char *cnode, set<char *> &cMFFC,
                map<char *, char *> &in_sig);


#endif
