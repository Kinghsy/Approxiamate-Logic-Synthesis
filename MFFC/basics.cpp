#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <climits>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <cassert>
#include <ctime>
#include <sys/timeb.h>
#include "head/queue.h"
#include "head/bnet.h"

using namespace std;

/*
functions in this file:
1. void topSort(BnetNetwork **net, vector<char*> &sort_list)
2. void get_MFFC(BnetNetwork *net, vector<char*> &sort_list, map<char*, set<char*> > &TFI_set_char, map<char*, set<char*> > &MFFC_set)
3. void find_insig_MFFC(BnetNetwork *net, map<char*, set<char*> > &MFFC_set, multimap<double, char*> &sp_MFFC, map<char*, map<char*, char*> > &insig_MFFC)
*/

//Global variables and external variables


/*topSort()*/
void topSort(BnetNetwork **net, vector<char*> &sort_list)
{
	BnetNode *nd, *auxnd, *tmp;
    queue sort_queue;
    map<char*, int> indegree_list; 
    map<char*, int>::iterator itrm_si;
    
    nd = (*net)->nodes;
    int i = 0;
    string name_str;
    while (nd != NULL) 
    {
    	indegree_list.insert(pair<char*, int>(nd->name, nd->ninp));
    	if(nd->ninp == 0)
    		sort_queue.push(nd->name);
		nd = nd->next;
    }
    
    while(!sort_queue.empty())
    {
    	sort_queue.traverse();
        char *pnode = sort_queue.pop(); 
//        cout << endl << "pop: " << pnode << endl;
        st_lookup((*net)->hash, pnode, &tmp);
        sort_list.push_back(tmp->name);
        for(int i = 0; i < tmp->nfo; i++)
        {        	
            char *outnode = tmp->fanouts[i];
            st_lookup((*net)->hash, outnode, &auxnd);
//            cout << "output: " << auxnd->name;
            itrm_si = indegree_list.find(auxnd->name);
            if(itrm_si == indegree_list.end())
            {
            	cout << " not exist!";
            	exit(1);
            }
            else 
            {
            	itrm_si->second = itrm_si->second - 1;
//            	cout << ", num: " << itrm_si->second << endl;
            	if(itrm_si->second == 0)
            	{
                	sort_queue.push(auxnd->name);
//                	cout << "node " << auxnd->name << " is enqueued. " << endl;
            	}
            }
        }         
    }     
}


