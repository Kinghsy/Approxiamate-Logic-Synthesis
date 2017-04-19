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
    struct ResultType {
        size_t errorCount;
        BoolFunction fun;
        BlifBuilder deInfo;
        ResultType():
                fun(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                deInfo(NodeName("")) {
            errorCount = 0;
        }
    };

private:

    BoolFunction initBF;
    ResultType searchPrcoe(const BoolFunction& bf,
                           const SimulationResult& simData,
                           const ActivedMode ApplicatedMode);

public:

    ResultType operate(const BoolFunction& bf,
                       const SimulationResult& simData,
                       const ActivedMode ApplicatedMode);

    AlgorithmDecompose():
            initBF(2, AND_TABLE,
                   std::vector<NodeName>(), NodeName("abc")) {
    }
    ~AlgorithmDecompose() {}

};


#endif //VE490_ALGORITHM_DECOMPOSE_H
