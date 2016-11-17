#include <stdio.h>

#include "cudd.h"
#include "util.h"
#include "cuddInt.h"

int main()
{
	DdManager *manager;
	DdNode *f, *var, *temp;
	int i;
	
	manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	f = Cudd_ReadOne(manager);
	Cudd_Ref(f);
	for(i=3;i>=0;i--)
	{
		var = Cudd_bddIthVar(manager, i);
		temp = Cudd_bddAnd(manager, Cudd_Not(var), f);
		Cudd_Ref(temp);
		Cudd_RecursiveDeref(manager, f);
		f = temp;
	}

	Cudd_Quit(manager);
	return 0;
}
