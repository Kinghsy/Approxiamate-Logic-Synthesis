
#include "print_truth_table.h"

#include <vector>
#include <truth_table.h>

typedef long long unsigned int ulli;


ulli power2(int power);
void getBits(ulli n, int* vec, int digit);

TruthTable getTruthTable(string &infile)
{
	DdManager *ddmanager = NULL;		/* pointer to DD manager */
	FILE *fp;
	fp = fopen(infile.c_str(), "r");
    if (fp == NULL) assert(0);
	BnetNetwork *net = Bnet_ReadNetwork(fp);

	/* Initialize manager. We start with 0 variables, because
	** Ntr_buildDDs will create new variables rather than using
	** whatever already exists.
	*/
	//dd = startCudd(option,net->ninputs);
	//if (dd == NULL) { exit(2); }

	ddmanager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
 	if (ddmanager == NULL) exit(-2);
    	cudd_build_v2(net, &ddmanager, infile.c_str(), BNET_GLOBAL_DD);

	// find the DdNode of the output node 
	BnetNode *auxnd;
	st_lookup(net->hash,net->outputs[0],&auxnd);
	DdNode *ddnode_pt = auxnd->dd;

    int nInputs = net->ninputs;
    vector<int> out;

    ulli max = power2(nInputs);
    int *ivec = new int[nInputs];
    for (ulli i = 0; i < max ; ++i) {
        getBits(i, ivec, nInputs);
        int n =
                (Cudd_ReadOne(ddmanager) == Cudd_Eval(ddmanager, ddnode_pt, ivec));
        out.push_back(n);
    }
    delete[] ivec;

    TruthTable table(nInputs, out);
    for (int j = 0; j < nInputs; ++j) {
        table.getName(j) = net->inputs[j];
    }
    
	Bnet_FreeNetwork_Bdd(net, ddmanager);
	Cudd_Quit(ddmanager);

    return table;
}



