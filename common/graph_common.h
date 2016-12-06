//
// Created by tripack on 16-12-6.
//

#ifndef VE490_GRAPH_COMMON_H
#define VE490_GRAPH_COMMON_H

#include "graph_interface.h"

template <class VertexID, class VertexData>
class BinaryTree :
        public virtual IGraphQuery<VertexID, VertexData>,
        public virtual IBinaryTree<VertexID, VertexData>
{
public:
    ~BinaryTree() {};
};

template <class VertexID, class VertexData, class Weight>
class WeightedGraph :
        public virtual IGraphQuery<VertexID, VertexData>,
        public virtual IAddWeightedEdge<VertexID, Weight>
{
public:
    ~WeightedGraph() {};
};

template <class VertexID, class VertexData>
class Tree :
        public virtual IGraphQuery<VertexID, VertexData>,
        public virtual ITree<VertexID, VertexData>
{
public:
    ~Tree() {};
};


#endif //VE490_GRAPH_COMMON_H
