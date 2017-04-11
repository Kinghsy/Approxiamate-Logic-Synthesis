//
// Created by king on 17-4-12.
//

#include <vector>
#include <map>

#include "const.h"
#include "truth_table.h"
#include "kmap.h"
#include "boolean_function.h"

using std::string;
using std::vector;

Kmap::Kmap(
        const BooleanFunction &BF,
        std::vector<std::string> heightName,
        std::vector<std::string> widthName) {

    height = heightName.size();
    width = widthName.size();
    //FIXME

}

Kmap::~Kmap() {
    while (kmap.size()>0)
        kmap.pop_back();
}

bool Kmap::operator==(const Kmap &initKmap) {
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (kmap[i][j] != initKmap.kmap[i][j]) return false;
    return true;
}

size_t Kmap::operator^(const Kmap &initKmap) {
    size_t count = 0;
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            count += (kmap[i][j] != initKmap.kmap[i][j]) ? 1:0 ;
    return count;
}

TruthTable Kmap::operator[](const int &i) {
    return kmap[i];
}

int Kmap::getHeight() {
    return height;
}

int Kmap::getWidth() {
    return width;
}

string Kmap::getHeightName(const int &i) {
    return heightName[i];
}

string Kmap::getWidthName(const int &j) {
    return widthName[j];
}

BestApprox Kmap::divide(const SimulationResult &simData) {

}
