//
// Created by tripack on 16-10-26.
//

#ifndef VE490_SEARCH_H
#define VE490_SEARCH_H

#include "tree.h"
#include "boolean_function.h"
#include "conts.h"
#include "object.h"
#include <memory.h>


class SearchNode {//: public Object {

public:
    void divide(int divideMethod);  //not finished.
    bool divideAble(); // chech whether this node is divideable.

    SearchNode();
    SearchNode(std::unique_ptr<BooleanFunction>);

    std::unique_ptr<BooleanFunction> getBooleanFunction();
    std::unique_ptr<BooleanFunction> combineBooleanFunction();

    int getInputNum();

private:
    std::unique_ptr<BooleanFunction> *booleanFunction;

    // record the best divide.
    int bestLocalErr;
    int bestOper;   // OPERATION_AND, OR, XOR, DROP
    int bestDivide;


};




class SearchSpace : public Object {

public:

    bool searchSpaceGrow();  // whether this search space could be divided or not
    std::unique_ptr<SearchSpace> *searchSpaceGenerate(); // return a search space that is generated from the current one.
    int getTotalError(); // return the total error.


private:
    BinaryTree<std::shared_ptr<SearchNode>> *btree;
    SearchNode &divideNode;   // which node is to divide.
    int currentDivide;        // how is the current node is divided.
    int currentDivideSpace;   // the range for current divide, should be a 2^n number,
                              // the range should be 1~(currentDivideSpace-2). The left tree
                              // or right tree can't be null.
    int calculTotalError(BooleanFunction &initBoolFunc);

    //error
    int totalError; // the total error for this search space
                    // should be calculated after this SearchSpace is created.

};


class SearchTree {

public:

    SearchSpace& getNextSearchSpace(SearchSpace& n); // asking current search space could generate a new one or nor, if could, finish current generating. Back to his parent.
    void eliminateSearchSpace(SearchSpace& n);
    SearchSpace& getRootSpace();

private:

    MulipleTree<std::unique_ptr<SearchSpace>> *mtree;
    SearchSpace& currentSearchSpace;    // the current working on search space.

};

#endif //VE490_SEARCH_H
