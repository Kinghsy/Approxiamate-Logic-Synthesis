//
// Created by king on 16-12-5.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"

#include "../../common/BinaryTree.h"

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <values.h>

using namespace std;

unique_ptr<BooleanFunction> calculTotalErrorHelper(
        BinaryTree<shared_ptr<SearchNode>>::iterator binaryTreeNode);

//==========Search Space===================

SearchSpace::SearchSpace(BinaryTree<shared_ptr<SearchNode>> &oldTree) {
    btree=new BinaryTree<shared_ptr<SearchNode>> (oldTree);
    divideNode=*(btree->findLeaf()); ////?????????????????
    currentDivide=1;
    currentDivideSpace=divideNode->getDivideRange();
    totalError=calculTotalError(initBoolFunc);
}

SearchSpace::SearchSpace() {
    btree=NULL;
    divideNode=NULL;
    currentDivide=0;
    currentDivideSpace=0;
    totalError=0;
}

SearchSpace::~SearchSpace() {
    return ;
}

bool SearchSpace::searchSpaceGrow() {
    if (currentDivide > currentDivideSpace - 2) return false;
    return true;
}

unique_ptr<SearchSpace> SearchSpace::searchSpaceGenerate() {
    return searchSpaceGenerate(currentDivide++);
}

unique_ptr<SearchSpace> SearchSpace::searchSpaceGenerate(int divideMethod) {
    tuple<shared_ptr<SearchNode>, shared_ptr<SearchNode>>
            divideRes = divideNode->divide(divideMethod);
    shared_ptr<SearchNode> leftNode = move(get<0>(divideRes));
    shared_ptr<SearchNode> rightNode = move(get<1>(divideRes));

    BinaryTree<shared_ptr<SearchNode>> bTreeTmp(*btree);
    BinaryTree<shared_ptr<SearchNode>>::iterator dealNode=bTreeTmp.findLeaf(); ////??????????
    bTreeTmp.addTwoLeaves(dealNode, leftNode, rightNode);

    unique_ptr<SearchSpace> newSearchSpace=new SearchSpace(bTreeTmp);
    return move(newSearchSpace);
}

int SearchSpace::getTotalError() {
    return totalError;
}

int SearchSpace::calculTotalError(BooleanFunction &initBoolFunc) {

    unique_ptr<BooleanFunction>
            result = move(calculTotalErrorHelper(btree->getRoot()));

    int sum = initBoolFunc.booleanCompare(*result);
    return sum;

}

unique_ptr<BooleanFunction> calculTotalErrorHelper(
        BinaryTree<shared_ptr<SearchNode>>::iterator binaryTreeNode) {
    if (binaryTreeNode->isLeaf()) return ????
    unique_ptr<BooleanFunction> left = move(binaryTreeNode->left);
    unique_ptr<BooleanFunction> right = move(binaryTreeNode->right);
    return ?????
}

