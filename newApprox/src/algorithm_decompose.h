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

class IAlgorithmDecompose {
public:
    struct ResultType {
        size_t errorCount;
        BoolFunction fun;
        BlifBuilder deInfo;
        ResultType():
                fun(0, NORMAL_1_INPUT, std::vector<NodeName>(), NodeName("")),
                deInfo (  BlifBuilder::buildConst( NodeName(""), 1 ) ) {
            errorCount = 0;
        }
    };

    virtual ResultType operator() (const BoolFunction& bf,
                                   const SimulationResult& simData) = 0;
};

typedef std::shared_ptr<IAlgorithmDecompose> AlgPtr;

class AlgorithmDecompose : public IAlgorithmDecompose{
public:
    enum Mode {
        FULL_SEARCH, BRANCH_AND_BOUND,
        BFS_SEARCH, BFS_SEARCH_PRE_WIDTH
    };

private:
    const Mode ApplicatedMode;

    BoolFunction initBF;
    ResultType searchPrcoe(const BoolFunction& bf,
                           const SimulationResult& simData);
    AlgPtr sAlgo;

public:

    ResultType operator()(const BoolFunction& bf,
                          const SimulationResult& simData) override;

    AlgorithmDecompose(const Mode ApplicateMode_, AlgPtr& smallAlgo):
            ApplicatedMode(ApplicateMode_),
            initBF(2, AND_TABLE,
                   std::vector<NodeName>(), NodeName("abc")) {
        sAlgo = smallAlgo;
    }
    ~AlgorithmDecompose() {}

};


#endif //VE490_ALGORITHM_DECOMPOSE_H
