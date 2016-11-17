/*
 * A simple test program for using cudd with c
 */

#include "util.h"
#include "cuddInt.h"

int 
main( )
{
 DdManager *mgr;	
 DdNode *f, *var, *tmp;
 int i; 
 
 unsigned int cacheSize, maxMemory;
 unsigned int nvars,nslots;
 
 printf("Testing cudd.......\n\n\n");
 
 nvars = 4;
 cacheSize = 127;
 maxMemory = 0;
 nslots = 0;
 
 mgr = Cudd_Init(nvars,0,nslots,cacheSize,maxMemory);
 
 f = Cudd_ReadOne(mgr);
 Cudd_Ref(f);
 for(i=3; i>=0; i--) {
  var = Cudd_bddIthVar(mgr, i);
  tmp = Cudd_bddAnd(mgr, Cudd_Not(var), f);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(mgr, f);
  f = tmp;
  }
  
  Cudd_PrintDebug(mgr, f, nvars, 4);
 
 return 0;
}
