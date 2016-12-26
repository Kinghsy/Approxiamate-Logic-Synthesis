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

    rawData.getMFFC(4, 5);
    cout << endl;
    std::vector<string> toplist = rawData.topologicalSort();

    for (auto& s : toplist) cout << s << " ";
    cout << endl;
    std::vector<string> top2 = rawData.topologicalSort();
    for (auto& s : top2) cout << s << " ";
    cout << endl;
}
