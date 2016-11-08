//
// Created by king on 16-11-8.
//
#include <iostream>
#include <cstdlib>
#include <memory>

#include "boolean_function.h"

using namespace std;

int main() {

    int portName[]={1,0,1,0,1,1};
    int portSize=6;
    int truthTable[]={0,1,0,0, 1,0,1,0, 1,1,1,0, 1,0,1,0};

    unique_ptr<BooleanFunction> bf(new BooleanFunction(portName, portSize, truthTable));
    int part1[]={0,0,0,0,0,1};
    int part2[]={1,0,1,0,1,0};
    cout << "right now." << endl;
    unique_ptr<KMap> kmap1=std::move(bf->getKMap(part1, part2));
    for (int i = 0; i < kmap1->height; ++i) {
        for (int j = 0; j < kmap1->width; ++j) {
            cout << kmap1->table[i][j] << " ";
        }
        cout << endl;
    }

}