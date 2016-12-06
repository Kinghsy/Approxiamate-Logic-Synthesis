//
// Created by tripack on 16-12-6.
//

#ifndef VE490_GRAPH_COMMON_H
#define VE490_GRAPH_COMMON_H

#include "graph_interface.h"

template <class VertexData>
class BinaryTree :
        public virtual IGraphQuery<int, VertexData>,
        public virtual IBinaryTree<int, VertexData>
{
public:
    BinaryTree* clone() {
        return new BinaryTree(*this);
    }
    ~BinaryTree() {};
    // FIXME Add copy constructor
};


template <class VertexID, class VertexData, class Weight>
class WeightedGraph :
        public virtual IGraphQuery<VertexID, VertexData>,
        public virtual IAddWeightedEdge<VertexID, Weight>
{
public:
    ~WeightedGraph() {};
};

template <class VertexData>
class Tree :
        public virtual IGraphQuery<int, VertexData>,
        public virtual ITree<int, VertexData>
{
public:
    Tree* clone() {
        return new Tree(*this);
    }
    ~Tree() {};
    // FIXME Add copy constructor

};


#endif //VE490_GRAPH_COMMON_H
