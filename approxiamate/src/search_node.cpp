//
// Created by king on 16-11-30.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <values.h>

using namespace std;

//===============Search Node========Search Node Op=================


SearchNode::SearchNode() {
    booleanFunction=NULL;
    bestLocalErr=MAXINT;
    bestOper=OPERATION_NONE;
    bestDivide=0;
    return ;
}

SearchNode::SearchNode(BooleanFunctionPtr ptr) {
    booleanFunction=move(ptr);
    bestLocalErr=MAXINT;
    bestOper=OPERATION_NONE;
    bestDivide=0;
    return ;
}

SearchNode::~SearchNode() {
    return ;
}

int SearchNode::getInputNum() {
    return booleanFunction->getInputNum();
}

int SearchNode::getDivideRange() {
    return ( (1 << booleanFunction->getInputNum()) );
}

bool SearchNode::divideAble(int divideMethod) {
    return ( (1 <= divideMethod) &&
            (divideMethod <= (1 << booleanFunction->getInputNum() - 2)) );
}

shared_ptr<BooleanFunction> SearchNode::getBooleanFunction() {
    return move(booleanFunction->copy());
}

shared_ptr<BooleanFunction> SearchNode::combineBooleanFunction(BooleanFunctionPtr p1,
                                                               BooleanFunctionPtr p2, int oper) {
    return move(p1->combine(*p2, oper));
}

shared_ptr<BooleanFunction> SearchNodeOp::combineBooleanFunction(BooleanFunctionPtr p1,
                                                               BooleanFunctionPtr p2) {
    return move(node->combineBooleanFunction(move(p1), move(p2), oper));
}

tuple<SearchNodeOpPtr, SearchNodeOpPtr, SearchNodeOpPtr>
SearchNodeOp::divide(int method) {

    SearchNodeOpPtr newNodeOp(new SearchNodeOp(node));
    tuple<SearchNodePtr, SearchNodePtr, int, int>
            res = node->divide(method);

    SearchNodeOpPtr nodeOp1(new SearchNodeOp(move(get<0>(res))));
    SearchNodeOpPtr nodeOp2(new SearchNodeOp(move(get<1>(res))));
    localErr=get<2>(res);
    oper=get<3>(res);
    currentDivide=method;

    return make_tuple(newNodeOp, nodeOp1, nodeOp2);

};

tuple<SearchNodePtr,
           SearchNodePtr, int, int>
        SearchNode::divide(int divideMethod) {

    int portSize = booleanFunction->getPortSize();
    int *portName = booleanFunction->getPortName();
    int *part[2];
    part[0] = new int[portSize];
    part[1] = new int[portSize];
    for (int i = 0; i < portSize; ++i) {
        part[0][i]=0; part[1][i]=0;
    }
    int *input= new int[portSize];
    int inputNum=0;

    for (int i = 0; i < portSize; ++i)
        if (portName[i]==1) input[inputNum++] = i;
    int temp=divideMethod;

    for (int i = 0; i < inputNum; i++) {
        part[1-(temp%2)][input[i]] = 1;
    }

    //std::tuple<BooleanFunctionPtr, BooleanFunctionPtr, int, int>
    auto result=booleanFunction->divide(part[0],part[1]);

    delete[] part[0];
    delete[] part[1];
    delete[] input;

    int localErr=get<2>(result);
    int oper=get<3>(result);

    SearchNodePtr node1(
            new SearchNode(move(get<0>(result)))
    );
    SearchNodePtr node2(
            new SearchNode(move(get<1>(result)))
    );

    return make_tuple(node1, node2, localErr, oper);

}

bool SearchNode::isDividable() {
    if (booleanFunction->isAll1s()) return true;
    if (booleanFunction->isAll0s()) return true;
    if (booleanFunction->getInputNum() == 1) return true;
    return false;
}

bool SearchNodeOp::isDiviable() {
    return node->isDividable();
}

SearchNodeOp::SearchNodeOp() {
    node=NULL;
    oper=OPERATION_NONE;
    currentDivide=0;
    localErr=MAXINT;
}

SearchNodeOp::SearchNodeOp(SearchNodePtr nd) {
    node=nd;
    oper=OPERATION_NONE;
    currentDivide=0;
    localErr=MAXINT;
}

SearchNodeOp::~SearchNodeOp() {
    return ;
}