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


class SearchNode : public Object {

public:
    void divide(); //not finished.
    bool divideAble();

private:
    int searchArray[NODE_SIZE];
    std::unique_ptr<BooleanFunction> *booleanFunction;

    // record the best divide.
    int bestLocalErr;
    int bestOper;   // OPERATION_AND, OR, XOR, DROP
    int bestDivide;

    // current divide.
    int currentDivie;



};


class SearchSpace : public Object {

public:
    bool searchSpaceGrow(); // return whether this search space could be divided into different search spaces
    std::unique_ptr<SearchSpace> *searchSpaceGenerate(); // return a search space that is generated from the current one.


private:
    BinaryTree<std::shared_ptr<SearchNode>> *btree;
    SearchNode& divideNode;   // which node is to divide.
    int currentDivide;        // how is the current node is divided.
    //error

    int totalError;

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
