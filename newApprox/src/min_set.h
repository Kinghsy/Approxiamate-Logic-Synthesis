//
// Created by king on 17-4-20.
//

#ifndef VE490_MIN_SET_H
#define VE490_MIN_SET_H

#include <vector>

template <class T>
class MinSet {

private:

    std::vector<T> data;
    int len;

public:

    MinSet(const int s) {
        len = s;
    }

    int size() {
        return data.size();
    }

    void push(const T& t) {
        std::vector<T> temp;
        while ( (data.size() > 0) && (data.back() > t)) {
            temp.push_back(data.back());
            data.pop_back();
        }
        data.push_back(t);
        while ( temp.size() > 0 ) {
            data.push_back(temp.back());
            temp.pop_back();
        }
        if (data.size() > len) data.pop_back();
    }

    void relen(const int s) {
        len = s;
        while (data.size() > len) data.pop_back();
    }

    void popBack() {
        data.pop_back();
    }

    const T& back() const {
        return data.back();
    }

    T& begin() const {
        return data.begin();
    }

    T& operator[] (const int i) const {
        return data[i];
    };

};

#endif //VE490_MIN_SET_H
