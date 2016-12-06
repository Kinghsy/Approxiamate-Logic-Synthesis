//
// Created by tripack on 16-12-6.
//

#ifndef VE490_GRAPH_H
#define VE490_GRAPH_H

#include <map>
#include <vector>

template <class VertexID, class VertexData>
class Graph {
public:
    virtual void addVertex (const VertexID& id,
                            const VertexData& d = VertexData()) = 0;

    virtual VertexData& valueOf(const VertexID& id) = 0;

    virtual void addEdge(const VertexID& v, const VertexID& w) = 0;

    virtual std::vector<VertexID> getEdges(const VertexID& v) = 0;

    virtual ~Graph() {}
};


template <class VertexID, class VertexData, class WeightT>
class WeightedGraph : public Graph<VertexID, VertexData> {
public:
    virtual WeightT& edgeWeight(const VertexID& s,
                                const VertexID& w) = 0;

    virtual void addEdge(const VertexID& v,
                         const VertexID& u,
                         const WeightT& w) = 0;

    virtual ~WeightedGraph() {}
};

template <class VertexID, class VertexData>
class Tree : public Graph<VertexID, VertexData> {
public:
    virtual VertexID root() = 0;

    virtual bool isRoot(const VertexID& id) = 0;

    virtual std::vector<VertexID> getChildren(const VertexID& id) = 0;

    virtual void addAsChildren(const VertexID& parent,
                               const VertexID& child,
                               const VertexData& data) = 0;

};

template <class VertexID, class VertexData>
class BinaryTree : public Tree<VertexID, VertexData> {
public:
    virtual bool hasLeft(const VertexID& p) = 0;
    virtual bool hasRight(const VertexID& p) = 0;
    virtual VertexID left(const VertexID& p) = 0;
    virtual VertexID right(const VertexID& p) = 0;
    virtual VertexID getParent(const VertexID& p) = 0;

    virtual void addLeft(const VertexID& p,
                         const VertexData& d,
                         const VertexID& l);

    virtual void addRight(const VertexID& p,
                         const VertexData& d,
                          const VertexID& r);
};

#endif //VE490_GRAPH_H
