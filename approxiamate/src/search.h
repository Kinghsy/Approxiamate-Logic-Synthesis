//
// Created by tripack on 16-10-26.
//

#ifndef VE490_SEARCH_H
#define VE490_SEARCH_H

#include "boolean_function.h"
#include "conts.h"
#include "object.h"
#include <memory.h>


#include <graph_common.h>
#include <binary_tree_impl.h>

class SearchNode {//: public Object {

public:
    std::tuple<std::shared_ptr<SearchNode>, std::shared_ptr<SearchNode>, int, int>
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
                std::unique_ptr<BooleanFunction> p2, int oper);
        // combine p1 and p2 together based on the bestOper.
        // note that this node can't be a leaf node.
    int getDivideRange(); // return 2^(getInputNum)
    int getInputNum();

    bool isDividable();

private:

    std::unique_ptr<BooleanFunction> booleanFunction;
    // record the best divide.
    int bestLocalErr;
    int bestOper;   // OPERATION_AND, OR, XOR, DROP
    int bestDivide;

};

class SearchNodeOp {
public:
    SearchNodeOp();
    ~SearchNodeOp();
    SearchNodeOp(std::shared_ptr<SearchNode> nd);

    std::unique_ptr<BooleanFunction> combineBooleanFunction(
            std::unique_ptr<BooleanFunction> p1,
            std::unique_ptr<BooleanFunction> p2);

    std::tuple<std::shared_ptr<SearchNodeOp>, std::shared_ptr<SearchNodeOp>, std::shared_ptr<SearchNodeOp>>
        divide(int method);
    // a new opnode, its left opnode and its right opnode

    bool isDiviable();

    std::shared_ptr<SearchNode> node;
    int oper;
    int currentDivide;
    int localErr;
};


class SearchSpace {//: public Object {

public:

    SearchSpace(BinaryTree<std::shared_ptr<SearchNodeOp>> &oldTree);
    SearchSpace();
    ~SearchSpace();

    BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t findDivideNode();
    // return the vertexID of divide node
    bool searchSpaceGrow();  // whether this search space could be divided or not
    std::shared_ptr<SearchSpace> searchSpaceGenerate(); // return a search space that is generated from the current one.
    std::shared_ptr<SearchSpace> searchSpaceGenerate(int divideMethod);
    int getTotalError(); // return the total error.


private:
    std::unique_ptr<BinaryTree<std::shared_ptr<SearchNodeOp> > > btree;
    std::shared_ptr<SearchNodeOp> divideNode;   // which node is to divide.
    int currentDivide;        // how is the current node is divided.
    int currentDivideRange;   // the range for current divide, should be a 2^n number,
                              // the range should be 1~(currentDivideRange-2). The left tree
                              // or right tree can't be null.
    bool growAble;
    int calculTotalError(BooleanFunction &initBoolFunc);
    BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t
        findDivideNodeHelper(BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t node);
    std::unique_ptr<BooleanFunction>
        calculTotalErrorHelper(BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t node);

    //error
    int totalError; // the total error for this search space
                    // should be calculated after this SearchSpace is created.

};


class SearchTree {

public:

    SearchTree();
    SearchTree(BooleanFunction &initBoolFunc);
    ~SearchTree();

    std::shared_ptr<SearchSpace> getNextSearchSpace(); // asking current search space could generate a new one or nor, if could, finish current generating. Back to his parent.
    std::shared_ptr<SearchSpace> getNextSearchSpace(int method);
    std::shared_ptr<SearchSpace> getCurrentSearchSpace(); //
    std::shared_ptr<SearchSpace> getRootSpace();
    SearchSpace &getMinTotalError();

private:

    std::shared_ptr<SearchSpace> getMinTotalErrorHelper(Tree<std::shared_ptr<SearchSpace> >::VertexID_t node)

    std::unique_ptr<Tree<std::shared_ptr<SearchSpace>>> mtree;
    //std::shared_ptr<SearchSpace> currentSearchSpace;    // the current working on search space.
    Tree<std::shared_ptr<SearchSpace> >::VertexID_t currentVertexID;

};

#endif //VE490_SEARCH_H
