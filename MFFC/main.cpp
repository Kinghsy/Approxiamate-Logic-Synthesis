#include <iostream>

#include "interface.h"

using namespace std;

int main(int argc, char **argv)
{
    string base="c880";
    string exten="blif";
    string initFileName = base+"."+exten;
    BlifBooleanNet rawData(initFileName);
    cout << rawData.name() << " " << rawData.gateCount() << " " << rawData.nInputs() << " " << rawData.nOutputs() << endl;

    const auto& s = rawData.getFFC();

    for (auto& n : s) {
        if (n.first == "n139") {
            for (auto& str : n.second.nodeSet)
                cout << str << " ";
            cout << endl;
            cout << "id " << n.second.depth2Input << endl;
            cout << "od " << n.second.depth2Output << endl;
            for (auto& str : n.second.inputNode)
                cout << str << " ";
            cout << endl;
        }
    }

    return 0;
}
