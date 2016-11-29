//
// Created by king on 16-11-30.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"
#include "tree.h"

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <values.h>

using std::unique_ptr;
using std::shared_ptr;
using std::tuple;
using std::tie;
using std::get;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::cout;


//===============Search Node============================


SearchNode::SearchNode() {
    booleanFunction=NULL;
    bestLocalErr=MAXINT;
    bestOper=OPERATION_NONE;
    bestDivide=0;
    return ;
}

SearchNode::SearchNode(std::unique_ptr<BooleanFunction> ptr) {
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
            (divideMethod <= (1 << booleanFunction->getInputNum())) );
}

unique_ptr<BooleanFunction> SearchNode::getBooleanFunction() {
    return move(booleanFunction->copy());
}

unique_ptr<BooleanFunction> SearchNode::combineBooleanFunction(std::unique_ptr<BooleanFunction> p1,
                                                               std::unique_ptr<BooleanFunction> p2) {
    return move(p1->combine(*p1, bestOper));
}

std::tuple<std::unique_ptr<BooleanFunction>,
           std::unique_ptr<BooleanFunction>>
        SearchNode::divide(int divideMethod) {

    int portSize = booleanFunction->getPortSize();



}
