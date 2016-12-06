//
// Created by tripack on 16-11-8.
//

#ifndef VE490_INTERFACE_H
#define VE490_INTERFACE_H

#include "exception.h"
#include <string>
#include <vector>

#include <truth_table.h>

std::string getMFFC(std::string infile,
                    int minInput,
                    int maxInput);

TruthTable getTruthTableFromBlif(const std::string& blifContent);

#endif //VE490_INTERFACE_H
