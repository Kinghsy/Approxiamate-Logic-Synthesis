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

string num2string(size_t bits, size_t num) {
    string base = "";
    size_t temp = num;
    for (size_t k = 0; k < bits; ++k) {
        base += (temp % 2) ? "1":"0";
        temp = temp / 2;
    }
    return base;
}

void randTTable(TTable& a, const size_t bits) {
    for (size_t i = 0; i < bits; ++i) {
        a[i] = rand() % 2;
    }
}

tempNodeNameGenertor nodeNameGen;

Kmap::Kmap(
        const BoolFunction &BF,
        const std::vector<NodeName> &heightN,
        const std::vector<NodeName> &widthN) {

    heightName = heightN;
    widthName = widthN;
    height = ( 1 << (heightName.size()));
    width = ( 1 << (widthName.size()));

    auto rowselector = DBitset(BF.getInputSize());
    auto colselector = DBitset(BF.getInputSize());

    for (const auto& elem : heightN)
        colselector.set(BF.getPortNum(elem));

    for (const auto& elem : widthN)
        rowselector.set(BF.getPortNum(elem));

    assert(colselector == ~rowselector);

    kmap = BF.getTTable().breakdown(rowselector, colselector);

}

bool Kmap::operator==(const Kmap &initKmap) const {
    return this->kmap == initKmap.kmap;
}

size_t Kmap::operator^(const Kmap &initKmap) const {
    size_t count = 0;
    for (int i = 0; i < height; ++i)
        count += (this->kmap[i] ^ initKmap.kmap[i]).count();
    return count;
}

