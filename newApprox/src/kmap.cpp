//
// Created by king on 17-4-12.
//

#include <vector>
#include <map>
#include <sstream>

#include "const.h"
#include "ttable.h"
#include "kmap.h"
#include "bool_function.h"
#include "../../circuit_profile/sim_profile.h"

using std::string;
using std::vector;
using std::map;

struct tempNodeNameGenertor {
    string base = "tempNodeTempNode";
    int num = 0;
    string gen() {
        std::stringstream xx;
        xx << base << num++ ;
        return xx.str();
    }
};

struct TTableRela {
    size_t lineNum;
    TTable line;
    TTableRela(size_t i, const TTable& t): line(t) {
        lineNum = i;
    }
};

size_t countErrorWithTarget(const TTable& target, const TTable& nowRun,
                            string base, const FocusedSimulationResult& focusSim);

TTable calcMajorRow(vector<TTableRela>& vec, size_t bits, const FocusedSimulationResult& simData);

string num2string(size_t bits, size_t num);

tempNodeNameGenertor nodeNameGen;

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

Kmap::BestApprox Kmap::divide(const SimulationResult &simData){

    // 1: 0 + 1
    // 2: MajorRow + 0
    // 3: MajorRpw + 1
    // 4: MajorRow + complement of MajorRow

    vector<NodeName> nodeNameSet = heightName;
    for (int i = 0; i < widthName.size(); ++i) {
        nodeNameSet.push_back(widthName[i]);
    }
    FocusedSimulationResult focusSim = simData.focus(nodeNameSet);

    // case 1:
    TTable all0(0, width);
    TTable all1((1 << width) - 1, width);
    BestApprox case1;
    case1.errorCount = 0;
    case1.rightFunc = BooleanFunction(width, TTable(0, width), widthName, nodeNameGen.gen());
    //case1.leftFunc = BooleanFunction(height, TTable(0, height), heightName, nodeNameGen.gen());
    case1.method = LEFT_RELA_TABLE;
    TTable leftt(0, height);

    for (size_t i = 0; i < height; ++i) {
        TTable line = kmap[i];
        string base = num2string(height, i);
        size_t error1 = countErrorWithTarget(all0, line, base, focusSim);
        size_t error2 = countErrorWithTarget(all1, line, base, focusSim);
        if (error1 < error2) {
            case1.errorCount += error1;
            leftt[i] = 0;
        } else {
            case1.errorCount += error2;
            leftt[i] = 1;
        }
    }
    case1.leftFunc = BooleanFunction(height, TTable(leftt), heightName, nodeNameGen.gen());


    if (case1.leftFunc.isAll1s()) case1.method = ALL_IRR_TABLE_1;
    if (case1.leftFunc.isAll0s()) case1.method = ALL_IRR_TABLE_0;


    // case 2:
    vector<TTableRela> majorSet;
    for (size_t i = 0; i < height; ++i) {
        majorSet.push_back(TTableRela(i, kmap[i]));
    }
    BestApprox case2; case2.errorCount = MAX_VALUE;
    while (true) {
        TTable majorRow = calcMajorRow(majorSet, height, focusSim);
        BestApprox case2temp;
        case2temp.errorCount = 0;
        //case2temp.leftFunc = BooleanFunction(height, TTable(0, height), heightName, nodeNameGen.gen());
        case2temp.rightFunc = BooleanFunction(width, majorRow, widthName, nodeNameGen.gen());
        case2temp.method = AND_TABLE;
        TTable lefttt(0, height);

        vector<TTableRela> majorSetTemp;
        for (size_t i = 0; i < height; ++i) {
            TTable line = kmap[i];
            string base = num2string(height, i);
            size_t error1 = countErrorWithTarget(all0, line, base, focusSim);
            size_t error2 = countErrorWithTarget(majorRow, line, base, focusSim);
            if (error1 < error2) {
                case2temp.errorCount += error1;
                lefttt[i] = 0;
            } else {
                case2temp.errorCount += error2;
                lefttt[i] = 1;
                majorSetTemp.push_back(TTableRela(i, line));
            }
        }
        case2temp.leftFunc = BooleanFunction(height, TTable(lefttt), heightName, nodeNameGen.gen());


        if (case2temp.errorCount >= case2.errorCount) break;
        majorSet = majorSetTemp;
        case2 = case2temp;
    }
    if (case2.leftFunc.isAll1s() && case2.rightFunc.isAll1s())
        case2.method = ALL_IRR_TABLE_1;
    else if (case2.leftFunc.isAll0s() || case2.rightFunc.isAll0s())
        case2.method = ALL_IRR_TABLE_0;
    else if (case2.leftFunc.isAll1s())
        case2.method = RIGHT_RELA_TABLE;
    else if (case2.rightFunc.isAll1s())
        case2.method = LEFT_RELA_TABLE;


    // case 3:
    vector<TTableRela>().swap(majorSet);
    for (size_t i = 0; i < height; ++i) {
        majorSet.push_back(TTableRela(i, kmap[i]));
    }
    BestApprox case3; case3.errorCount = MAX_VALUE;
    while (true) {
        TTable majorRow = calcMajorRow(majorSet, height, focusSim);
        BestApprox case3temp;
        case3temp.errorCount = 0;
        //case3temp.leftFunc = BooleanFunction(height, TTable(0, height), heightName, nodeNameGen.gen());
        case3temp.rightFunc = BooleanFunction(width, majorRow, widthName, nodeNameGen.gen());
        case3temp.method = OR_TABLE;
        TTable lefttt(0, height);

        vector<TTableRela> majorSetTemp;
        for (size_t i = 0; i < height; ++i) {
            TTable line = kmap[i];
            string base = num2string(height, i);
            size_t error1 = countErrorWithTarget(majorRow, line, base, focusSim);
            size_t error2 = countErrorWithTarget(all1, line, base, focusSim);
            if (error1 < error2) {
                case3temp.errorCount += error1;
                lefttt[i] = 0;
                majorSetTemp.push_back(TTableRela(i, line));
            } else {
                case3temp.errorCount += error2;
                lefttt[i] = 1;
            }
        }
        case3temp.leftFunc = BooleanFunction(height, TTable(lefttt), heightName, nodeNameGen.gen());

        if (case3temp.errorCount >= case3.errorCount) break;
        majorSet = majorSetTemp;
        case3 = case3temp;
    }
    if (case3.leftFunc.isAll0s() && case3.rightFunc.isAll0s())
        case3.method = ALL_IRR_TABLE_0;
    else if (case3.leftFunc.isAll1s() || case3.rightFunc.isAll1s())
        case3.method = ALL_IRR_TABLE_1;
    else if (case3.leftFunc.isAll0s())
        case3.method = RIGHT_RELA_TABLE;
    else if (case3.rightFunc.isAll0s())
        case3.method = LEFT_RELA_TABLE;


    // case 4
    vector<TTableRela>().swap(majorSet);
    vector<TTableRela> compMajorSet;
    for (size_t i = 0; i < height; ++i) {
        majorSet.push_back(TTableRela(i, kmap[i]));
    }
    BestApprox case4; case4.errorCount = MAX_VALUE;
    while (true) {
        for (auto& item: compMajorSet) {
            TTable tempTTable = item.line.flip();
            majorSet.push_back(TTableRela(item.lineNum, tempTTable));
        }
        TTable majorRow = calcMajorRow(majorSet, height, focusSim);
        TTable compMajorRow = majorRow; compMajorRow.flip();
        BestApprox case4temp;
        case4temp.errorCount = 0;
        //case4temp.leftFunc = BooleanFunction(height, TTable(0, height), heightName, nodeNameGen.gen());
        case4temp.rightFunc = BooleanFunction(width, majorRow, widthName, nodeNameGen.gen());
        case4temp.method = XOR_TABLE;
        TTable lefttt(0, height);

        vector<TTableRela> majorSetTemp;
        vector<TTableRela> compMajorSetTemp;
        for (size_t i = 0; i < height; ++i) {
            TTable line = kmap[i];
            string base = num2string(height, i);
            size_t error1 = countErrorWithTarget(majorRow, line, base, focusSim);
            size_t error2 = countErrorWithTarget(compMajorRow, line, base, focusSim);
            if (error1 < error2) {
                case4temp.errorCount += error1;
                lefttt[i] = 0;
                majorSetTemp.push_back(TTableRela(i, line));
            } else {
                case4temp.errorCount += error2;
                lefttt[i] = 1;
                compMajorSetTemp.push_back(TTableRela(i,line));
            }
        }
        case4temp.leftFunc = BooleanFunction(height, TTable(lefttt), heightName, nodeNameGen.gen());

        if (case4temp.errorCount >= case4.errorCount) break;
        majorSet = majorSetTemp;
        compMajorSet = compMajorSetTemp;
        case4 = case4temp;
    }
    if ((case4.leftFunc.isAll0s() && case4.rightFunc.isAll1s()) ||
        (case4.leftFunc.isAll1s() && case4.rightFunc.isAll0s()))
        case4.method = ALL_IRR_TABLE_1;
    else if ((case4.leftFunc.isAll0s() && case4.rightFunc.isAll0s()) ||
             (case4.leftFunc.isAll1s() && case4.rightFunc.isAll1s()))
        case4.method = ALL_IRR_TABLE_0;
    else if (case4.leftFunc.isAll0s())
        case4.method = RIGHT_RELA_TABLE;
    else if (case4.leftFunc.isAll1s())
        case4.method = RIGHT_RELA_NOT_TABLE;
    else if (case4.rightFunc.isAll0s())
        case4.method = LEFT_RELA_TABLE;
    else if (case4.rightFunc.isAll1s())
        case4.method = LEFT_RELA_NOT_TABLE;


    // conclusion
    if ((case1.errorCount <= case2.errorCount)
        && (case1.errorCount <= case3.errorCount)
        && (case1.errorCount <= case4.errorCount))
        return case1;
    else if ((case2.errorCount <= case3.errorCount)
            && (case2.errorCount <= case4.errorCount))
        return case2;
    else if (case3.errorCount <= case4.errorCount)
        return case3;
    else return case4;

}

