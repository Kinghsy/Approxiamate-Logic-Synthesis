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

SearchSpace::SearchSpace(BinaryTree<shared_ptr<SearchNodeOp> > &oldTree) {
    btree = unique_ptr<BinaryTree<shared_ptr<SearchNodeOp> > > (oldTree.clone());
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

shared_ptr<SearchSpace> SearchSpace::searchSpaceGenerate() {
    return searchSpaceGenerate(currentDivide++);
}

shared_ptr<SearchSpace> SearchSpace::searchSpaceGenerate(int divideMethod) {

    if (divideMethod > currentDivideRange-2) return NULL;
    if (divideMethod < 1) return NULL;

    tuple<shared_ptr<SearchNodeOp>, shared_ptr<SearchNodeOp>, shared_ptr<SearchNodeOp>>
            divideRes = divideNode->divide(divideMethod);

    shared_ptr<SearchNodeOp> newNodeOp = move(get<0>(divideRes));
    shared_ptr<SearchNodeOp> leftNodeOp = move(get<1>(divideRes));
    shared_ptr<SearchNodeOp> rightNodeOp = move(get<2>(divideRes));

    unique_ptr<BinaryTree<shared_ptr<SearchNodeOp>>> newBTree(*btree);
    BinaryTree<shared_ptr<SearchNodeOp>>::VertexID_t treeNodeID = findDivideNode();
    shared_ptr &refer = btree->valueOf(treeNodeID);
    refer = newNodeOp;
    btree->addLeft(treeNodeID, leftNodeOp);
    btree->addRight(treeNodeID, rightNodeOp);

    unique_ptr<BinaryTree<shared_ptr<SearchNodeOp>>> tmpBTree;

    tmpBTree = move(btree);
    btree = move(newBTree);
    newBTree = move(tmpBTree);

    shared_ptr<SearchSpace> newSearchSpace(new SearchSpace(*tmpBTree));
    return move(newSearchSpace);
}

int SearchSpace::getTotalError() {
    return totalError;
}

BinaryTree<shared_ptr<SearchNodeOp>>::VertexID_t SearchSpace::findDivideNode() {
    BinaryTree<shared_ptr<SearchNodeOp>>::VertexID_t node;
    node = btree->root();
    if (node==btree->nullId())
        growAble=false;
    else
        growAble=true;
    return findDivideNodeHelper(node);
}

BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t SearchSpace::findDivideNodeHelper(
        BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t node) {
    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t leftRes=findDivideNodeHelper(btree->left(node));
        if (leftRes!=btree->nullId()) return leftRes;
        BinaryTree<std::shared_ptr<SearchNodeOp>>::VertexID_t rightRes=findDivideNodeHelper(btree->right(node));
        if (rightRes!=btree->nullId()) return rightRes;
        return btree->nullId();
    }
    if (btree->valueOf(node)->isDiviable()) return node;
    return btree->nullId();
}

int SearchSpace::calculTotalError(BooleanFunction &initBoolFunc) {

    unique_ptr<BooleanFunction> result = move(calculTotalErrorHelper(btree->root()));
    int sum = initBoolFunc.booleanCompare(*result);
    return sum;

}

unique_ptr<BooleanFunction> SearchSpace::calculTotalErrorHelper(
        BinaryTree<shared_ptr<SearchNodeOp>>::VertexID_t node) {

    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        unique_ptr<BooleanFunction> r1 = calculTotalErrorHelper(btree->left(node));
        unique_ptr<BooleanFunction> r2 = calculTotalErrorHelper(btree->right(node));
        unique_ptr<BooleanFunction> res = r1->combine(*r2, btree->valueOf(node)->oper);
    }

    return move(btree->valueOf(node)->node->getBooleanFunction());
}