/*MFFC()*/
void get_MFFC(BnetNetwork *net, vector<char*> &sort_list, map<char*, set<char*> > &TFI_set_char, map<char*, set<char*> > &MFFC_set)
{
	set<char*>::iterator itrs, itrs1, itrs2;
	BnetNode *nd, *tmp, *auxnd;	
	map<char*, set<char*> >::iterator itrm_cs, itrm_cs1;	
	map<char*, map<int, char*> >::iterator itrm_cm, itrm_cm1;
	map<char*, map<int, char*> > TFI_set;
	map<int, char*> ind_sort;
	map<char*, int> sort_ind; 
	map<int, char*>::iterator itrm_ic, itrm_ic1, itrm_ic2, itrm_ic3;
	map<char*, int>::iterator itrm_ci;
	for(int i = 0; i < sort_list.size(); i++)
	{
		ind_sort.insert(pair<int, char*>(i, sort_list[i]));
		sort_ind.insert(pair<char*, int>(sort_list[i], i));
	}
/*	cout << "ind_sort" << endl;
	for(itrm_ic = ind_sort.begin(); itrm_ic != ind_sort.end(); itrm_ic++)
		cout << itrm_ic->first << ", " << itrm_ic->second << endl;
*/	
	map<int, char*> this_TFI;
	map<int, char*> pred_TFI;
	set<char*> this_TFI_set;
	for(itrm_ic = ind_sort.begin(); itrm_ic != ind_sort.end(); itrm_ic++)
    {
        char *cnode = itrm_ic->second;
        st_lookup(net->hash, cnode, &nd);
		int cind = itrm_ic->first;
//		cout << "cnode = " << cnode << endl;
		
        this_TFI.clear();
        this_TFI.insert(pair<int, char*>(cind, nd->name));
        for(int j = 0; j < nd->ninp; j++)
        {
        	char *innode = nd->inputs[j];
        	st_lookup(net->hash, innode, &tmp);  	
        	itrm_cm = TFI_set.find(tmp->name);
        	if(itrm_cm == TFI_set.end())
        	{
        		cout << "this innode is not in TFI_set" << endl;
        		exit(1);
        	}
        	pred_TFI = itrm_cm->second;
			for(itrm_ic1 = pred_TFI.begin(); itrm_ic1 != pred_TFI.end(); itrm_ic1++)
        	{
        		st_lookup(net->hash, itrm_ic1->second, &auxnd);  
        		this_TFI.insert(pair<int, char*>(itrm_ic1->first, auxnd->name));
        	}
        }
        TFI_set.insert(pair<char*, map<int, char*> >(cnode, this_TFI));
        this_TFI_set.clear();
        for(itrm_ic1 = this_TFI.begin(); itrm_ic1 != this_TFI.end(); itrm_ic1++)
        {
        	char *node = itrm_ic1->second;
        	st_lookup(net->hash, node, &tmp);  	
        	this_TFI_set.insert(tmp->name);
        }
        MFFC_set.insert(pair<char*, set<char*> >(nd->name, this_TFI_set));
        TFI_set_char.insert(pair<char*, set<char*> >(nd->name, this_TFI_set));
    }
    
    
    for(int i = 0; i < sort_list.size(); i++)
    {
        char *cnode = sort_list[i];
        st_lookup(net->hash, cnode, &nd);
        itrm_cm = TFI_set.find(nd->name);
        map<int, char*> current_MFFC = itrm_cm->second;
        map<int, char*> delete_list;
        itrm_ic = current_MFFC.end();
        itrm_ic--;
        if(current_MFFC.size() > 1)
	    {
	        while(1)
	        {
	        	char *mnode = itrm_ic->second;        	
	        	if(!strcmp(cnode, mnode))
	        	{
	        		itrm_ic--;
	        		if(itrm_ic == current_MFFC.begin())
	        			break;
	        		else
	        			continue;
	        	}
//	        	cout << "checking node " << mnode << endl;
	        	st_lookup(net->hash, mnode, &tmp);
	        	for(int j = 0; j < tmp->nfo; j++)
	        	{
	        		char *outnode = tmp->fanouts[j];
//	        		cout << "outnode " << outnode << endl;
	        		st_lookup(net->hash, outnode, &auxnd);
	        		itrm_ci = sort_ind.find(auxnd->name);
	        		int ind = itrm_ci->second;
	        		itrm_ic1 = current_MFFC.find(ind);
	        		itrm_ic2 = delete_list.find(ind);
	        		if((itrm_ic1 == current_MFFC.end()) || (itrm_ic2 != delete_list.end()))
	        		{
//	        			cout << "node " << mnode << " is to deleted." << endl;
						itrm_cm1 = TFI_set.find(tmp->name);
						if(itrm_cm1 == TFI_set.end())
						{
							cout << "current fanout node " << outnode << " is not in TFI_set" << endl;
							exit(1);
						}
	        			map<int, char*> delete_tfi = itrm_cm1->second;
	        			for(itrm_ic3 = delete_tfi.begin(); itrm_ic3 != delete_tfi.end(); itrm_ic3++)
	        				delete_list.insert(pair<int, char*>(itrm_ic3->first, itrm_ic3->second));
/*	        			cout << "delete_list: " << endl;
						for(itrm_ic3 = delete_list.begin(); itrm_ic3 != delete_list.end(); itrm_ic3++)
							cout << itrm_ic3->second << " ";
						cout << endl;	        			
*/
						break;
	        		}	        		
	        	}
	        	itrm_ic--;
	        	if(itrm_ic == current_MFFC.begin())
	        		break;
	        }
	    }
        //deal with the first element
        itrm_ic = current_MFFC.begin();
        char *mnode = itrm_ic->second;        	
        if(strcmp(cnode, mnode))
		{
//			cout << "checking node " << mnode << endl;
	        st_lookup(net->hash, mnode, &tmp);
	        for(int j = 0; j < tmp->nfo; j++)
	        {
	        	char *outnode = tmp->fanouts[j];
//	        	cout << "outnode " << outnode << endl;
	        	st_lookup(net->hash, outnode, &auxnd);
	        	itrm_ci = sort_ind.find(auxnd->name);
	        	int ind = itrm_ci->second;
	        	itrm_ic1 = current_MFFC.find(ind);
	        	itrm_ic2 = delete_list.find(ind);
	        	if((itrm_ic1 == current_MFFC.end()) || (itrm_ic2 != delete_list.end()))
	        	{
//	        		cout << "node " << mnode << " is to deleted." << endl;
					itrm_cm1 = TFI_set.find(tmp->name);
					if(itrm_cm1 == TFI_set.end())
					{
						cout << "current fanout node " << outnode << " is not in TFI_set" << endl;
						exit(1);
					}
	        		map<int, char*> delete_tfi = itrm_cm1->second;
	        		for(itrm_ic3 = delete_tfi.begin(); itrm_ic3 != delete_tfi.end(); itrm_ic3++)
	        			delete_list.insert(pair<int, char*>(itrm_ic3->first, itrm_ic3->second));
/*	        		cout << "delete_list: " << endl;
					for(itrm_ic3 = delete_list.begin(); itrm_ic3 != delete_list.end(); itrm_ic3++)
						cout << itrm_ic3->second << " ";
					cout << endl;	
*/
					break;      
	        	}	        	
	        }
	    }

        itrm_cs1 = MFFC_set.find(cnode);
        for(itrm_ic = delete_list.begin(); itrm_ic != delete_list.end(); itrm_ic++)
        {
        	char *dnode = itrm_ic->second;
            itrs = itrm_cs1->second.find(dnode);
            if(itrs != itrm_cs1->second.end())
                itrm_cs1->second.erase(itrs);
        }
  	}                  
}


