//
// Created by tripack on 17-3-1.
//

#include <string>
#include <iostream>
#include "../../MFFC/interface.h"

using std::string;
using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Missing input file name" << endl;
        return 1;
    }
    const string file(argv[1]);
    BlifBooleanNet net(file);
    cout << "BLIF information program." << endl;
    cout << "File: " << net.name() << endl;
    cout << "Circuit Name: " << net.netName() << endl;
    cout << "nInputs/nOutputs: " << net.nInputs() << "/" << net.nOutputs() << endl;
    cout << "# of Nodes/Gates: " << net.nodeCount() << "/" << net.gateCount() << endl;

    const auto& s = net.getFFC();

    for (auto& np : s) {
        int po = np.second.nodeSet.size() - np.second.inputNode.size() + 1;
        //if (po == 0) continue;
        if (np.second.inputNode.size() > 6) continue;
        if (np.first.size() <= 1) cout << " ";
        cout << "Node = " << np.first << "\t";
        cout << " : Supp = " << np.second.inputNode.size() << "\t";
        cout << " : Cone = " << np.second.nodeSet.size() << "\t";
        cout << "(";
        for (auto& n : np.second.nodeSet) {
            cout << n << ", ";
        }
        cout << ")" << endl;
    }
    return 0;
}
