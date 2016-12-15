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
//    string s = getMFFC("c880_new.blif", 4, 6);
//    ofstream ofile("file.txt");
//    ofile << s;
//    ofile.close();
    BlifBooleanNet net1("bignode.blif");
    BlifBooleanNet net2("bignode_org.blif");
    BlifCompareResult r = sampleCompareBlifs(net1, net2, 60);
    cout << r.errorCount << "/" << r.nSamples;
}
