//
// Created by tripack on 17-1-5.
//

#ifndef VE490_PATTERN_GEN_H
#define VE490_PATTERN_GEN_H

#include <generator.h>
#include <vector>

class PatternGenerator : public Generator<std::vector<int> > {
public:
    virtual int nInputs() = 0;
};

class FullPatternGenerator : public PatternGenerator {
    int inputSize;

    typedef long long unsigned int ulli;

    ulli currPattern;
    ulli max;

public:
    FullPatternGenerator(int nInputs);

    int nInputs() override;

    std::vector<int> generate() override;

    bool hasEnded() const override;

    Generator<std::vector<int>> *clone() const override;
};


class InfiniteRandomPatternGenerator : public PatternGenerator {
    int inputSize;

public:
    InfiniteRandomPatternGenerator(int nInputs);

    int nInputs() override;

    std::vector<int> generate() override;

    bool hasEnded() const override;

    Generator<std::vector<int>> *clone() const override;
};

class RandomPatternGenerator : public InfiniteRandomPatternGenerator {
    int nSamples;
    int currentSampleCount;

public:
    RandomPatternGenerator(int nInputs, int nSamples);

    std::vector<int> generate() override;

    bool hasEnded() const override;
};

#endif //VE490_PATTERN_GEN_H
