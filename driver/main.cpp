//
// Created by tripack on 16-11-18.
//
#include "../MFFC/interface.h"

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;

int main(int argc, char* agrv[]) {
    string s = getMFFC("c880_new.blif", 4, 6);
    vector<int> v = getTruthTableFromBlif(s);
    for (auto i : v) {
        cout << i << " ";
    }
}
