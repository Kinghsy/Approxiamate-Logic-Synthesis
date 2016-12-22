#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>
#include <ctime>
#include <sys/timeb.h>
#include "basics.h"
#include "bnet.h"


using namespace std;

void searchMFFC(BnetNetwork *net,
                ostream &ostr,
                int num_input_min,
                int num_input_max)
{
    //variables
    struct timeb startTime, endTime; 
    BnetNode *nd;

    //Topological sort
    //cout << "****************************" << endl;
    //cout << "step1. topSort: " << endl;
    vector<BnetNodeID> sort_list = topSort(&net);
    //vector<char*> raw_sort_list;
    //for (auto it = sort_list.begin(); it != sort_list.end(); it++)
    //    raw_sort_list.push_back((char*)(it->c_str()));
    //for (auto str : sort_list) cout << str << " ";
    //cout <<endl;
    
    //Get MFFC for each node and find input signals for each MFFC
    //cout << "****************************" << endl;
    //cout << "step2. get_MFFC & find_insig_MFFC: " << endl;	
    map<BnetNodeID, set<BnetNodeID> > TFI_set, MFFC_set;
    get_MFFC(net, sort_list, TFI_set, MFFC_set);
    //auto nodeMffc = MFFC_set.at("391");
    //cout << nodeMffc.size() << endl;
    //for (auto node : nodeMffc) cout << node << endl;
    map<BnetNodeID, set<BnetNodeID> > insig_MFFC;
    findMffcInputNodes(net, MFFC_set, insig_MFFC);
    //nodeMffc = MFFC_set.at("391");
    //cout << nodeMffc.size() << endl;
    //for (auto node : nodeMffc) cout << node << endl;

 	//Get MFFC for each node and find input signals for each MFFC
    //cout << "****************************" << endl;
    //cout << "step3. print MFFC to the output file: " << endl;

    int LUTs_saved_number_max = 0;
    int candidate_MFFC_index = 0;
    int count_2input_LUTs;
    int LUTs_potentially_saved;

	// find the candidate MFFC
    BnetNodeID      candidateID;
    set<BnetNodeID> candidateMffc;
    set<BnetNodeID> candidateMffcInsig;
    for (auto node : MFFC_set) {
        BnetNodeID searchNode = node.first;
        set<BnetNodeID> nodeMffc = node.second;
        if (nodeMffc.size() == 1) continue;
        //cout << nodeMffc.size() << endl;
        //for (auto node : nodeMffc) cout << node << endl;
        set<BnetNodeID> nodeInput = insig_MFFC.at(searchNode);

        if (nodeInput.size() <= num_input_max && nodeInput.size() >= num_input_min) {
            count_2input_LUTs = nodeMffc.size();
            LUTs_potentially_saved = count_2input_LUTs - (nodeInput.size() - 1);
            //cout << "LUTs_potentially_saved = " << LUTs_potentially_saved << endl;
            if (LUTs_potentially_saved > LUTs_saved_number_max) {
                LUTs_saved_number_max = LUTs_potentially_saved;
                candidateMffc = nodeMffc;
                candidateMffcInsig = nodeInput;
                candidateID = searchNode;
            }
        }
    }

	////cout << "debug1" << endl;
	////cout << "debug2" << endl;
	write_MFFC(net, ostr, candidateID, candidateMffc, candidateMffcInsig);
	////cout << "debug3" << endl;
	/*
	
	for(itrm_cs = MFFC_set.begin(); itrm_cs != MFFC_set.end(); itrm_cs++)
	{
		char *cnode = itrm_cs->first;
		set<char*> this_MFFC = itrm_cs->second;
		if(this_MFFC.size() == 1)
			continue;
			
		st_lookup(net->hash, cnode, &nd);
		itrm_cm = insig_MFFC.find(nd->name);
		map<char*, char*> insig = itrm_cm->second;
			
		if(itrm_cs == itrm_candidate_MFFC){
			//fout2 << "node: " << cnode << endl;	    	
		    	write_MFFC(net, fout2, cnode, this_MFFC, insig);
		}
		else{	
		    	//fout << "node: " << cnode << endl;	    	
		    	write_MFFC(net, fout, cnode, this_MFFC, insig);
		}
	}
	*/
	//cout << "candidate_MFFC_index = " << candidate_MFFC_index << endl;
	//cout << "LUTs_saved_number_max = " << LUTs_saved_number_max << endl;
}


