//
// Created by tripack on 16-11-8.
//

#ifndef VE490_BINARYTREE_H
#define VE490_BINARYTREE_H

#include <utility>

template <class T>
class BinaryTree {
public:
    BinaryTree(T* root = nullptr);
    ~BinaryTree();

    BinaryTree& operator= (const BinaryTree& btree) = delete;
    BinaryTree(const BinaryTree& btree) = delete;
    BinaryTree(BinaryTree&& btree);

    node getRoot();
    bool isEmpty() const;
    void mergeLeft(node nd, BinaryTree &&ltree);
    void mergeRight(node nd, BinaryTree &&rtree);

    class node {
    public:
        friend class BinaryTree<T>;
        inline node left();
        inline node right();
        inline node parent();
        inline T& operator*();

        node(const node& n);
        node& operator=(const node& n);

    private:
        node(treeNode *tn);
        treeNode *ref;
    };

private:
    treeNode *root;

    struct treeNode {
        treeNode *left;
        treeNode *right;
        treeNode *parent;
        T *value;
    };

};

template <class T>
BinaryTree<T> mergeTree(T* newValue,
                        BinaryTree<T>* left,
                        BinaryTree<T>* right) {
    BinaryTree<T> tree(newValue);
    BinaryTree<T>::node root = tree.getRoot();
    tree.mergeLeft(tree.getRoot(), std::move(*left));
    tree.mergeRight(tree.getRoot(), std::move(*right));
}



#include "BinaryTree.hpp"

#endif //VE490_BINARYTREE_H
