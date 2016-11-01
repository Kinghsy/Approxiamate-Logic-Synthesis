//
// Created by patri on 2016/10/13.
//

#ifndef VE281_FA16_GENERATOR_H
#define VE281_FA16_GENERATOR_H
#include "exception.h"
#include <cstdlib>

template <class T>
class Generator {
public:
    virtual T generate() = 0;
    virtual bool hasEnded() const= 0;
    virtual Generator<T>* clone() const = 0;
};

template <class T>
class StaticSequenceGenerator : public Generator<T> {
    const T* data;
    int size;
    int count;
    StaticSequenceGenerator(const T* data_, int size_)
            : data(data_), size(size_), count(0) {};
public:
    template <size_t n>
    StaticSequenceGenerator(const T (&seq)[n]) :
            size(n), count(0) { data = (const T*)&seq; }

    T generate() {
        if (!hasEnded()) return data[count++];
        throw ExceptionGeneratorEmpty();
    }

    bool hasEnded() const { return count >= size; }

    Generator<T>* clone() const {
        return new StaticSequenceGenerator (data, size);
    }

    int getSize() {return size;}


};

class RandomIntGenerator : public Generator<int> {
    int count;
    int size;
public:
    RandomIntGenerator(int size_)
            : count(0), size(size_) {};
    int generate() {
        if (!hasEnded()) { count++; return mrand48(); }
        throw ExceptionGeneratorEmpty();
    }
    bool hasEnded() const {return count >= size;}
    Generator<int>* clone() const {
        return new RandomIntGenerator(size);
    }
};

#endif //VE281_FA16_GENERATOR_H
