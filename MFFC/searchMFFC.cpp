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
#include "queue.h"
#include "basics.h"
#include "bnet.h"


using namespace std;

extern int numPI_ini;

/*
functions in this file:

*/

//Global variables and external variables


void searchMFFC(BnetNetwork *net,
                ostream &ostr,
                int num_input_min,
                int num_input_max)
{
    //variables
    struct timeb startTime, endTime; 
    BnetNode *nd;
    
    //iterators
    map<char*, set<char*> >::iterator itrm_cs, itrm_candidate_MFFC;
    map<char*, char*>::iterator itrm_cc;
    map<char*, map<char*, char*> >:: iterator itrm_cm;    
    set<char*>::iterator itrs;

        
    //Topological sort
    cout << "****************************" << endl;
    cout << "step1. topSort: " << endl;	
    vector<char*> sort_list;
    topSort(&net, sort_list);
    for(int i = 0; i < sort_list.size(); i++)
    	cout << sort_list[i] << " ";
    cout << endl;
    
    //Get MFFC for each node and find input signals for each MFFC
    cout << "****************************" << endl;
    cout << "step2. get_MFFC & find_insig_MFFC: " << endl;	
    map<char*, set<char*> > TFI_set, MFFC_set;
    get_MFFC(net, sort_list, TFI_set, MFFC_set);
    map<char*, map<char*, char*> > insig_MFFC;
    find_insig_MFFC(net, MFFC_set, insig_MFFC);


 	//Get MFFC for each node and find input signals for each MFFC
    cout << "****************************" << endl;
    cout << "step3. print MFFC to the output file: " << endl;

    int LUTs_saved_number_max = 0;
    int candidate_MFFC_index = 0;
    int count_2input_LUTs;
    int LUTs_potentially_saved;

	// find the candidate MFFC
	int i = 0;
    	for(itrm_cs = MFFC_set.begin(); itrm_cs != MFFC_set.end(); itrm_cs++)
    	{
		char *cnode = itrm_cs->first;
		set<char*> this_MFFC = itrm_cs->second;
		if(this_MFFC.size() == 1)
			continue;
			
		st_lookup(net->hash, cnode, &nd);
		itrm_cm = insig_MFFC.find(nd->name);
		map<char*, char*> insig = itrm_cm->second;	

		cout << "insig.size() = " << insig.size() << endl;
		
		if(insig.size() <= num_input_max && insig.size() >= num_input_min)
		{
			count_2input_LUTs = this_MFFC.size();
			LUTs_potentially_saved = count_2input_LUTs - (insig.size() - 1);
			cout << "LUTs_potentially_saved = " << LUTs_potentially_saved << endl;
						
			if(LUTs_potentially_saved > LUTs_saved_number_max)
			{				
				LUTs_saved_number_max = LUTs_potentially_saved;
				itrm_candidate_MFFC = itrm_cs;
				candidate_MFFC_index = i;
			}
			
		}	
		i++;
	}

	//cout << "debug1" << endl;
	char *cnode = itrm_candidate_MFFC->first;
	set<char*> this_MFFC = itrm_candidate_MFFC->second;
	st_lookup(net->hash, cnode, &nd);
	itrm_cm = insig_MFFC.find(nd->name);
	map<char*, char*> insig = itrm_cm->second;
	//cout << "debug2" << endl;
	write_MFFC(net, ostr, cnode, this_MFFC, insig);
	//cout << "debug3" << endl;
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
	cout << "candidate_MFFC_index = " << candidate_MFFC_index << endl;
	cout << "LUTs_saved_number_max = " << LUTs_saved_number_max << endl;
}


