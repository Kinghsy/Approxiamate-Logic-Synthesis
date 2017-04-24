//
// Created by king on 17-4-11.
//
#include <vector>
#include <map>

#include "const.h"
#include "kmap.h"
#include "bool_function.h"
#include "algorithm_decompose.h"
#include "min_set.h"

//#include <ttable.h>
//#include <blif_builder.h>

#include "../../common/ttable.h"
#include "../../circuit_profile/sim_profile.h"

using std::string;
using std::vector;

size_t boolErrorCount(const BoolFunction& f1, const BoolFunction& f2,
                      const FocusedSimulationResult& simData) {

    assert(f1.getInputSize() == f2.getInputSize());
    size_t err = 0;
    size_t n = f1.getInputSize();
    TTable ff1(n);
    ff1 = f1.getTTable();
    TTable ff2(n);
    ff2 = f2.getTTable();

    for (int i = 0; i < (1 << n); ++i) {
        if (ff1[i] == ff2[i]) continue;
        DBitset db(n, 0);
        size_t temp = i;
        for (int k = 0; k < n; ++k) {
            db[k] = temp % 2;
            temp  = temp / 2;
        }
        err += simData.count(db);
    }
    return err;
}


AlgorithmDecompose::ResultType
AlgorithmDecompose::operator()(const BoolFunction &bf,
                               const SimulationResult &simData) {

    initBF = bf;
    srand(time(nullptr));
    ResultType res = searchPrcoe(bf, simData);
    srand(0);
    return res;

}

AlgorithmDecompose::ResultType
AlgorithmDecompose::searchPrcoe(const BoolFunction& bf,
                                const SimulationResult &simData) {

    if ((bf.getInputSize() <= 6) &&
    (bf.getInputSize() >=3 )) {
        return (*sAlgo)(bf, simData);
    }

    if (bf.getInputSize() == 1) {
        ResultType res;
        //assert(bf.getPortName(0) != "");
        if ((bf.getTTable()) == NOT_1_INPUT) {
            BlifBuilder temp = BlifBuilder::buildInput(bf.getPortName(0), true);
            res.deInfo = temp;
        } else if ((bf.getTTable()) == NORMAL_1_INPUT ) {
            BlifBuilder temp = BlifBuilder::buildInput(bf.getPortName(0), false);
            res.deInfo = temp;
        } /*else if ( bf.isAll0s() ) {
            BlifBuilder temp = BlifBuilder::buildConst(bf.getPortName(0), 0);
            res.deInfo = temp;
        } else if ( bf.isAll1s() ) {
            BlifBuilder temp = BlifBuilder::buildConst(bf.getPortName(0), 1);
            res.deInfo = temp;
        }*/
        res.errorCount = 0;
        res.fun = bf;
        return res;
    }

    // here means this bf at least has two inputs and it is divideble
    // now try some method to divideCore bf's inputs into two sets.
    ResultType bestApproximation;
    bestApproximation.errorCount = MAX_VALUE;

    vector<string > nodeSet;
    for (int j = 0; j < bf.getInputSize(); ++j) {
        nodeSet.push_back(bf.getPortName(j));
    }
    FocusedSimulationResult focus(simData, nodeSet);

    MinSet<Kmap::BestApprox> bfsSeries(bf.getInputSize());
    vector<Kmap::BestApprox> searchSeries;

    for (size_t i = 1; i < (1 << bf.getInputSize()) - 1; ++i) {

        if (i % 2 == 0) continue; // keep the first input always in the left function( as column )

        size_t temp = i;
        vector<string> portSet[2];
        for (size_t j = 0; j < bf.getInputSize(); ++j) {
            portSet[temp % 2].push_back(bf.getPortName(j));
            temp = temp / 2;
        }
        /*if (i == 29) {
            initBF.display();
            bf.display();
        }*/
        Kmap fig(bf, portSet[1], portSet[0]);
        /*if (i == 29) {
            fig.display();
        }*/
        Kmap::BestApprox approx = fig.divide(simData);
        // approx contains the best divideCore information

        // BRANCH_AND_BOUND
        if ((ApplicatedMode == BRANCH_AND_BOUND)
            || (ApplicatedMode == FULL_SEARCH)) {
            searchSeries.push_back(approx);
        }

        // BFS_SEARCH
        if (ApplicatedMode == BFS_SEARCH) {
            bfsSeries.push(approx);
        }
    }

    while ((bfsSeries.size() > 0) || (searchSeries.size() > 0)) {
        Kmap::BestApprox approx;
        if ((ApplicatedMode == FULL_SEARCH )
            || (ApplicatedMode == BRANCH_AND_BOUND)) {
            approx = searchSeries.back();
            searchSeries.pop_back();
        }
        if (ApplicatedMode == BFS_SEARCH) {
            approx = bfsSeries.back();
            bfsSeries.popBack();
        }

        if (ApplicatedMode == BRANCH_AND_BOUND)
            if (approx.errorCount >= bestApproximation.errorCount)
                continue;

        ResultType leftRes = searchPrcoe(approx.leftFunc, simData);
        ResultType rightRes = searchPrcoe(approx.rightFunc, simData);
        // search for the left best solution and right best solution

        BoolFunction approxFun = combineBooleanFunction(
                leftRes.fun, rightRes.fun, approx.method, bf.getOutPortName()
        );
        // obtain approximation bool function and compare it with initial

        approxFun.reorder(nodeSet, bf.getOutPortName()); // reorder.

        /*Kmap::BestApprox recombine = fig.errorCountWhole(
                focus, leftRes.fun.getTTable(), rightRes.fun.getTTable(), approx.method
        );*/
        size_t err = boolErrorCount(approxFun, bf, focus);

        if ( err < bestApproximation.errorCount) {
            bestApproximation.errorCount =  err;
            bestApproximation.deInfo = combineBilfBuilder(
                    leftRes.deInfo, rightRes.deInfo, approx.method, bf.getOutPortName()
            );
            //leftRes.fun.display();
            //rightRes.fun.display();
            //std::cout << (approx.method==XOR_TABLE) << std::endl;
            //bestApproximation.deInfo.printBody(std::cout);
            bestApproximation.fun = approxFun;
        }

    }

    return bestApproximation;

}


