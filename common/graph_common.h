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
protected:
    const IGraphQuery::VertexID_t nullId = -1;
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

template <class VertexData>
class Tree :
        public virtual IGraphQuery<int, VertexData>,
        public virtual ITree<int, VertexData>
{
protected:
    const IGraphQuery::VertexID_t nullId = -1;
public:
    ~Tree() {};
};


#endif //VE490_GRAPH_COMMON_H
