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
    recordBestSearchSpacePtr= nullptr;
    mtree = nullptr;
    currentVertexID = 0;
}

SearchTree::SearchTree(BooleanFunction &initBoolFunc) {
    BooleanFunctionPtr boolFunc(new BooleanFunction(initBoolFunc));
    SearchNodePtr initSearchNode(new SearchNode(boolFunc));
    SearchNode &referInitSearchNode=*initSearchNode;
    SearchNodeOpPtr initSearchNodeOp(new SearchNodeOp(initSearchNode));
    SearchNodeOp &referInitSearchNodeOp=*initSearchNodeOp;
    unique_ptr<BinaryTree<SearchNodeOpPtr> > Btree(
            new MapBasedBinaryTree<SearchNodeOpPtr>(initSearchNodeOp)
    );
    BinaryTree<SearchNodeOpPtr> &referBtree=*Btree;
    SearchSpacePtr initSearchSpace = SearchSpacePtr(
            new SearchSpace(move(Btree))
    );
    SearchSpace &referInitSearchSpace = *initSearchSpace;

    /*SearchSpacePtr initSearchSpace = shared_ptr<SearchNodeOpPtr>( new SearchNodeOp (
            unique_ptr<BinaryTree<SearchNodeOpPtr> >(new MapBasedBinaryTree<SearchNodeOpPtr>(initSearchNodeOp) )
    ));*/
    mtree = unique_ptr<Tree<SearchSpacePtr>>(
            new MapBasedTree<SearchSpacePtr>(initSearchSpace)
    );

    currentVertexID = mtree -> root();
    recordBestSearchSpacePtr= mtree->valueOf(mtree->root());
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

    if (newSpace!=nullptr) {
        if (newSpace->isAtLowestLevel()) {
            if ((recordBestSearchSpacePtr== nullptr)
                || (recordBestSearchSpacePtr == mtree->valueOf(mtree->root()))
                || (newSpace->getTotalError() < recordBestSearchSpacePtr->getTotalError()))
                recordBestSearchSpacePtr = newSpace;
        }
        mtree->addAsChildren(currentVertexID, newSpace);
        vector<Tree<SearchSpacePtr>::VertexID_t > vec=mtree->getChild(currentVertexID);
        currentVertexID = *(vec.end()-1);
    } else {
        if (mtree->isRoot(currentVertexID)) return nullptr;
        Tree<SearchSpace>::VertexID_t tmpID;
        tmpID=mtree->getParent(currentVertexID);
        mtree->valueOf(currentVertexID)= nullptr;
        mtree->chopSubTree(currentVertexID);
        currentVertexID=tmpID;
        return getNextSearchSpace();
    }
    return newSpace;
}

SearchSpacePtr SearchTree::getNextSearchSpace(int method) {
    SearchSpace& currentSearchSpace = *(mtree->valueOf(currentVertexID));
    SearchSpacePtr newSpace = currentSearchSpace.searchSpaceGenerate(method);
    if (newSpace!=nullptr) mtree->addAsChildren(currentVertexID, newSpace);
    return newSpace;
}

SearchSpacePtr SearchTree::getCurrentSearchSpace() {
    return (mtree->valueOf(currentVertexID));
}

SearchSpace& SearchTree::getMinTotalError() {

    //SearchSpacePtr res = getMinTotalErrorHelper(mtree->root());
    //return *res;
    return (*recordBestSearchSpacePtr);

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

SearchSpacePtr SearchTree::getBestSpace() {
    return recordBestSearchSpacePtr;
    //return getBestSpaceHelper(mtree->root());
}

SearchSpacePtr SearchTree::getBestSpaceHelper(Tree<SearchSpacePtr>::VertexID_t node) {
    vector<Tree<SearchSpacePtr>::VertexID_t > vec = mtree->getChild(node);
    if (vec.empty()) return mtree->valueOf(node);
    SearchSpacePtr tmpSpace = nullptr;
    for (auto iter=vec.begin(); iter!=vec.end(); iter++) {
        SearchSpacePtr tmpRes = getBestSpaceHelper(*iter);
        if ((tmpSpace == nullptr) || (tmpRes->getTotalError() < tmpSpace -> getTotalError()))
            tmpSpace = tmpRes;
    }
    return tmpSpace;
}