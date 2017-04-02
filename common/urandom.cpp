//
// Created by tripack on 17-4-2.
//
#include "urandom.h"
#include <vector>
#include <random>
#include <bitset>

static std::mt19937 engine(std::random_device{}());
int count = 0;

int Random::randomInt() {
    return engine();
}

std::vector<int> Random::randomIntVector(int size) {
    std::vector<int> v;
    for (int i = 0; i < size; ++i) {
        v.push_back(engine());
    }
    return v;
}

std::vector<int> Random::randomBitVector(int size) {
    std::bernoulli_distribution d;
    std::vector<int> v;
    for (int i = 0; i < size; ++i) {
        v.push_back(d(engine));
    }
    return v;
}

template<size_t l>
std::bitset<l> Random::randomBitset() {
    std::bernoulli_distribution d;
    std::bitset<l> v;
    for (int i = 0; i < v.size(); ++i) {
        v[i] = d(engine);
    }
    return v;
}

