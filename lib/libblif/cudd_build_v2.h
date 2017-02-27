#ifndef _CUDD_BUILD_V2_H
#define _CUDD_BUILD_V2_H

#include "../lib/cudd-2.5.0/cudd/cudd.h"
#include "../lib/cudd-2.5.0/cudd/cuddInt.h"
#include "bnet.h"


void cudd_build_v2(BnetNetwork *net, DdManager **dd, const char *filename, int params);

#endif
