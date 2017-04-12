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
        std::vector<NodeName> heightN,
        std::vector<NodeName> widthN) {

    height = heightName.size();
    width = widthName.size();
    this->heightName = heightN;
    this->widthName = widthN;

    for (int i = 0; i < (1 << height) ; ++i) {
        size_t base = 0;
        size_t temp1 = i;
        for (int k = 0; k < height; ++k){
            base += (temp1 % 2) * ( 1 << (BF.getProtNum(heightN[k])) );
            temp1 = temp1 / 2;
        }
        for (int j = 0; j < width; ++j) {
            size_t exten = 0;
            size_t temp2 = j;
            for (int k = 0; k < width; ++k) {
                exten += (temp2 % 2) * (1 << (BF.getProtNum(widthN[k])))
            }

            int res = BF.getVal(base + exten);
            kmap[i][j] = res;
        }
    }

}

Kmap::~Kmap() {
    while (kmap.size()>0)
        kmap.pop_back();
}

bool Kmap::operator==(const Kmap &initKmap) {
    // FIXME
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

TTable& Kmap::operator[](const int &i) {
    return kmap[i];
}

size_t Kmap::getHeight() {
    return height;
}

size_t Kmap::getWidth() {
    return width;
}

NodeName Kmap::getHeightName(const int &i) {
    if (i >= height) assert(0);
    return heightName[i];
}

NodeName Kmap::getWidthName(const int &j) {
    if (j >= width) assert(0);
    return widthName[j];
}

BestApprox Kmap::divide(SimulationResult &simData) {

    // 1: 0 + 1
    // 2: MajorRow + 0
    // 3: MajorRpw + 1
    // 4: MajorRow + complement of MajorRow

    // this is the most important part !!!!!
    // but my thinking is cease !!!! WTF !!!!!

    vector<NodeName> nodeNameSet = heightName;
    for (int i = 0; i < widthName.size(); ++i) {
        nodeNameSet.push_back(widthName[i]);
    }
    FocusedSimulationResult focusSim = simData.focus(nodeNameSet);



}
