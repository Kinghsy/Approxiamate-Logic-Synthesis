//
// Created by king on 16-11-2.
//

#include "boolean_function.h"
#include <memory>
#include <cstring>
#include "conts.h"
#include <tuple>

using std::unique_ptr;
using std::shared_ptr;
using std::tuple;
using std::tie;
using std::get;

//======================================================================================

KMap::KMap() {
    height=0;
    width=0;
    table=NULL;
    return ;
}

KMap::KMap(int &hei, int &wid, int **tab) {
    height=hei;
    width=wid;
    table=tab;
    return ;
}

KMap::~KMap() {
    for (int i = 0; i < height; ++i) {
        delete[] table[i];
    }
    delete[] table;
    return ;
}


BooleanFunction::BooleanFunction() {
    portSize=0;
    portName=NULL;
    truthTable=NULL;
    return ;
}

BooleanFunction::BooleanFunction(int *portname, int portsize, int *truthtable) {
    this->portSize=portsize;
    this->portName=new int[portsize];
    int sum=0;
    for (int i = 0; i < portsize; ++i) {
        this->portName[i]=portname[i];
        sum+=portname[i];
    }
    int totalSum=(1<<sum);
    this->truthTable=new int[totalSum];
    for (int j = 0; j < totalSum; ++j) {
        this->truthTable[j]=truthtable[j];
    }
    return ;
}

BooleanFunction::~BooleanFunction() {
    delete[] portName;
    delete[] truthTable;
    return ;
}

unique_ptr<KMap> BooleanFunction::getKMap(int *portPart1, int *portPart2) {
    // constructure a kmap
    int size1=0;
    int size2=0;
    for (int i = 0; i < portSize; ++i) {
        size1+=portPart1[i];
        size2+=portPart2[i];
    }

    int height=1<<size1;
    int weight=1<<size2;
    int *(*table)=new int*[height];
    for (int i = 0; i < height; ++i) {
        table[i]=new int[weight];
    }
    for (int num1=0; num1 < height; num1++) {
        for (int num2=0; num2 < weight; num2++) {
            int temp[PORT_SIZE];
            memset(temp, 0 ,sizeof(temp));
            int num1Op=num1; int num2Op=num2;
            for (int i = portSize-1; i >=0 ; --i) {
                if (portPart2[i]==1) { temp[i]=num2Op % 2; num2Op=num2Op / 2; }
                if (portPart1[i]==1) { temp[i]=num1Op % 2; num1Op=num1Op / 2; }
            }
            int num=0;
            for (int i = 0; i < portSize; ++i)
                if (portName[i]==1)
                    num=((num<<1)+temp[i]);
            table[num1][num2]=truthTable[num];
        }
    }

    unique_ptr<KMap> kmap(new KMap(height, weight, table));
    return kmap;

}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>> BooleanFunction::divide(int *portPart1,
                                                                                        int *portPart2) {

    unique_ptr<KMap> kmap=std::move( this->getKMap(portPart1, portPart2) );

    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> method1=findMinError1(*kmap, portPart1, portPart2); // major rows and all 0s
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> method2=findMinError1(*kmap, portPart1, portPart2); // major rows and all 1s
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> method3=findMinError1(*kmap, portPart1, portPart2); // major rows and complements of major rows
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> method4=findMinError1(*kmap, portPart1, portPart2); // 0s and 1s

    // maybe some errors would come this way but now sure. it mainly concerned about the usage of unique_ptr.

    if ( ((get<2>(method4)<=get<2>(method1)) && (get<2>(method4)<=get<2>(method2))) && (get<2>(method4)<=get<2>(method3)) ) {
        // method 4 is choosen.
        return make_tuple( move(get<0>(method4)), move(get<1>(method4)) );
    } else if ((get<2>(method1)<=get<2>(method2)) && (get<2>(method1)<=get<2>(method3))) {
        // method 1 is choosen.
        return make_tuple( move(get<0>(method1)), move(get<1>(method1)) );
    } else if (get<2>(method2)<=get<2>(method3)) {
        // method 2 is choosen.
        return make_tuple( move(get<0>(method2)), move(get<1>(method2)) );
    } else {
        // method 3 is choosen.
        return make_tuple( move(get<0>(method3)), move(get<1>(method3)) );
    }

}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> BooleanFunction::findMinError1(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and 0s


};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> BooleanFunction::findMinError2(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and 1s


};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> BooleanFunction::findMinError3(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and complement of major rows


};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int> BooleanFunction::findMinError4(const KMap &kmap, int *portPart1, int *portPart2) {
    // 0s and 1s
    int *tempRes1=new int[kmap.height];
    int error=0;
    for (int i = 0; i < kmap.height; ++i) {
        int rec[2]={0,0};
        for (int j = 0; j < kmap.width; ++j) rec[kmap.table[i][j]]++;
        if (rec[0] > rec[1]) {
            tempRes1[i]=0;
            error+=rec[1];
        } else {
            tempRes1[i]=1;
            error+=rec[0];
        }
    }
    int *tempRes2=new int[kmap.width];
    for (int i = 0; i < kmap.width; ++i) tempRes2[i]=0;
    unique_ptr<BooleanFunction> p1(new BooleanFunction(portPart1, portSize, tempRes1));
    unique_ptr<BooleanFunction> p2(new BooleanFunction(portPart2, portSize, tempRes2));

    delete[] tempRes1;
    delete[] tempRes2;

    return make_tuple(move(p1), move(p2), error);

};
