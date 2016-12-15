//
// Created by tripack on 16-12-6.
//

#ifndef VE490_BINARY_TREE_IMPL_H
#define VE490_BINARY_TREE_IMPL_H

#include "graph_common.h"

#include <memory>
#include <map>
#include <cassert>

template <class VertexData>
class MapBasedBinaryTree : public BinaryTree<VertexData> {
    typedef typename BinaryTree<VertexData>::VertexID_t NodeID;

    struct Node {
        NodeID id;
        NodeID left;
        NodeID right;
        NodeID parent;
        VertexData data;
    };

    std::map<NodeID, Node> nodeTable;

    NodeID r;

    int getID() {
        static int c = 0;
        return c++;
    }

public:

    MapBasedBinaryTree(const VertexData& data) {
        Node r = {
                getID(),
                nullId(),
                nullId(),
                nullId(),
                data
        };
        nodeTable.insert(
                std::make_pair(r.id, r)
        );
        this->r = r.id;
    }

    VertexData &valueOf(const NodeID &id) override {
        Node& n = nodeTable.at(id);
        return n.data;
    }

    std::vector<NodeID> getEdges(const NodeID &v) override {
        Node& n = nodeTable.at(v);
        std::vector<NodeID> vec;
        if (n.parent != nullId())
            vec.push_back(n.parent);
        else
            if(!isRoot(v)) assert(0);
        if (n.left != nullId())  vec.push_back(n.left);
        if (n.right != nullId()) vec.push_back(n.right);
        return vec;
    }

    int inDegree(const NodeID &v) override {
        Node& n = nodeTable.at(v);
        int count = (n.parent != nullId())? 1:0; // Parent
        if (hasLeft(v)) count++;
        if (hasRight(v)) count++;
        return 0;
    }

    int outDegree(const NodeID &v) override {
        return inDegree(v);
    }

    NodeID root() override {
        return r;
    }

    bool isRoot(const NodeID &id) override {
        return id == r;
    }

    std::vector<NodeID> getChild(const NodeID &id) override {
        if (id == nullId()) assert(0);
        Node& n = nodeTable.at(id);
        std::vector<NodeID> vec;
        if (n.left != nullId())  vec.push_back(n.left);
        if (n.right != nullId()) vec.push_back(n.right);
        return vec;
    }

    NodeID getParent(const NodeID &id) override {
        return nodeTable.at(id).parent;
    }

    void addAsChildren(const NodeID &parent, const VertexData &data) override {
        if (!hasLeft(parent)) addLeft(parent, data);
        if (!hasRight(parent)) addRight(parent, data);
        assert(0);
    }

    void chopSubTree(const NodeID &id) override {
        assert(0);
        // FIXME: Unused for now
    }

    bool hasLeft(const NodeID &p) override {
        return (nodeTable.at(p).left != nullId());
    }

    bool hasRight(const NodeID &p) override {
        return (nodeTable.at(p).right != nullId());
    }

    NodeID left(const NodeID &p) override {
        if (!hasLeft(p)) assert(0);
        return nodeTable.at(p).left;
    }

    NodeID right(const NodeID &p) override {
        if (!hasRight(p)) assert(0);
        return nodeTable.at(p).right;
    }

    void addLeft(const NodeID &p, const VertexData &d) override {
        if (hasLeft(p)) assert(0);
        Node& pNode = nodeTable.at(p);
        int id = getID();
        Node newNode = {
                id,     // ID
                nullId(), // LEFT
                nullId(), // RIGHT
                p,      // PARENT
                d       // Data
        };
        nodeTable.insert(
                std::make_pair(newNode.id, newNode)
        );
        pNode.left = id; // Update parent
    }

    void addRight(const NodeID &p, const VertexData &d) override {
        if (hasRight(p)) assert(0);
        Node& pNode = nodeTable.at(p);
        Node newNode = {
                getID(),     // ID
                nullId(), // LEFT
                nullId(), // RIGHT
                p,      // PARENT
                d       // Data
        };
        nodeTable.insert(
                std::make_pair(newNode.id, newNode)
        );
        pNode.right = newNode.id; // Update parent
    }

    int nullId() override {
        return -1;
    }

    BinaryTree<VertexData> *clone() override {
        return new MapBasedBinaryTree(*this);
    }
};

#endif //VE490_BINARY_TREE_IMPL_H
