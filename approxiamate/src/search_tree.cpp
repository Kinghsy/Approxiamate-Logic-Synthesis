//
// Created by king on 16-12-5.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"

#include <graph_common.h>
#include <binary_tree_impl.h>
#include <tree_impl.h>


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
    BooleanFunctionPtr boolFunc(new BooleanFunction(initBoolFunc));
    SearchNodePtr initSearchNode(new SearchNode(move(boolFunc)));
    SearchNodeOpPtr initSearchNodeOp(new SearchNodeOp(initSearchNode));
    unique_ptr<BinaryTree<SearchNodeOpPtr> > Btree(
            new MapBasedBinaryTree<SearchNodeOpPtr>(initSearchNodeOp)
    );
    SearchSpacePtr initSearchSpace = SearchSpacePtr(
            new SearchSpace(move(Btree))
    );

    /*SearchSpacePtr initSearchSpace = shared_ptr<SearchNodeOpPtr>( new SearchNodeOp (
            unique_ptr<BinaryTree<SearchNodeOpPtr> >(new MapBasedBinaryTree<SearchNodeOpPtr>(initSearchNodeOp) )
    ));*/
    mtree = unique_ptr<Tree<SearchSpacePtr>>(
            new MapBasedTree<SearchSpacePtr>(initSearchSpace)
    );
    // TODO finish the left part of MapBasedTree
    currentVertexID = mtree -> root();

    //delete bTree;
}

SearchTree::~SearchTree() {
    return ;
}

SearchSpacePtr SearchTree::getRootSpace() {
    return (mtree->valueOf(mtree->root()));
}

SearchSpacePtr SearchTree::getNextSearchSpace() {
    SearchSpace& currentSearchSpace = *(mtree->valueOf(currentVertexID));
    SearchSpacePtr newSpace = currentSearchSpace.searchSpaceGenerate();
    if (newSpace!=NULL)
        mtree->addAsChildren(currentVertexID, newSpace);
    else {
        if (mtree->isRoot(currentVertexID)) return NULL;
        currentVertexID=mtree->getParent(currentVertexID);
        return getNextSearchSpace();
    }
    return newSpace;
}

SearchSpacePtr SearchTree::getNextSearchSpace(int method) {
    SearchSpace& currentSearchSpace = *(mtree->valueOf(currentVertexID));
    SearchSpacePtr newSpace = currentSearchSpace.searchSpaceGenerate(method);
    if (newSpace!=NULL) mtree->addAsChildren(currentVertexID, newSpace);
    return newSpace;
}

SearchSpacePtr SearchTree::getCurrentSearchSpace() {
    return (mtree->valueOf(currentVertexID));
}

SearchSpace& SearchTree::getMinTotalError() {

    SearchSpacePtr res = getMinTotalErrorHelper(mtree->root());
    return *res;

}

SearchSpacePtr
SearchTree::getMinTotalErrorHelper(Tree<SearchSpacePtr >::VertexID_t node) {
    vector<Tree<SearchSpacePtr>::VertexID_t >
            res = mtree->getChild(node);
    if (res.empty()) return mtree->valueOf(node);

    SearchSpacePtr record= nullptr;
    for (auto iter : res) {
        SearchSpacePtr tmp=getMinTotalErrorHelper(iter);
        if ((record == nullptr) || (tmp->getTotalError() < record->getTotalError()))
            record = tmp;
    }
    return record;
}
