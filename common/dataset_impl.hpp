//
// Created by tripack on 16-10-12.
//

template <class T>
Dataset<T>::Dataset(Generator<T>& g) :
    Dataset() {
    while (!g.hasEnded()) {
        T val = g.generate();
        data.push_back(val);
    }
}

template <class T>
Dataset<T>::Dataset() {}

template <class T>
int Dataset<T>::getSize() const{
    return (int)data.size();
}

template <class T>
const T *Dataset<T>::getData() const{
    return data.data();
}

template <class T>
T *Dataset<T>::getCopied() const{
    int size = data.size();
    T *odata = new T[size];
    std::copy(data.begin(), data.end(),
              odata);
    return odata;
}


template <class T>
Dataset<T>::~Dataset() {
    data.clear();
}

template <class T>
void Dataset<T>::copyfrom(const Dataset <T> &ds) {
    data = ds.data;
}

template <class T>
Dataset<T> &Dataset<T>::operator=(const Dataset <T> &ds) {
    copyfrom(ds);
    return *this;
}

template <class T>
SortableDataset<T>::SortableDataset(Generator<T>& g) :
        Dataset<T>(g), isSorted(false) { }

template <class T>
SortableDataset<T>::SortableDataset() :
        Dataset<T>(), isSorted(false) { }

template <class T>
SortableDataset<T> &SortableDataset<T>::operator=(const Dataset<T> &ds) {
    const SortableDataset<T> &sds = dynamic_cast<const SortableDataset<T>&>(ds);
    Dataset<T>::operator=(sds);
    isSorted = sds.isSorted;
    if (isSorted) sortedData = sds.sortedData;
}

template <class T>
const T* SortableDataset<T>::getSorted() {
    if (isSorted) return sortedData.data();
    sortedData = this->data;
    std::sort(sortedData.begin(), sortedData.end());
    isSorted = true;
    return sortedData.data();
}

template <class T>
T SortableDataset<T>::select(int order) {
    if (order >= this->data.size())
        throw ExceptionOutOfRange();
    const T* sorted = getSorted();
    return sorted[order];
}

template <class T>
SortableDataset<T>::~SortableDataset() {
    sortedData.clear();
}


