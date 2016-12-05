//
// Created by tripack on 16-10-26.
//

#ifndef VE490_SEARCH_H
#define VE490_SEARCH_H

#include "boolean_function.h"
#include "conts.h"
#include "object.h"
#include <memory.h>

#include "../../common/BinaryTree.h"
#include "../../common/MultipleTree.h"

class SearchNode {//: public Object {

public:
    std::tuple<std::shared_ptr<SearchNode>, std::shared_ptr<SearchNode>>
            divide(int divideMethod);  // return two boolean function.
    bool divideAble(int divideMethod); // chech whether this node is divideable.

    SearchNode();
    SearchNode(std::unique_ptr<BooleanFunction> ptr);
    ~SearchNode();

    std::unique_ptr<BooleanFunction> getBooleanFunction();
        // create!!! notice this function create a new booleanfunction
        // that has exactly same information as itself.
    std::unique_ptr<BooleanFunction> combineBooleanFunction(
                std::unique_ptr<BooleanFunction> p1,
                std::unique_ptr<BooleanFunction> p2);
        // combine p1 and p2 together based on the bestOper.
        // note that this node can't be a leaf node.
    int getDivideRange(); // return 2^(getInputNum)
    int getInputNum();

    bool isDividable();


private:

    std::unique_ptr<BooleanFunction> booleanFunction;
    // record the best divide.
    int localErr;
    int oper;   // OPERATION_AND, OR, XOR, DROP
    int currentDivide;

};


class SearchSpace {//: public Object {

public:

    SearchSpace(BinaryTree<std::shared_ptr<SearchNode>> &oldTree);
    SearchSpace();
    ~SearchSpace();

    bool searchSpaceGrow();  // whether this search space could be divided or not
    std::unique_ptr<SearchSpace> searchSpaceGenerate(); // return a search space that is generated from the current one.
    std::unique_ptr<SearchSpace> searchSpaceGenerate(int divideMethod);
    int getTotalError(); // return the total error.


private:
    std::unique_ptr<BinaryTree<std::shared_ptr<SearchNode>>> btree;
    std::shared_ptr<SearchNode> divideNode;   // which node is to divide.
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

    SearchTree();
    ~SearchTree();

    SearchSpace& getNextSearchSpace(SearchSpace& n); // asking current search space could generate a new one or nor, if could, finish current generating. Back to his parent.
    void eliminateSearchSpace(SearchSpace& n);
    SearchSpace& getRootSpace();

private:

    MultipleTree<std::unique_ptr<SearchSpace>> *mtree;
    SearchSpace& currentSearchSpace;    // the current working on search space.

};

#endif //VE490_SEARCH_H