/*find_insig_MFFC()*/
void find_insig_MFFC(BnetNetwork *net, map<char*, set<char*> > &MFFC_set, map<char*, map<char*, char*> > &insig_MFFC)
{ 	
	map<char*, set<char*> >::iterator itrm_cs;
    map<char*, char*>::iterator itrm_cc; 
    map<char*, map<char*, char*> >::iterator itrm_cm;
    set<char*>::iterator itrs, itrs1;
    BnetNode *nd, *tmp, *auxnd;

	nd = net->nodes;
	while(nd != NULL)
    {
    	if(nd->type == BNET_OUTPUT_NODE)
    	{
    		nd = nd->next;
            continue;   
    	}
    	cout << endl << "cnode: " << nd->name << endl;
 		itrm_cs = MFFC_set.find(nd->name);
        set<char*> cMFFC = itrm_cs->second;
        cout << "cMFFC: ";
        for(itrs = cMFFC.begin(); itrs != cMFFC.end(); itrs++)
        	cout << *itrs << " ";
        cout << endl;
               
        if(cMFFC.size() == 1)
        {
        	nd = nd->next;
            continue;     
        }   

		map<char*, char*> in_sig;
        for(itrs = cMFFC.begin(); itrs != cMFFC.end(); itrs++)
        {
        	char *cnode = *itrs;
//        	cout << "MFFC node: " << cnode << endl;
        	st_lookup(net->hash, cnode, &tmp);
        	for(int i = 0; i < tmp->ninp; i++)
        	{
        		char *innode = tmp->inputs[i];
//        		cout << "innode: " << innode << endl;
        		st_lookup(net->hash, innode, &auxnd);
        		if(auxnd->type == BNET_INPUT_NODE)
        			in_sig.insert(pair<char*, char*>(auxnd->name, auxnd->name));
        		else
        		{
        			itrs1 = cMFFC.find(auxnd->name);
        			if(itrs1 == cMFFC.end())
        				in_sig.insert(pair<char*, char*>(auxnd->name, auxnd->name));
        		}
        	}
        }
        
        cout << "this insig: " << endl;
        for(itrm_cc = in_sig.begin(); itrm_cc != in_sig.end(); itrm_cc++)
        	cout << itrm_cc->first << " ";
        cout << endl;
        insig_MFFC.insert(pair<char*, map<char*, char*> >(nd->name, in_sig));
        nd = nd->next;
    }
}



void write_MFFC(BnetNetwork *net, ofstream &fout, char *cnode, set<char*> &cMFFC, map<char*, char*> &in_sig)
{
	set<char*>::iterator itrs;
	map<char*, char*>::iterator itrm_cc;

	BnetNode *nd, *tmp;
	st_lookup(net->hash, cnode, &nd);

    fout << ".model bigNode" << endl;
    fout << ".inputs ";
    for(itrm_cc = in_sig.begin(); itrm_cc != in_sig.end(); itrm_cc++)
        fout << itrm_cc->first << " ";
	fout << endl;
    fout << ".outputs " << cnode << endl;
    
    for(itrs = cMFFC.begin(); itrs != cMFFC.end(); itrs++)
    {
    	char *node = *itrs;
        st_lookup(net->hash, node, &tmp);
        if(tmp->type == BNET_INPUT_NODE)
            continue;
        fout << ".names "; 
        for(int i = 0 ; i < tmp->ninp; i++)
        	fout << tmp->inputs[i] << " ";
        fout << node << endl;       
 		BnetTabline *t = tmp->f;
		while(t != NULL) 
		{
			if(t->values != NULL) 
				fout << t->values << " " << 1 - tmp->polarity << endl;
			else
				fout << 1 - tmp->polarity << endl;
		
			t = t->next;
		}               
    }    
    
    fout << ".end" << endl;

}
