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
        NodeID parent;
        std::vector<NodeID> children;
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
        std::vector<NodeID > vec;
        Node r = {
                getID(),
                nullId(),
                vec,
                data
        };
        nodeTable.insert(std::make_pair(r.id, r));
        this->r = r.id;
    }

    ~MapBasedTree() {
        chopSubTree(root());
    }

    VertexData &valueOf(const NodeID &id) override {
        Node& n = nodeTable.at(id);
        return n.data;
    }

    std::vector<NodeID> getEdges(const NodeID &v) override {
        Node& n =nodeTable.at(v);
        std::vector<NodeID> vec = (n.children);
        if (n.parent != nullId())
            vec.push_back(n.parent);
        return vec;
    }

    int inDegree(const NodeID &v) override {
        Node &n = nodeTable.at(v);
        int count = (n.parent == nullId())? 0:1; // Parent
        count += n.children.size();
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
        std::vector<NodeID> vec = (n.children);
        return vec;
    }

    NodeID getParent(const NodeID &id) override {
        if (id==r) assert(0);
        return nodeTable.at(id).parent;
    }

    void addAsChildren(const NodeID &parent, const VertexData &data) override {
        Node &n = nodeTable.at(parent);
        std::vector<NodeID > vec;
        Node newNode= { getID(), parent, vec, data };
        nodeTable.insert(std::make_pair(newNode.id, newNode));
        n.children.push_back(newNode.id);
        return ;
    }

    void chopSubTree(const NodeID &id) override {
        if ( id==nullId() ) assert(0);
        Node &n = nodeTable.at(id);
        for (auto iter = n.children.begin(); iter != n.children.end(); iter++) {
            chopSubTree( *iter );
        }
        if ( id == root()) {
            r = nullId();
        } else {
            Node &par=nodeTable.at(n.parent);
            for (auto iter = par.children.begin(); iter != par.children.end(); iter++)
                if (*iter == id) {
                    par.children.erase(iter);
                    break;
                }
        }
        auto iter = nodeTable.find(id);
        nodeTable.erase(iter);
        return ;
    }

    int nullId() override {
        return -1;
    }

    Tree<VertexData> *clone() override {
        return new MapBasedTree(*this);
    }
};
#endif //VE490_TREE_IMPL_H
