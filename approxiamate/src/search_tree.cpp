//
// Created by king on 16-12-5.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"

#include <graph_common.h>
#include <binary_tree_impl.h>

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>

using namespace std;

//=======search tree=========================

SearchTree::SearchTree() {
    mtree = NULL;
    currentVertexID = 0;
}

SearchTree::SearchTree(BooleanFunction &initBoolFunc) {
    unique_ptr<BooleanFunction> boolFunc(new BooleanFunction(initBoolFunc));
    shared_ptr<SearchNode> initSearchNode(new SearchNode(boolFunc));
    shared_ptr<SearchNodeOp> initSearchNodeOp(new SearchNodeOp(initSearchNode));
    BinaryTree<shared_ptr<SearchNodeOp> > *bTree=new MapBasedBinaryTree(initSearchNodeOp);
    shared_ptr<SearchSpace> initSearchSpace(bTree);
    mtree = new MapBasedTree(initSearchSpace);
    // TODO finish the left part of MapBasedTree
    currentVertexID = mtree -> root();
    delete bTree;

}

SearchTree::~SearchTree() {
    return ;
}

shared_ptr<SearchSpace> SearchTree::getRootSpace() {
    return (mtree->valueOf(mtree->root()));
}

shared_ptr<SearchSpace> SearchTree::getNextSearchSpace() {
    SearchSpace& currentSearchSpace = *(mtree->valueOf(currentVertexID));
    shared_ptr<SearchSpace> newSpace = currentSearchSpace.searchSpaceGenerate();
    if (newSpace!=NULL) mtree->addAsChildren(currentVertexID, newSpace);
    return newSpace;
}

shared_ptr<SearchSpace> SearchTree::getNextSearchSpace(int method) {
    SearchSpace& currentSearchSpace = *(mtree->valueOf(currentVertexID));
    shared_ptr<SearchSpace> newSpace = currentSearchSpace.searchSpaceGenerate(method);
    if (newSpace!=NULL) mtree->addAsChildren(currentVertexID, newSpace);
    return newSpace;
}

shared_ptr<SearchSpace> SearchTree::getCurrentSearchSpace() {
    return (mtree->valueOf(currentVertexID));
}

SearchSpace& SearchTree::getMinTotalError() {

    shared_ptr<SearchSpace> res = getMinTotalErrorHelper(mtree->root());
    return *res;

}

shared_ptr<SearchSpace> SearchTree::getMinTotalErrorHelper(
        Tree<shared_ptr<SearchSpace> >::VertexID_t node) {

    vector<Tree<shared_ptr<SearchSpace> >::VertexID_t >
            res = mtree->getChild(node);
    if (res.empty) return mtree->valueOf(node);

    shared_ptr<SearchSpace> record=NULL;
    for (auto iter : res) {
        shared_ptr<SearchSpace> tmp=getMinTotalErrorHelper(iter);
        if ((record == NULL) || (tmp->getTotalError() < record->getTotalError()))
            record = tmp;
    }
    return record;
}
