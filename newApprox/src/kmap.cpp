//
// Created by king on 17-4-12.
//

#include <vector>
#include <map>

#include "const.h"
#include "ttable.h"
#include "kmap.h"
#include "bool_function.h"
#include "../../circuit_profile/sim_profile.h"

using std::string;
using std::vector;
using std::map;

Kmap::Kmap(
        const BooleanFunction &BF,
        const std::vector<NodeName> &heightN,
        const std::vector<NodeName> &widthN) {

    heightName = heightN;
    widthName = widthN;
    height = heightName.size();
    width = widthName.size();

    auto rowselector = DBitset(BF.getInputSize());
    auto colselector = DBitset(BF.getInputSize());

    for (const auto& elem : heightN)
        colselector.set(BF.getPortNum(elem));

    for (const auto& elem : widthN)
        rowselector.set(BF.getPortNum(elem));

    assert(colselector == ~rowselector);

    kmap = BF.getTTable().breakdown(rowselector, colselector);

}

Kmap::~Kmap() {
    while (kmap.size()>0)
        kmap.pop_back();
}

bool Kmap::operator==(const Kmap &initKmap) const {
    // FIXME
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (kmap[i][j] != initKmap.kmap[i][j]) return false;
    return true;
}

size_t Kmap::operator^(const Kmap &initKmap) const {
    size_t count = 0;
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            count += (kmap[i][j] != initKmap.kmap[i][j]) ? 1:0 ;
    return count;
}




NodeName Kmap::getHeightName(int i) const {
    if (i >= height) assert(0);
    return heightName[i];
}

NodeName Kmap::getWidthName(int j) const {
    if (j >= width) assert(0);
    return widthName[j];
}

Kmap::BestApprox Kmap::divide(const SimulationResult &simData){

    // 1: 0 + 1
    // 2: MajorRow + 0
    // 3: MajorRpw + 1
    // 4: MajorRow + complement of MajorRow

    // this is the most important part !!!!!
    // but my thinking is ceased !!!! WTF !!!!!

    vector<NodeName> nodeNameSet = heightName;
    for (int i = 0; i < widthName.size(); ++i) {
        nodeNameSet.push_back(widthName[i]);
    }
    FocusedSimulationResult focusSim = simData.focus(nodeNameSet);

    //vector<int, TTable> set1, set2;

}

// FIXME
// FIXME
// FIXME

//TTable buildMajorRow(vector<int, TTable> set1,
//                     vector<int, TTable> set2,
//                     ModeType mode,
//                     FocusedSimulationResult& focusSim);
//
//int errorCompare(vector<int, TTable> set1,
//                 TTable target1,
//                 vector<int, TTable> set2,
//                 TTable target2,
//                 FocusedSimulationResult& focusSim) {
//
////    int count = 0;
////    for (auto item : set1) {
////        item.
////    }
//
//}
