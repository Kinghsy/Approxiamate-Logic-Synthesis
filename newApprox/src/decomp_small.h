//
// Created by tripack on 4/22/17.
//

#ifndef VE490_DECOMP_SMALL_H
#define VE490_DECOMP_SMALL_H

#include "algorithm_decompose.h"
#include "../../pre_decomp/pre_decomp.h"

class AlgorithmDecomposeSmall : public IAlgorithmDecompose {
private:
    std::vector<NodeName> inputs;
    NodeName output;
    NodeNameGenerator nameGenerator;

    const float SIZE_ERROR_BONUS;

    const PreDecomp::DbEntry& match(const DBitset &fun, size_t inputSize,
                                    const FocusedSimulationResult& simResult);

    BlifBuilder buildFromMatch(const PreDecomp::DbEntry &entry,
                               const std::vector<NodeName>& nodeList,
                               const NodeName thisNode);

public:

    AlgorithmDecomposeSmall(float bonus = 0.0f)
            : nameGenerator("none"), SIZE_ERROR_BONUS(bonus) {};

    ResultType operator()(const BoolFunction &bf,
                          const SimulationResult &simData) override;

};

#endif //VE490_DECOMP_SMALL_H