TTable calcMajorRow(vector<TTableRela>& vec, size_t bits,
                    const FocusedSimulationResult& focusSim) {

    vector<size_t > count[2];
    size_t len = vec[0].line.nInputs();
    for (size_t k = 0; k < (1 << len); ++k) {
        count[0].push_back(0);
        count[1].push_back(0);
    }
    for (auto& item : vec) {
        string base = num2string(item.lineNum, bits);
        size_t n = item.line.nInputs();
        for (size_t i = 0; i < (1 << n); ++i) {
            string exten = num2string(i, n);
            count[item.line[i]][i] += focusSim.count(base + exten);
        }
    }

    TTable majorRow(0, len);
    for (size_t k = 0; k < (1 << len) ; ++k)
        if (count[1][k] > count[0][k])
            majorRow[k] = 1;

    return majorRow;

}

size_t countErrorWithTarget(const TTable& target, const TTable& nowRun,
    string base, const FocusedSimulationResult& focusSim) {

    size_t num = nowRun.nInputs();
    size_t len = 1 << num;
    size_t error = 0;
    for (size_t i = 0; i < len; ++i) {
        if (target[i] == nowRun[i]) continue;
        string exten = num2string(len, i);
        error += focusSim.count(base + exten);
    }
    return error;
}

string num2string(size_t bits, size_t num) {
    string base = "";
    size_t temp = num;
    for (size_t k = 0; k < bits; ++k) {
        base += (temp % 2) ? "1":"0";
        temp = temp / 2;
    }
    return base;
}

