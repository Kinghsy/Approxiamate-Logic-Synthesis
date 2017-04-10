//
// Created by king on 17-4-11.
//

#ifndef VE490_NODE_H
#define VE490_NODE_H

#include <vector>
#include <boost/shared_ptr.hpp>

#include "bool_function.h"
#include "kmap.h"


typedef std::shared_ptr<DecomposedFunc >  DecomposedFuncPtr;

class DecomposedFunc{

private:
//    int divideOp;  //
    BoolFunction fun;
    divPair div;

    int currentDiv; // e.g. if currentDiv == 3, then select 1,2 as leftFunc and 3,4,5,6 as rightFunc
                    // FIXME for better expression

public:
    struct divPair {
        DecomposedFuncPtr leftFun;
        DecomposedFuncPtr rightFun;
        int opera;
    };

    void operator= (const DecomposedFunc& initDF); // Deep copy required.
    bool operator== (const DecomposedFunc& initDF);
    BoolFunction& operator* ();

    BoolFunction combine(); // return (div.leftFun)->combine().combineWith((div.rightFun)->combine(), div.opera); ??
                            // FIXME
    bool divide();

    DecomposedFunc();
    DecomposedFunc(const BoolFunction& initFun);
    ~DecomposedFunc();

    void display();

};


#endif //VE490_NODE_H
