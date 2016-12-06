#ifndef _PRINT_MFFC_TRUTHTABLE_H
#define _PRINT_MFFC_TRUTHTABLE_H

#include <string>
#include "ntr.h"
#include "bnet.h"
#include "../lib/cudd-2.5.0/include/cuddInt.h"
#include "cudd_build_v2.h"

#include <truth_table.h>

TruthTable getTruthTable(string &infile);


#endif
