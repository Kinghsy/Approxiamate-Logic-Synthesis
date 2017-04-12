//
// Created by king on 17-4-11.
//
#include <vector>
#include <map>

#include "const.h"
#include "truth_table.h"
#include "kmap.h"
#include "boolean_function.h"
#include "decomposition_info.h"
#include "algorithm_decompose.h"

using std::string;
using std::vector;

AlgorithmDecompose::AlgorithmDecompose() {}

AlgorithmDecompose::~AlgorithmDecompose() {}

ResultType AlgorithmDecompose::operate(
        const BooleanFunction &bf,
        const SimulationResult &simData) {

    initBF = bf;
    return searchPrcoe(bf, simData);
    //return bestDecomp;

}

ResultType AlgorithmDecompose::searchPrcoe(
        BooleanFunction bf, const SimulationResult &simData) {

    if (bf.isAll0s() || bf.isAll1s()) {
        BlifBuilder::Connection conn;
        conn.n1 = ""; conn.n2 = "";
        conn.method = (bf.isAll0s()) ? ALL_OS:ALL_1S;
        BlifBuilder info(bf.getOutPortName(), conn);
        ResultType res;
        res.errorCount = 0; res.deInfo = info;
        return res;
    }

    // include one input, which could be consider as a kind of cache?

    if (bf.getInputSize() <= 2) {
        BlifBuilder::Connection conn;
        conn.n1 = bf.getPortName(0); conn.n2 = bf.getPortName(1);
        conn.method = findMethod(bf);
        BlifBuilder info(bf.getOutPortName(), conn);
        ResultType res;
        res.errorCount = 0; res.deInfo = info;
        return res;
    }

    // here means this bf at least has three inputs and it is divideble
    // now try some method to divide bf's inputs into two sets.


    ResultType bestApproximation;
    bestApproximation.errorCount = MAX_VALUE;
    for (size_t i = 1; i < (1 << bf.getInputSize() - 1); ++i) {

        size_t temp = i;
        vector<string > portSet[2];
        for (int j = 0; j < bf.getInputSize(); ++j) {
            portSet[temp % 2].push_back(bf.getPortName(j));
            temp = temp >> 1;
        }
        Kmap fig(bf, portSet[1], portSet[0]);
        Kmap::BestApprox approx = fig.divide(simData);
        // approx contains the best divide information

        ResultType leftRes = searchPrcoe(approx.leftFunc, simData);
        ResultType rightRes = searchPrcoe(approx.rightFunc, simData);
        BlifBuilder::Connection conn;
        conn.n1 = leftRes.deInfo.outputNodeName();
        conn.n2 = rightRes.deInfo.outputNodeName();
        conn.method = approx.method;

        BlifBuilder thisStep(bf.getOutPortName(), conn);
        BlifBuilder wholeDecompInfo =
                combineDecompositionInfo(thisStep, leftRes.deInfo, rightRes.deInfo);
        if ( ((wholeDecompInfo.buildDecompInfo()) ^ (initBF)) < bestApproximation.errorCount) {
            bestApproximation.errorCount =  (wholeDecompInfo.buildDecompInfo()) ^ (initBF);
            bestApproximation.deInfo = wholeDecompInfo;
        }

    }

    return bestApproximation;

}
