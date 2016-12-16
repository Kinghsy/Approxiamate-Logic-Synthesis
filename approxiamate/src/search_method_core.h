//
// Created by king on 16-12-17.
//

#ifndef VE490_SEARCH_METHOD_CORE_H
#define VE490_SEARCH_METHOD_CORE_H

#include "boolean_function.h"
#include "conts.h"
#include "search.h"

#include "../../common/truth_table.h"

TruthTable calculApproxTruthTable(TruthTable &truthTab);
void writeBlifFileByGivenTruthTable(const TruthTable &truthTab);


#endif //VE490_SEARCH_METHOD_CORE_H
