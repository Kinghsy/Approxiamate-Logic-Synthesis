#include <iostream>

#include "interface.h"
#include "../policy/policy.h"

using namespace std;

int main(int argc, char **argv)
{
    string base="c1908";
    string exten="blif";
    string initFileName = base+"."+exten;
    BlifBooleanNet rawData(initFileName);
    cout << "name : " << rawData.name() << endl;
    cout << "gate : " << rawData.gateCount() << endl;
    cout << "inputs: " << rawData.nInputs() << endl;
    cout << "outputs: " << rawData.nOutputs() << endl;


    const auto& s = rawData.getFFC();
    /*
    for (auto& n : s) {
        if (n.first == "n139") {
            for (auto& str : n.second.nodeSet)
                cout << str << " ";
            cout << endl;
            cout << "id " << n.second.depth2Input << endl;
            cout << "od " << n.second.depth2Output << endl;
            cout << "mid " << n.second.minDepth2Input << endl;
            cout << "mod " << n.second.minDepth2Output << endl;
            for (auto& str : n.second.inputNode)
                cout << str << " ";
            cout << endl;
        }
    }*/

    for (auto& np : s) {
        int po = np.second.nodeSet.size() - np.second.inputNode.size() + 1;
        if (po == 0) continue;
        if (np.second.inputNode.size() > 6) continue;
        cout << np.first << "  \t";
        if (np.first.size() <= 1) cout << "\t";
        cout << "ip = " << np.second.inputNode.size() << "\t";
        cout << "po = " << po << "\t";
        cout << "to = " << np.second.nodeSet.size() << "\t";
        cout << "mi = " << np.second.minDepth2Input << "\t";
        cout << "mo = " << np.second.minDepth2Output << endl;
    }

    return 0;
}
