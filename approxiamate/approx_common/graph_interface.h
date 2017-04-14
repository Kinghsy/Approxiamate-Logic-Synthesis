//
// Created by tripack on 16-12-6.
//

#ifndef VE490_GRAPH_H
#define VE490_GRAPH_H

#include <vector>

template <class VertexID, class VertexData>
class IGraphQuery {
public:
    typedef VertexID VertexID_t;

    virtual VertexID nullId() = 0;

    virtual VertexData& valueOf(const VertexID& id) = 0;

    virtual std::vector<VertexID> getEdges(const VertexID& v) = 0;

    virtual int inDegree(const VertexID& v) = 0;

    virtual int outDegree(const VertexID& v) = 0;

};

template <class VertexID, class VertexData>
class IAddVertex {
public:
    virtual void addVertex (const VertexID& v,
                            const VertexData& w) = 0;
};

template <class VertexID, class Weight>
class IAddEdge {
public:
    virtual void addEdge(const VertexID& u,
                         const VertexID& v) = 0;
};

template <class VertexID, class Weight>
class IAddWeightedEdge : public virtual IAddEdge<VertexID, Weight> {
public:
    virtual Weight& addWeight(const VertexID& u,
                              const VertexID& v) = 0;

    virtual void addEdge(const VertexID& u,
                         const VertexID& v,
                         const VertexID& w) = 0;
};

template <class VertexID, class VertexData>
class ITree {
public:
    virtual VertexID root() = 0;

    virtual bool isRoot(const VertexID& id) = 0;

    virtual std::vector<VertexID> getChild(const VertexID& id) = 0;

    virtual VertexID getParent(const VertexID& id) = 0;

    virtual void addAsChildren(const VertexID& parent,
                               const VertexData& data) = 0;

    virtual void chopSubTree(const VertexID&) = 0;
};

template <class VertexID, class VertexData>
class IBinaryTree : public virtual ITree<VertexID, VertexData> {
public:
    virtual bool hasLeft(const VertexID& p) = 0;
    virtual bool hasRight(const VertexID& p) = 0;

    virtual VertexID left(const VertexID& p) = 0;
    virtual VertexID right(const VertexID& p) = 0;

    virtual void addLeft(const VertexID& p,
                         const VertexData& d) = 0;

    virtual void addRight(const VertexID& p,
                         const VertexData& d) = 0;
};

#endif //VE490_GRAPH_H
