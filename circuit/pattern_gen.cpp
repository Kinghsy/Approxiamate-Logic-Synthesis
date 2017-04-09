//
// Created by tripack on 17-1-5.
//

#include <cassert>
#include "pattern_gen.h"
#include <common.h>

FullPatternGenerator::FullPatternGenerator(int nInputs) {
    assert(nInputs > 0);
    assert(nInputs < sizeof(ulli) * 8 - 1);
    inputSize = nInputs;
    currPattern = 0u;
    max = ((ulli)1u) << nInputs;
}

int FullPatternGenerator::nInputs() {
    return this->inputSize;
}

std::vector<int> FullPatternGenerator::generate() {
    assert(!this->hasEnded());
    std::vector<int> p;
    for (int i = 0; i < inputSize; ++i) {
        if ((currPattern & (((ulli)1) << i)) > 0) {
            p.push_back(1);
        } else {
            p.push_back(0);
        }
    }
    currPattern++;
    return p;
}

bool FullPatternGenerator::hasEnded() const {
    return currPattern == max;
}

Generator<std::vector<int>> *FullPatternGenerator::clone() const {
    assert(0);
    return nullptr;
}

InfiniteRandomPatternGenerator::InfiniteRandomPatternGenerator(int nInputs) {
    assert(nInputs > 0);
    this->inputSize = nInputs;
}

int InfiniteRandomPatternGenerator::nInputs() {
    return this->inputSize;
}

std::vector<int> InfiniteRandomPatternGenerator::generate() {
    assert(!this->hasEnded());
    return Random::randomBitVector(inputSize);
}

bool InfiniteRandomPatternGenerator::hasEnded() const {
    return false;
}

Generator<std::vector<int>> *InfiniteRandomPatternGenerator::clone() const {
    assert(0);
    return nullptr;
}

RandomPatternGenerator::RandomPatternGenerator(int nInputs, int nSamples)
        : InfiniteRandomPatternGenerator(nInputs) {
    assert(nSamples >= 0);
    this->nSamples = nSamples;
    currentSampleCount = 0;
}

bool RandomPatternGenerator::hasEnded() const {
    return currentSampleCount == nSamples;
}

std::vector<int> RandomPatternGenerator::generate() {
    assert(!this->hasEnded());
    auto v =  InfiniteRandomPatternGenerator::generate();
    currentSampleCount++;
    return v;
}
