#include "cuddObj.hh"

#include <iostream>
#include <fstream>

using namespace std;


/**
 * Uses restrict to test our function over a range of values
 */
void test(DdManager* manager, DdNode **node)
{
	DdNode *x0 = Cudd_bddIthVar(manager,0);
	DdNode *x1 = Cudd_bddIthVar(manager,1);

	const int SIZE=4;
	DdNode* restrictBy[SIZE];
	DdNode* testSum[SIZE];
	DdNode* testCarry[SIZE];

	// Restrict by the following assignments
	restrictBy[0] = Cudd_bddAnd(manager,Cudd_Not(x0),Cudd_Not(x1));  // x1=0 and x2=0
	restrictBy[1] = Cudd_bddAnd(manager,Cudd_Not(x0),x1);			// x1=0 and x2=1
	restrictBy[2] = Cudd_bddAnd(manager,x0,Cudd_Not(x1));			// x1=1 and x2=0
	restrictBy[3] = Cudd_bddAnd(manager,x0,x1);						// x1=1 and x2=1

	for (int i=0;i<SIZE;i++) {

		Cudd_Ref(restrictBy[i]);  // Reference restrictBy

		// Now restrict by the new functions
		testSum[i]   = Cudd_bddRestrict(manager,node[0],restrictBy[i]);
		testCarry[i] = Cudd_bddRestrict(manager,node[1],restrictBy[i]);

		Cudd_RecursiveDeref(manager,restrictBy[i]); // clean up restrictBy
	}

	cerr << "(x1=0, x2=0): sum = " << 1-Cudd_IsComplement(testSum[0]) << " Carry = " << 1-Cudd_IsComplement(testCarry[0]) <<  endl
		 << "(x1=0, x2=1): sum = " << 1-Cudd_IsComplement(testSum[1]) << " Carry = " << 1-Cudd_IsComplement(testCarry[1]) <<  endl
		 << "(x1=1, x2=0): sum = " << 1-Cudd_IsComplement(testSum[2]) << " Carry = " << 1-Cudd_IsComplement(testCarry[2]) <<  endl
		 << "(x1=1, x2=1): sum = " << 1-Cudd_IsComplement(testSum[3]) << " Carry = " << 1-Cudd_IsComplement(testCarry[3]) <<  endl;

	for (int i=0;i<SIZE;i++) {
		Cudd_RecursiveDeref(manager,testSum[i]);
		Cudd_RecursiveDeref(manager,testCarry[i]);
	}
}

/**
 * This takes an array of 2 output nodes and prints them to a dot file
 */
void toDot(DdManager *manager, DdNode **outputs)
{
	char **inputNames = new char*[2];   // Label the two input nodes
	inputNames[0] = new char[3];
	inputNames[1] = new char[3];
	inputNames[0] = "x1";
	inputNames[1] = "x2";

	char **outputNames = new char*[2];  // Label the two output nodes
	outputNames[0] = new char[4];
	outputNames[1] = new char[6];
	strcpy(outputNames[0],"sum");
	strcpy(outputNames[1],"carry");

	FILE *f = fopen("./half_adder.dot","w");

	// manager: The cudd manager
	// 2      : The number of outputs
	// outputs: An array of outputs (DdNode*)
	// inames : Maps input nodes to their names
	// onames : Maps output nodes to their names
	// f      : The file to write to

	Cudd_DumpDot(manager, 2, outputs,inputNames, outputNames,f);
}

DdNode** createHalfAdderBDD(DdManager *manager)
{
	// Construct the Half Adder Circuit
	DdNode *x0 = Cudd_bddIthVar(manager,0);
	DdNode *x1 = Cudd_bddIthVar(manager,1);

	DdNode *and1 = Cudd_bddAnd(manager,x0,Cudd_Not(x1));
	Cudd_Ref(and1);

	DdNode *and2 = Cudd_bddAnd(manager,Cudd_Not(x0),x1);
	Cudd_Ref(and2);

	DdNode *sum = Cudd_bddOr(manager,and1,and2);
	Cudd_Ref(sum);

	Cudd_RecursiveDeref(manager,and1);
	Cudd_RecursiveDeref(manager,and2);

	DdNode *carry = Cudd_bddAnd(manager,x0,x1);
	Cudd_Ref(carry);

	DdNode **outputs = new DdNode*[2];
	outputs[0] = sum;
	outputs[1] = carry;

	return outputs;
}


int main()
{
	// Initialize the manager
	// 0                : Initial number of BDD variables
	// 0                : Initial number of ZDD variables
	// CUDD_UNIQUE_SLOTS: Initial size of the unique tables (256)
	// CUDD_CACHE_SLOTS : Initial size of the cache (262144)
	// 0                : target maximum memory (0 means unlimited)
	DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

	// Create our Bdd
	DdNode **outputs = createHalfAdderBDD(manager);

	// Test BDD
	//testBDD(manager,outputs);

	// Write the BDD to a dot file
	toDot(manager,outputs);

	test(manager,outputs);
	return 0;
}
