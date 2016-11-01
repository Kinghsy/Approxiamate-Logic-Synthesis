//
// Created by tripack on 16-10-26.
//

#ifndef VE490_TREE_H
#define VE490_TREE_H

template <class T>
class Tree {

public:
    class NodeRef {
    public:
        int countChild();
        T& getValue();
        NodeRef getNextChild();
        bool isEnded();
    };

    virtual NodeRef& getRoot();
};

template <class T>
class MulipleTree : public Tree<T> {

};

template <class T>
class DynamicTree : public Tree<T> {

public:

    class DynamicNodeRef : public Tree<T>::NodeRef {
        DynamicNodeRef()
    };
};


template <class T>
class BinaryTree : public Tree<T> {
public:
    class BTreeNodeRef : public Tree<T>::NodeRef {
    public:
        BTreeNodeRef left();
        BTreeNodeRef right();
    };

    virtual BTreeNodeRef& getRoot();
};



#endif //VE490_TREE_H
