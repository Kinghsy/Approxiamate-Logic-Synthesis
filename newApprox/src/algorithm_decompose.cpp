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
AlgorithmDecompose::operate(const BoolFunction &bf,
                            const SimulationResult &simData,
                            const ActivedMode ApplicatedMode) {

    initBF = bf;
    srand(time(nullptr));
    ResultType res = searchPrcoe(bf, simData, ApplicatedMode);
    srand(0);
    return res;
    //return bestDecomp;

}

AlgorithmDecompose::ResultType
AlgorithmDecompose::searchPrcoe(const BoolFunction& bf,
                                const SimulationResult &simData,
                                const ActivedMode ApplicatedMode) {

    if (bf.getInputSize() == 1) {
        ResultType res;
        if ((bf.getTTable()) == NOT_1_INPUT) {
            BlifBuilder temp(bf.getOutPortName(), true);
            res.deInfo = temp;
        } else {
            BlifBuilder temp(bf.getOutPortName(), false);
            res.deInfo = temp;
        } //FIXME
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

    for (size_t i = 1; i < (1 << bf.getInputSize()) - 1; ++i) {

        if (i % 2 == 0) continue; // keep the first input always in the left function( as column )

        size_t temp = i;
        vector<string > portSet[2];
        for (size_t j = 0; j < bf.getInputSize(); ++j) {
            portSet[temp % 2].push_back(bf.getPortName(j));
            temp = temp / 2;
        }
        Kmap fig(bf, portSet[1], portSet[0]);
        Kmap::BestApprox approx = fig.divide(simData);
        // approx contains the best divideCore information

        // BRANCH_AND_BOUND
        if ( ApplicatedMode == BRANCH_AND_BOUND ) {
            if (approx.errorCount >= bestApproximation.errorCount)
                continue;
        }

        // BFS_SEARCH
        if ( ApplicatedMode == BFS_SEARCH ) {
            bfsSeries.push(approx);
            continue;
        }

        ResultType leftRes = searchPrcoe(approx.leftFunc, simData, ApplicatedMode);
        ResultType rightRes = searchPrcoe(approx.rightFunc, simData, ApplicatedMode);
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
            bestApproximation.fun = approxFun;
        }
    }

    if ( (ApplicatedMode == BRANCH_AND_BOUND)
         || (ApplicatedMode == FULL_SEARCH) )
        return bestApproximation;

    while (bfsSeries.size() > 0) {
        Kmap::BestApprox approx = bfsSeries.back();
        bfsSeries.popBack();
        // FIXME
    }

}


