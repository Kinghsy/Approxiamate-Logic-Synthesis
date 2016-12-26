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
}
