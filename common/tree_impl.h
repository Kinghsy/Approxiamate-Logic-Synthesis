//
// Created by tripack on 16-12-7.
//

#ifndef VE490_TREE_IMPL_H
#define VE490_TREE_IMPL_H

#include "graph_common.h"

template <class VertexData>
class MapBasedTree : public Tree<VertexData> {
    typedef typename Tree<VertexData>::VertexID_t NodeID;

    struct Node {
        NodeID id;
        NodeID child;
        NodeID nextSibling;
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

    MapBasedTree(const VertexData& data) {
        Node r = {
                getID(),
                nullId(),
                nullId(),
                nullId(),
                data
        };
        nodeTable.insert(std::make_pair(r.id, r));
        this->r = r.id;
    }

    VertexData &valueOf(const NodeID &id) override {
        Node& n = nodeTable.at(id);
        return n.data;
    }

    std::vector<NodeID> getEdges(const NodeID &v) override {
        Node& n =nodeTable.at(v);
        std::vector<NodeID> vec;
        if (n.parent != nullId())
            vec.push_back(n.parent);
        else
        if (!isRoot(v)) assert(0);
        NodeID tmpChd= n.child;
        while (tmpChd != nullId()) {
            vec.push_back(tmpChd);
            tmpChd = nodeTable.at(tmpChd).nextSibling;
        }
        return vec;
    }

    int inDegree(const NodeID &v) override {
        Node &n = nodeTable.at(v);
        int count = (n.parent == nullId())? 0:1; // Parent
        NodeID tmpChd = n.child;
        while (tmpChd != nullId()) {
            count ++;
            tmpChd = nodeTable.at(tmpChd).nextSibling;
        }
        return count;
    }

    int outDegree(const NodeID &v) override {
        return inDegree(v);
    }

    NodeID root() override {
        return r;
    }

    bool isRoot(const NodeID &id) override {
        return (id == r);
    }

    std::vector<NodeID> getChild(const NodeID &id) override {
        if (id == nullId()) assert(0);
        Node &n = nodeTable.at(id);
        std::vector<NodeID> vec;
        NodeID tmpChd = n.child;
        while (tmpChd != nullId()) {
            vec.push_back(tmpChd);
            tmpChd = nodeTable.at(tmpChd).nextSibling;
        }
        return vec;
    }

    NodeID getParent(const NodeID &id) override {
        return nodeTable.at(id).parent;
    }

    void addAsChildren(const NodeID &parent, const VertexData &data) override {
        Node &n = nodeTable.at(parent);
        if (n.child == nullId()) {
            Node newNode= {
                    getID(),
                    nullId(),
                    nullId(),
                    parent,
                    data
            };
            n.child = newNode.id;
            nodeTable.insert(std::make_pair(newNode.id, newNode));

        } else {
            NodeID tmpChd = n.child;
            while (nodeTable.at(tmpChd).nextSibling != nullId()) {
                tmpChd = nodeTable.at(tmpChd).nextSibling;
            }
            Node newNode={
                    getID(),
                    nullId(),
                    nullId(),
                    parent,
                    data
            };
            Node &tmp=nodeTable.at(tmpChd);
            tmp.nextSibling = newNode.id;
            nodeTable.insert(std::make_pair(newNode.id, newNode));
        }
    }

    void chopSubTree(const NodeID &id) override {
        assert(0);
    }

    int nullId() override {
        return -1;
    }

    Tree<VertexData> *clone() override {
        return new MapBasedTree(*this);
    }
};
#endif //VE490_TREE_IMPL_H
