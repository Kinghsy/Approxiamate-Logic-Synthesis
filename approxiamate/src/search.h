//
// Created by tripack on 16-10-26.
//

#ifndef VE490_SEARCH_H
#define VE490_SEARCH_H

#include "tree.h"
#include "boolean_function.h"

class SearchSpace : public Object {
//
    BooleanFunction *booleanFunction;
public:

};


class SearchTree {

public:
    class SearchNode {
        SearchSpace& sspace;
        MulipleTree<SearchNode>::NodeRef node;
    public:

    };
    SearchNode& getNextChild(SearchNode& n);
    void eliminateSearchSpace(SearchNode& n);
    SearchNode& getRootNode();

    //

private:
    MulipleTree<SearchSpace> *mtree;

};

#endif //VE490_SEARCH_H
