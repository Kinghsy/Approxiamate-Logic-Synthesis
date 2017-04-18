//
// Created by king on 17-4-11.
//
#include <vector>
#include <map>

#include "const.h"
#include "ttable.h"
#include "kmap.h"
#include "bool_function.h"
#include "algorithm_decompose.h"

#include <ttable.h>
#include <blif_builder.h>

using std::string;
using std::vector;

AlgorithmDecompose::ResultType
AlgorithmDecompose::operate(const BoolFunction &bf,
                               const SimulationResult &simData) {

    initBF = bf;
    srand(time(nullptr));
    ResultType res = searchPrcoe(bf, simData);
    srand(0);
    return res;
    //return bestDecomp;

}

AlgorithmDecompose::ResultType
AlgorithmDecompose::searchPrcoe(const BoolFunction& bf,
                                const SimulationResult &simData) {

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
    // now try some method to divide bf's inputs into two sets.

    ResultType bestApproximation;
    bestApproximation.errorCount = MAX_VALUE;

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
        // approx contains the best divide information

        // if branch and bound, please add in this line.
        // do some pre-operation.
        // BRANCH_AND_BOUND, BFS

        ResultType leftRes = searchPrcoe(approx.leftFunc, simData);
        ResultType rightRes = searchPrcoe(approx.rightFunc, simData);
        // search for the left best solution and right best solution

        BoolFunction approxFun = combineBooleanFunction(
                leftRes.fun, rightRes.fun, approx.method, bf.getOutPortName()
        );
        // obtain approximation bool function and compare it with initial

        if ( (approxFun^bf) < bestApproximation.errorCount) {
            bestApproximation.errorCount =  approxFun ^ bf;
            bestApproximation.deInfo = combineBilfBuilder(
                    leftRes.deInfo, rightRes.deInfo, approx.method, bf.getOutPortName()
            );
            bestApproximation.fun = approxFun;
        }
    }

    return bestApproximation;

}
