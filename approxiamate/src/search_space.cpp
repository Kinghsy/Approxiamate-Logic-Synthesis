//
// Created by king on 16-12-5.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"

#include <binary_tree_impl.h>
#include <graph_common.h>

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <stack>

using namespace std;

//==========Search Space===================

SearchSpace::SearchSpace(BinaryTree<SearchNodeOpPtr > &oldTree) {
    btree = unique_ptr<BinaryTree<SearchNodeOpPtr > > (oldTree.clone());
    // FIXME add polymorphic copy constructor
    divideNode=btree->valueOf(findDivideNode());
    currentDivide=1;
    if (growAble) currentDivideRange=divideNode->node->getDivideRange();
    totalError=calculTotalError(initBoolFunc);
}

SearchSpace::SearchSpace() {
    btree=NULL;
    divideNode=NULL;
    currentDivide=0;
    currentDivideRange=0;
    totalError=0;
    growAble=false;
}

SearchSpace::~SearchSpace() {
    return ;
}

bool SearchSpace::searchSpaceGrow() {
    if (!growAble) return growAble;
    if (currentDivide > currentDivideRange - 2) return false;
    return true;
}

SearchSpacePtr SearchSpace::searchSpaceGenerate() {
    return searchSpaceGenerate(currentDivide++);
}

SearchSpacePtr SearchSpace::searchSpaceGenerate(int divideMethod) {

    if (divideMethod > currentDivideRange-2) return NULL;
    if (divideMethod < 1) return NULL;

    tuple<SearchNodeOpPtr, SearchNodeOpPtr, SearchNodeOpPtr>
            divideRes = divideNode->divide(divideMethod);

    SearchNodeOpPtr newNodeOp = move(get<0>(divideRes));
    SearchNodeOpPtr leftNodeOp = move(get<1>(divideRes));
    SearchNodeOpPtr rightNodeOp = move(get<2>(divideRes));

    unique_ptr<BinaryTree<SearchNodeOpPtr> > newBTree(btree->clone());
    BinaryTree<SearchNodeOpPtr>::VertexID_t treeNodeID = findDivideNode();
    btree->addLeft(treeNodeID, leftNodeOp);
    btree->addRight(treeNodeID, rightNodeOp);

    unique_ptr<BinaryTree<SearchNodeOpPtr>> tmpBTree;

    tmpBTree = move(btree);
    btree = move(newBTree);
    newBTree = move(tmpBTree);

    treeNodeID = findDivideNode();
    btree->valueOf(treeNodeID) = newNodeOp;

    SearchSpacePtr newSearchSpace(new SearchSpace(*tmpBTree));
    return move(newSearchSpace);
}

int SearchSpace::getTotalError() {
    return totalError;
}

BinaryTree<SearchNodeOpPtr>::VertexID_t SearchSpace::findDivideNode() {
    BinaryTree<SearchNodeOpPtr>::VertexID_t node;
    node = btree->root();
    if (node==btree->nullId())
        growAble=false;
    else
        growAble=true;
    return findDivideNodeHelper(node);
}

BinaryTree<SearchNodeOpPtr>::VertexID_t SearchSpace::findDivideNodeHelper(
        BinaryTree<SearchNodeOpPtr>::VertexID_t node) {
    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        BinaryTree<SearchNodeOpPtr>::VertexID_t leftRes=findDivideNodeHelper(btree->left(node));
        if (leftRes!=btree->nullId()) return leftRes;
        BinaryTree<SearchNodeOpPtr>::VertexID_t rightRes=findDivideNodeHelper(btree->right(node));
        if (rightRes!=btree->nullId()) return rightRes;
        return btree->nullId();
    }
    if (btree->valueOf(node)->isDiviable()) return node;
    return btree->nullId();
}

int SearchSpace::calculTotalError(BooleanFunction &initBoolFunc) {

    shared_ptr<BooleanFunction> result = calculTotalErrorHelper(btree->root());
    int sum = initBoolFunc.booleanCompare(*result);
    return sum;

}

shared_ptr<BooleanFunction> SearchSpace::calculTotalErrorHelper(
        BinaryTree<SearchNodeOpPtr>::VertexID_t node) {

    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        shared_ptr<BooleanFunction> r1 = calculTotalErrorHelper(btree->left(node));
        shared_ptr<BooleanFunction> r2 = calculTotalErrorHelper(btree->right(node));
        shared_ptr<BooleanFunction> res = r1->combine(*r2, btree->valueOf(node)->oper);
    }

    return move(btree->valueOf(node)->node->getBooleanFunction());
}