Kmap::BestApprox Kmap::divide(const SimulationResult &simData) {

    // case 1: 0s and 1s
    //  operation DROP_RIGHT (LEFT_RELA_TABLE)
    //  column = 1 if 1s is selected

    // case 2: MajorRow + 0s
    //  operation AND
    //  column = 1 if RowPattern is selected

    // case 3: 1s + MajorRow
    //  operatrion OR
    //  column = 1 if 1s is selected

    // case 4: MajorRow + ~MajorRow
    //  operation XOR
    //  column = 1 if MajorRow is selected

    vector<NodeName> nodeNameSet = heightName;
    for (int i = 0; i < widthName.size(); ++i) {
        nodeNameSet.push_back(widthName[i]);
    }
    FocusedSimulationResult focusSim = simData.focus(nodeNameSet);

    TTable All0s(0, width);
    TTable All1s( ( 1 << width ) - 1, width);


    // case 1:
    TTable rowPattern(0, width);
    TTable columnPattern(0, height);
    size_t err = MAX_VALUE;

    while (true) {
        TTable recordRowPattern(0, width);
        recordRowPattern = rowPattern;
        TTable recordColumnPattern(0, height);
        recordColumnPattern = columnPattern;

        // rowPattern --> columnPatternTemp
        TTable columnPatternTemp(0, height);
        columnPatternTemp = getColumnPattern(focusSim, All1s, All0s);
        size_t errTemp = errorCountRowColumnPattern(
                rowPattern, columnPatternTemp, LEFT_RELA_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            columnPattern = columnPatternTemp;
        }

        // columnPattern --> rowPatternTemp
        TTable rowPatternTemp(0, width);
        rowPatternTemp = getRowPattern(
                columnPattern, focusSim, NORMAL_1_INPUT, NORMAL_1_INPUT);
        errTemp = errorCountRowColumnPattern(
                rowPatternTemp, columnPattern, LEFT_RELA_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            rowPattern = rowPatternTemp;
        }

        if ((recordRowPattern == rowPattern) &&
        (recordColumnPattern == columnPattern)) break;
    }

    BestApprox case1;
    case1.leftFunc = BoolFunction(heightName.size(), columnPattern, heightName, nodeNameGen.gen());
    case1.rightFunc = BoolFunction(widthName.size(), rowPattern, widthName, nodeNameGen.gen());
    case1.errorCount = err;
    case1.method = LEFT_RELA_TABLE;
    if (case1.leftFunc.isAll1s())
        case1.method = ALL_IRR_TABLE_1;
    else if (case1.leftFunc.isAll0s())
        case1.method = ALL_IRR_TABLE_0;
    

    // case 2
    randTTable(rowPattern, width);
    randTTable(columnPattern, height);
    err = MAX_VALUE;

    while (true) {
        TTable recordRowPattern(0, width);
        recordRowPattern = rowPattern;
        TTable recordColumnPattern(0, height);
        recordColumnPattern = columnPattern;

        // rowPattern --> columnPatternTemp
        TTable columnPatternTemp(0, height);
        columnPatternTemp = getColumnPattern(focusSim, rowPattern, All0s);
        size_t errTemp = errorCountRowColumnPattern(
                rowPattern, columnPatternTemp, AND_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            columnPattern = columnPatternTemp;
        }

        // columnPattern --> rowPatternTemp
        TTable rowPatternTemp(0, width);
        rowPatternTemp = getRowPattern(
                columnPattern, focusSim, ALL_0_1_INPUT, NORMAL_1_INPUT);
        errTemp = errorCountRowColumnPattern(
                rowPatternTemp, columnPattern, AND_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            rowPattern = rowPatternTemp;
        }

        if ((recordRowPattern == rowPattern) &&
            (recordColumnPattern == columnPattern)) break;
    }

    BestApprox case2;
    case2.leftFunc = BoolFunction(heightName.size(), columnPattern, heightName, nodeNameGen.gen());
    case2.rightFunc = BoolFunction(widthName.size(), rowPattern, widthName, nodeNameGen.gen());
    case2.errorCount = err;
    case2.method = AND_TABLE;
    if (case2.leftFunc.isAll1s() && case2.rightFunc.isAll1s())
        case2.method = ALL_IRR_TABLE_1;
    else if (case2.leftFunc.isAll0s() || case2.rightFunc.isAll0s())
        case2.method = ALL_IRR_TABLE_0;
    else if (case2.leftFunc.isAll1s())
        case2.method = RIGHT_RELA_TABLE;
    else if (case2.rightFunc.isAll1s())
        case2.method = LEFT_RELA_TABLE;


    // case 3
    randTTable(rowPattern, width);
    randTTable(columnPattern, height);
    err = MAX_VALUE;

    while (true) {
        TTable recordRowPattern(0, width);
        recordRowPattern = rowPattern;
        TTable recordColumnPattern(0, height);
        recordColumnPattern = columnPattern;

        // rowPattern --> columnPatternTemp
        TTable columnPatternTemp(0, height);
        columnPatternTemp = getColumnPattern(focusSim, All1s, rowPattern);
        size_t errTemp = errorCountRowColumnPattern(
                rowPattern, columnPatternTemp, OR_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            columnPattern = columnPatternTemp;
        }

        // columnPattern --> rowPatternTemp
        TTable rowPatternTemp(0, width);
        rowPatternTemp = getRowPattern(
                columnPattern, focusSim, NORMAL_1_INPUT, ALL_1_1_INPUT);
        errTemp = errorCountRowColumnPattern(
                rowPatternTemp, columnPattern, OR_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            rowPattern = rowPatternTemp;
        }

        if ((recordRowPattern == rowPattern) &&
            (recordColumnPattern == columnPattern)) break;
    }

    BestApprox case3;
    case3.leftFunc = BoolFunction(heightName.size(), columnPattern, heightName, nodeNameGen.gen());
    case3.rightFunc = BoolFunction(widthName.size(), rowPattern, widthName, nodeNameGen.gen());
    case3.errorCount = err;
    case3.method = OR_TABLE;
    if (case3.leftFunc.isAll1s() || case3.rightFunc.isAll1s())
        case3.method = ALL_IRR_TABLE_1;
    else if (case3.leftFunc.isAll0s() && case3.rightFunc.isAll0s())
        case3.method = ALL_IRR_TABLE_0;
    else if (case3.leftFunc.isAll0s())
        case3.method = RIGHT_RELA_TABLE;
    else if (case3.rightFunc.isAll0s())
        case3.method = LEFT_RELA_TABLE;


    // case 4
    randTTable(rowPattern, width);
    randTTable(columnPattern, height);
    err = MAX_VALUE;

    while (true) {
        TTable recordRowPattern(0, width);
        recordRowPattern = rowPattern;
        TTable recordColumnPattern(0, height);
        recordColumnPattern = columnPattern;

        // rowPattern --> columnPatternTemp
        TTable columnPatternTemp(0, height);
        TTable compRowPattern(0, width);
        compRowPattern = rowPattern;
        compRowPattern.flip();
        columnPatternTemp = getColumnPattern(focusSim, compRowPattern, rowPattern);
        size_t errTemp = errorCountRowColumnPattern(
                rowPattern, columnPatternTemp, XOR_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            columnPattern = columnPatternTemp;
        }

        // columnPattern --> rowPatternTemp
        TTable rowPatternTemp(0, width);
        rowPatternTemp = getRowPattern(
                columnPattern, focusSim, NORMAL_1_INPUT, NOT_1_INPUT);
        errTemp = errorCountRowColumnPattern(
                rowPatternTemp, columnPattern, XOR_TABLE, focusSim);
        if (errTemp < err) {
            err = errTemp;
            rowPattern = rowPatternTemp;
        }

        if ((recordRowPattern == rowPattern) &&
            (recordColumnPattern == columnPattern)) break;
    }

    BestApprox case4;
    case4.leftFunc = BoolFunction(heightName.size(), columnPattern, heightName, nodeNameGen.gen());
    case4.rightFunc = BoolFunction(widthName.size(), rowPattern, widthName, nodeNameGen.gen());
    case4.errorCount = err;
    case4.method = XOR_TABLE;
    if ((case4.leftFunc.isAll0s() && case4.rightFunc.isAll1s()) ||
        (case4.leftFunc.isAll1s() && case4.rightFunc.isAll0s()))
        case4.method = ALL_IRR_TABLE_1;
    else if ((case4.leftFunc.isAll0s() && case4.rightFunc.isAll0s()) ||
        (case4.leftFunc.isAll1s() && case4.rightFunc.isAll1s()))
        case4.method = ALL_IRR_TABLE_0;
    else if (case4.leftFunc.isAll1s())
        case4.method = RIGHT_RELA_NOT_TABLE;
    else if (case4.leftFunc.isAll0s())
        case4.method = RIGHT_RELA_TABLE;
    else if (case4.rightFunc.isAll1s())
        case4.method = LEFT_RELA_NOT_TABLE;
    else if (case4.rightFunc.isAll0s())
        case4.method = LEFT_RELA_TABLE;


    // conclude
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

TTable Kmap::getColumnPattern(const FocusedSimulationResult &focusSim,
                              const TTable &targetOf1,
                              const TTable &targetOf0) {
// rowPattern --> columnPattern

// taraget1 means if columnPattern of this row is selected as 1,
// the outcome of this row should be target1.

    TTable column(0, height);
    for (size_t i = 0; i < height; ++i) {
        string base = num2string(heightName.size(), i);
        size_t err_0, err_1;
        err_0 = 0; err_1 = 0;
        TTable diff_0 = targetOf0 ^ kmap[i];
        TTable diff_1 = targetOf1 ^ kmap[i];
        for (size_t j = 0; j < width; ++j) {
            string exten = num2string(widthName.size(), j);
            err_0 += (diff_0[j] * focusSim.count(base + exten));
            err_1 += (diff_1[j] * focusSim.count(base + exten));
        }
        column[i] = (err_0 < err_1) ? 0:1;
    }
    return column;
}

TTable Kmap::getRowPattern(const TTable &columnPattern, 
                           const FocusedSimulationResult &focusSim, 
                           const TTable &transOf0,
                           const TTable &transOf1) {
// columnPattern --> rowPattern

// transOf0 means if rowPattern of this column is selected as 0,
// the outcome of it under columnPattern should follow transOf0.    

    TTable columnOf0(0, heightName.size());
    TTable columnOf1(1, heightName.size());
    for (size_t i = 0; i < height; ++i) {
        columnOf0[i] = transOf0[columnPattern[i]];
        columnOf1[i] = transOf1[columnPattern[i]];
    }

    TTable row(0, width);
    for (size_t j = 0; j < width ; ++j) {
        string exten = num2string(widthName.size(), j);
        size_t err_0, err_1;
        err_0 = 0; err_1 = 0;
        for (size_t i = 0; i < height; ++i) {
            string base = num2string(heightName.size(), i);
            err_0 += (kmap[i][j] ^ columnOf0[i]) * focusSim.count(base + exten);
            err_1 += (kmap[i][j] ^ columnOf1[i]) * focusSim.count(base + exten);
        }
        row[j] = (err_0 < err_1) ? 0:1;
    }
    return row;
}

size_t Kmap::errorCountRowColumnPattern(
        const TTable &rowPattern, 
        const TTable &columnPattern,
        const TTable &combineMethod,
        const FocusedSimulationResult &focusSim
) {
    
// under given rowPattern, columnPattern and combineMethod,
// check the current error. (total bits flip in different cases)
    size_t totalInputs = rowPattern.nInputs() + columnPattern.nInputs();
    DBitset colMask(0, totalInputs);
    DBitset rowMask(0, totalInputs);

    for (size_t i = 0; i < (columnPattern.nInputs()); ++i) {
        colMask[i] = 1;
    }
    for (size_t i = columnPattern.nInputs(); i < totalInputs; ++i) {
        rowMask[i] = 1;
    }

    TTable combineTTable(0, totalInputs);
    combineTTable = combineTruthTable(columnPattern, rowPattern,
                        colMask, rowMask, combineMethod);
    vector<TTable> newMap = combineTTable.breakdown(rowMask, colMask);

    size_t err = 0;
    for (size_t i = 0; i < height; ++i) {
        string base = num2string(heightName.size(), i);
        for (size_t j = 0; j < width; ++j) {
            string exten = num2string(widthName.size(), j);
            err += (newMap[i][j] ^ kmap[i][j]) * focusSim.count(base + exten);
        }
    }

    return err;
}

