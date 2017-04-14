//
// Created by king on 16-12-17.
//

#ifndef VE490_SEARCH_METHOD_CORE_H
#define VE490_SEARCH_METHOD_CORE_H

#include "boolean_function.h"
#include "conts.h"
#include "search.h"

#include <truth_table.h>

TruthTable calculApproxTruthTable(TruthTable &truthTab, int ActivedMode);
TruthTable calculApproxTruthTable_BranchAndBound(TruthTable &truthTab);
TruthTable calculApproxTruthTable_Full(TruthTable &truthTab);
TruthTable calculApproxTruthTable_BFS(TruthTable &truthTab);

TruthTable writeApproxBlifFileByTruthTable(TruthTable &truthTable, std::string BlifFileName, int ActivedMode);
TruthTable writeApproxBlifFileByTruthTable_BranchAndBound(TruthTable &truthTab, std::string BlifFileName);
TruthTable writeApproxBlifFileByTruthTable_BFS(TruthTable &truthTab, std::string BlifFileName);
TruthTable writeApproxBlifFileByTruthTable_Full(TruthTable &truthTab, std::string BlifFileName);

#endif //VE490_SEARCH_METHOD_CORE_H
