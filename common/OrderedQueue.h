//
// Created by king on 17-2-27.
//

#ifndef VE490_ORDEREDQUEUE_H
#define VE490_ORDEREDQUEUE_H

#include <stack>
#include <deque>
#include <cassert>
#include <memory>

template <class T>
class OrderedQueue {

private:
    std::deque<T> oq;

public:
    OrderedQueue() {
    }
    OrderedQueue(const T& tempT) {
        push(tempT);
    }

    OrderedQueue& operator= (const OrderedQueue) = delete;
    OrderedQueue(OrderedQueue&& tempQueue) {
        this -> oq = tempQueue.oq;
    }

    ~OrderedQueue() {
    }

    //==============================================

    void clearAll() {
        while (!oq.empty()) oq.pop_front();
    }
    void push(const T& tempT) {
        // FIXME to be added
        std::stack<T> tempQ;
        while ((!oq.empty()) && (oq.back() > tempT)) {
            tempQ.push(oq.back());
            oq.pop_back();
        }
        oq.push_back(tempT);
        while (!tempQ.empty()) {
            oq.push_back(tempQ.top());
            tempQ.pop();
        }
    }
    T& val_back() {
        return oq.back();
    }
    void pop_back() {
        oq.pop_back();
    }
    T& val_front() {
        return oq.front();
    }
    void pop_front() {
        oq.pop_front();
    }
    int size() {
        return oq.size();
    }

};


#endif //VE490_ORDEREDQUEUE_H
