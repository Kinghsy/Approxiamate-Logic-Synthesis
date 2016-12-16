//
// Created by tripack on 16-11-18.
//

#include <string>
#include <vector>
#include <iostream>
#include <truth_table.h>
#include <fstream>
#include <cstdlib>

#include "../MFFC/interface.h"
#include "../MFFC/err_asses.h"

using std::string;
using std::vector;
using std::cout;

int main(int argc, char* agrv[]) {
    BlifBooleanNet net0("file.txt");
    cout << net0.gateCount() << "/" << net0.nodeCount() << endl;
    BlifBooleanNet netx("c880.blif");
    BlifBooleanNet mffc = netx.getMFFC(4, 6);
    mffc.truthTable().print();
    BlifBooleanNet net1("c880.blif");
    BlifBooleanNet net2("c880_new.blif");
    BlifCompareResult r = sampleCompareBlifs(net1, net2);
    cout << r.errorCount << "/" << r.nSamples;
}
