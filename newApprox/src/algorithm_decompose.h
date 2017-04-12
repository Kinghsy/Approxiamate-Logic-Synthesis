//
// Created by king on 17-4-11.
//

#ifndef VE490_ALGORITHM_DECOMPOSE_H
#define VE490_ALGORITHM_DECOMPOSE_H

#include <deque>

#include "const.h"
#include "boolean_function.h"
#include "decomposition_info.h"
#include "kmap.h"
#include "../../circuit_profile/sim_profile.h"

class AlgorithmDecompose {

public:
    //std::deque<BooleanFunction> BooleanFunctionPool;
    struct ResultType {
        size_t errorCount;
        BlifBuilder deInfo;
    };

private:

    BooleanFunction initBF;
    //ResultType bestDecomp;
    ResultType searchPrcoe(BooleanFunction bf,
                           const SimulationResult& simData);

public:

    ResultType operate(const BooleanFunction& bf,
                       const SimulationResult& simData);

    AlgorithmDecompose();
    ~AlgorithmDecompose();

};


#endif //VE490_ALGORITHM_DECOMPOSE_H
