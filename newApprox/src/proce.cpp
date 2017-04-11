//
// Created by king on 17-4-11.
//

#include <vector>
#include <stack>
#include <boost/shared_ptr.hpp>

#include "kmap.h"
#include "bool_function.h"
#include "node.h"
#include "proce.h"

using namespace std;

void wholeProcess::freeStack() {
    while (nowRun.size() > 0)
        nowRun.pop();
}

wholeProcess::wholeProcess() {
    head = nullptr;
    freeStack();
}

wholeProcess::wholeProcess(const DecomposedFuncPtr &initPtr) {
    head = initPtr;
    freeStack();
    nowRun.push(head);
}

wholeProcess::wholeProcess(const DecomposedFunc &initDF) {
    head = DecomposedFunc(&initDF);
    freeStack();
    nowRun.push(head);
}

wholeProcess::wholeProcess(const BoolFunction &initFun) {
    head = DecomposedFunc(new DecomposedFunc(initFun));
    freeStack();
    nowRun.push(head);
}

wholeProcess::~wholeProcess() {
    freeStack();
    free(head);
}

void wholeProcess::operator=(const wholeProcess &initWP) {
    head = DecomposedFunc(new DecomposedFunc());
    (*head) = *(initWP.head);
}

bool wholeProcess::operator==(const wholeProcess &initWP) {
    return ( (head->combine())^((initWP.head)->combine()) ) == 0 ;
}

bool wholeProcess::fullyDecomposed() {
    vector<DecomposedFuncPtr > travel = head->inOrderTravel();
    bool flag = true;
    for (auto& item:travel) {
        if (!(item->isLeaf) && (*(*item).divideAble)) flag = false;
    }
    return flag;
}

int wholeProcess::diff() {
    return ( (head->combine()) ^ *(*head));
}

bool wholeProcess::grow() {

}

