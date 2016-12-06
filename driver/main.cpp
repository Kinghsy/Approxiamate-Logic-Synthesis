//
// Created by tripack on 16-11-18.
//
#include "../MFFC/interface.h"

#include <string>
#include <vector>
#include <iostream>
#include <truth_table.h>

using std::string;
using std::vector;
using std::cout;

int main(int argc, char* agrv[]) {
    string s = getMFFC("c880_new.blif", 4, 6);
    TruthTable v = getTruthTableFromBlif(s);
    v.print();
}
