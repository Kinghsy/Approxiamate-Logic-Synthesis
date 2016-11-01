//
// Created by tripack on 16-10-12.
//

#ifndef VE281_FA16_DATASET_H
#define VE281_FA16_DATASET_H

#include <vector>
#include <algorithm>
#include "exception.h"
#include "generator.h"

template <class T>
class Dataset {
public:
    virtual int         getSize() const;
    virtual const T*    getData() const;
    virtual T*          getCopied() const;

    Dataset(Generator<T>& g);
    Dataset();
    virtual Dataset& operator=(const Dataset& ds);
    virtual ~Dataset();

protected:
    void copyfrom(const Dataset<T>& ds);

    std::vector<T> data;
};

template <class T>
class SortableDataset : public Dataset<T> {
public:
    SortableDataset(Generator<T>& g);
    SortableDataset();
    virtual SortableDataset& operator=(const Dataset<T>& ds);
    virtual ~SortableDataset();

    const T *getSorted();
    T select(int order);

protected:
    std::vector<T> sortedData;
    bool isSorted;
};


#include "dataset_impl.hpp"
// Actual implementation

#endif //VE281_FA16_DATASET_H
