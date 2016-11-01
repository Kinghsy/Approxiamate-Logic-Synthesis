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

//SearchSpace& sspace;
//MulipleTree<SearchNode>::NodeRef node;


using std::shared_ptr;
using std::unique_ptr;

class SearchNode : public Object {

public:
    void divide(); //not finished.
    bool divideAble();

private:
    int searchArray[NODE_SIZE];
    unique_ptr<BooleanFunction> *booleanFunction;

};


class SearchSpace : public Object {

public:
    bool searchSpaceGrow(); // return whether this search space could be divided into different search spaces
    unique_ptr<SearchSpace> *searchSpaceGenerate(); // return a search space that is generated from the current one.


private:
    BinaryTree<shared_ptr<SearchNode>> *btree;
    SearchNode& divideNode;   // which node is to divide.
    int currentDivide;        // how is the current node is divided.
    //error

};


class SearchTree {

public:

    SearchSpace& getNextSearchSpace(SearchSpace& n); // asking current search space could generate a new one or nor, if could, finish current generating. Back to his parent.
    void eliminateSearchSpace(SearchSpace& n);
    SearchSpace& getRootSpace();

private:
    MulipleTree<unique_ptr<SearchSpace>> *mtree;
    unique_ptr<SearchSpace> currentSearchSpace;

};

#endif //VE490_SEARCH_H
