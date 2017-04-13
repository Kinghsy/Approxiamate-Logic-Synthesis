//
// Created by king on 17-4-11.
//

#ifndef VE490_ALGORITHM_DECOMPOSE_H
#define VE490_ALGORITHM_DECOMPOSE_H

#include <deque>

#include "const.h"
#include "bool_function.h"
#include "blif_builder.h"
#include "kmap.h"

class SimulationResult;

class AlgorithmDecompose {

public:
    //std::deque<BooleanFunction> BooleanFunctionPool;
    struct ResultType {
        size_t errorCount;
        BooleanFunction fun;
        BlifBuilder deInfo;
    };

private:

    BooleanFunction initBF;
    //ResultType bestDecomp;
    ResultType searchPrcoe(const BooleanFunction& bf,
                           const SimulationResult& simData);

public:

    ResultType operator()(const BooleanFunction& bf,
                          const SimulationResult& simData);

    AlgorithmDecompose() {}
    ~AlgorithmDecompose() {}

};


#endif //VE490_ALGORITHM_DECOMPOSE_H
