//
// Created by tripack on 16-11-8.
//

#include "BinaryTree.h"

class Exception {};

class ExceptionBTree : public Exception {};

class ExceptionBTreeNullNodeReference : public ExceptionBTree {};
class ExceptionBTreeMergeIntoNonNull : public ExceptionBTree {};

template <class T>
BinaryTree<T>::BinaryTree(BinaryTree &&btree) {
    treeNode *nd = btree.root;
    btree.root = this->root;
    this->root = btree.root;
}

template <class T>
BinaryTree<T>::BinaryTree(T *root) {
    this->root = root;
}

template <class T>
node BinaryTree<T>::getRoot() {
    return node(this->root);
}

template <class T>
bool BinaryTree<T>::isEmpty() const {
    return root == nullptr;
}

template <class T>
void BinaryTree<T>::mergeLeft(node nd, BinaryTree &&ltree) {
    treeNode *tn = nd.ref;
    if (nd. ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    if (tn->left != nullptr)
        throw ExceptionBTreeMergeIntoNonNull();
    tn->left = ltree.root; // Apply move sematic
    ltree.root = nullptr;
}

template <class T>
void BinaryTree<T>::mergeRight(node nd, BinaryTree &&ltree) {
    treeNode *tn = nd.ref;
    if (nd. ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    if (tn->right != nullptr)
        throw ExceptionBTreeMergeIntoNonNull();
    tn->right = ltree.root; // Apply move sematic
    ltree.root = nullptr;
}

template <class T>
inline node BinaryTree<T>::node::left() {
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    return BinaryTree<T>::node(this->ref->left);
}

template <class T>
inline node BinaryTree<T>::node::right(){
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    return BinaryTree<T>::node(this->ref->right);

}

template <class T>
inline node BinaryTree<T>::node::parent(){
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    return BinaryTree<T>::node(this->ref->parent);
}

template <class T>
inline T& BinaryTree<T>::node::operator*(){
    if (ref == nullptr)
        throw ExceptionBTreeNullNodeReference();
    return *(ref->value);
}

template <class T>
inline BinaryTree<T>::node
BinaryTree<T>::node::node(const node &n) {
    this->ref = n.ref;
}

template <class T>
inline BinaryTree<T>::node&
BinaryTree<T>::node::operator=(const node &n) {
    this->ref = n.ref;
    return *this;
}

template <class T>
inline node& BinaryTree<T>::node::node(treeNode* tn) {
    this->ref = tn;
}

template <class T>
BinaryTree<T>::~BinaryTree() {
    // TODO: implement destructor
}
