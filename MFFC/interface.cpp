#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "bnet.h"
#include "searchMFFC.h"
#include "print_truth_table.h"

#include <truth_table.h>

using namespace std;

string getMFFC(string infile, int minInput, int maxInput) {
    FILE *fp;
    fp = fopen(infile.c_str(), "r");
    BnetNetwork *net = Bnet_ReadNetwork(fp);
    if (net == NULL) {
        cout << "Syntax error in " << infile << endl;
        exit(2);
    }
    fclose(fp);

    stringstream s;
    searchMFFC(net, s, minInput, maxInput);
    Bnet_FreeNetwork(net);
    return s.str();
}

TruthTable getTruthTableFromBlif(const string& blifContent) {
    string tFile = "temp.blif";
    ofstream ofile(tFile);
    if (!ofile.is_open()) {
        cerr << "Cannot open temp file." << endl;
        exit(1);
    }
    ofile << blifContent;
    ofile.close();

    return getTruthTable(tFile);
}
