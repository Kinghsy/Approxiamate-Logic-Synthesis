//
// Created by king on 16-11-2.
//

#include "boolean_function.h"
#include <memory>
#include <cstring>
#include "conts.h"
#include <tuple>
#include <iostream>
#include <cstring>
#include <sstream>

using std::unique_ptr;
using std::shared_ptr;
using std::tuple;
using std::tie;
using std::get;
using std::make_tuple;
using std::string;
using std::stringstream;
using std::cout;

//======================================================================================


KMap::KMap() {
    height=0;
    width=0;
    table=NULL;
    return ;
}

KMap::KMap(const int &hei, const int &wid, int **tab) {
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

bool KMap::operator==(const KMap &b) {
    if ( (this->height != b.height) || (this->width != b.width) ) return false;
    for (int i = 0; i < this->height; ++i)
        for (int j = 0; j < this->width; ++j)
            if (this->table[i][j] != b.table[i][j]) return false;
    return true;
}

void KMap::display() {
    for (int i = 0; i < this->height; ++i) {
        for (int j = 0; j < this->width; ++j) {
            cout << this->table[i][j] << " ";
        }
        cout << "\n";
    }
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

bool BooleanFunction::operator== (const BooleanFunction &b) {
    if (this->portSize != b.portSize) return false;
    int sum = 0;
    for (int i = 0; i < portSize; ++i)
        if (this->portName[i] != b.portName[i]) return false;
    for (int i = 0; i < (1 << sum); ++i)
        if (this->truthTable[i] != b.truthTable[i]) return false;
    return true;
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
    int *temp=new int[portSize];
    for (int num1=0; num1 < height; num1++) {
        for (int num2=0; num2 < weight; num2++) {
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
    delete[] temp;
    unique_ptr<KMap> kmap(new KMap(height, weight, table));
    return kmap;

}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> BooleanFunction::divide(int *portPart1,
                                                                                        int *portPart2) {

    unique_ptr<KMap> kmap=std::move( this->getKMap(portPart1, portPart2) );

    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> method1=findMinError1(*kmap, portPart1, portPart2); // major rows and all 0s
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> method2=findMinError2(*kmap, portPart1, portPart2); // major rows and all 1s
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> method3=findMinError3(*kmap, portPart1, portPart2); // major rows and complements of major rows
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> method4=findMinError4(*kmap, portPart1, portPart2); // 0s and 1s

    // maybe some errors would come this way but now sure. it mainly concerned about the usage of unique_ptr.

    if ( ((get<2>(method4)<=get<2>(method1)) && (get<2>(method4)<=get<2>(method2))) && (get<2>(method4)<=get<2>(method3)) ) {
        // method 4 is choosen.
        return method4;
    } else if ((get<2>(method1)<=get<2>(method2)) && (get<2>(method1)<=get<2>(method3))) {
        // method 1 is choosen.
        return method1;
    } else if (get<2>(method2)<=get<2>(method3)) {
        // method 2 is choosen.
        return method2;
    } else {
        // method 3 is choosen.
        return method3;
    }

}

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> BooleanFunction::findMinError1(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and 0s

    int *majorRow=new int[kmap.width];
    int *notSure=new int[kmap.width];
    int deterNum;
    int notSureNum;

    findMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, REMOVE_0S);
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> result=findBestMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, METHOD_1, portPart1, portPart2);

    delete[] majorRow;
    delete[] notSure;

    return result;
};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> BooleanFunction::findMinError2(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and 1s

    int *majorRow=new int[kmap.width];
    int *notSure=new int[kmap.width];
    int deterNum;
    int notSureNum;

    findMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, REMOVE_1S);
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> result=findBestMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, METHOD_2, portPart1, portPart2);

    delete[] majorRow;
    delete[] notSure;

    return result;
};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> BooleanFunction::findMinError3(const KMap &kmap, int *portPart1, int *portPart2) {
    // major rows and complement of major rows

    int *majorRow=new int[kmap.width];
    int *notSure=new int[kmap.width];
    int deterNum;
    int notSureNum;

    findMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, NO_ACTION);
    tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> result=findBestMajorRow(kmap, majorRow, notSure, deterNum, notSureNum, METHOD_3, portPart1, portPart2);

    delete[] majorRow;
    delete[] notSure;

    return result;

};

tuple<unique_ptr<BooleanFunction>, unique_ptr<BooleanFunction>, int, int> BooleanFunction::findMinError4(const KMap &kmap, int *portPart1, int *portPart2) {
    // 0s and 1s
    int *tempRes1=new int[kmap.height];
    int *tempRes2=new int[kmap.width];
    int *comTempRes2=new int[kmap.width];
    for (int i = 0; i < kmap.width; ++i) { tempRes2[i]=0; comTempRes2[i]=1;}
    int error=0;
    for (int i = 0; i < kmap.height; ++i) {
        tuple<int, int> rowRes=findDifference(kmap.width, tempRes2, comTempRes2, kmap.table[i]);
        error+=get<0>(rowRes);
        tempRes1[i]=get<1>(rowRes);
    }
    unique_ptr<BooleanFunction> p1(new BooleanFunction(portPart1, portSize, tempRes1));
    unique_ptr<BooleanFunction> p2(new BooleanFunction(portPart2, portSize, tempRes2));

    delete[] tempRes1;
    delete[] tempRes2;
    delete[] comTempRes2;

    return make_tuple(move(p1), move(p2), error, OPERATION_DROP);

};

tuple<int, int> BooleanFunction::findDifference(int len, int *mode0, int *mode1, int *goal) {
    //error, answer(0 or 1); if used 0 then goes 0, else goes 1.
    int diff0=0;
    int diff1=0;
    for (int i = 0; i < len; ++i) {
        diff0 += (mode0[i] != goal[i]);
        diff1 += (mode1[i] != goal[i]);
    }
    if (diff0 <= diff1) return make_tuple( diff0, 0 );
    return make_tuple( diff1, 1 );

};

void BooleanFunction::findMajorRow(const KMap &kmap,
                                   int *majorRow,
                                   int *notSure,
                                   int &deterNum,
                                   int &notSureNum,
                                   const int &mode) {
   // std::cout << kmap.width << " " << kmap.height << std:: endl;
    int *count=new int[kmap.width];
    for (int j = 0; j < kmap.width; ++j) {
        count[j]=0;
    }

    int rowNum=0;
    for (int i = 0; i < kmap.height; ++i) {
        int numCount=0;
        for (int j = 0; j < kmap.width; ++j) numCount += kmap.table[i][j];
        if ((mode==REMOVE_0S) && (numCount==0)) continue;
        if ((mode==REMOVE_1S) && (numCount==kmap.width)) continue;
        for (int j = 0; j < kmap.width; ++j) count[j] += kmap.table[i][j];
        rowNum++;
    }

    deterNum=0;
    notSureNum=0;

    for (int j = 0; j < kmap.width; ++j) {
        if ((count[j]==(rowNum/2)) && (rowNum % 2 == 0)) {
            notSure[notSureNum++]=j;
            majorRow[j]=0;
        } else if (count[j]>(rowNum/2)) {
            majorRow[j]=1;
        } else {
            majorRow[j]=0;
        }
    }

    if (FASTMODEOPTION==1) {
        if ( notSureNum > DONTCAREBOND ) {
            deterNum = notSureNum - DONTCAREBOND;
            notSureNum = DONTCAREBOND;
        }
    }

    delete[] count;
    return ;

}

std::tuple<std::unique_ptr<BooleanFunction>, std::unique_ptr<BooleanFunction>, int, int>
    BooleanFunction::findBestMajorRow(const KMap &kmap, int *majorRow, int *notSure, const int &deterNum, const int &notSureNum, const int &mode, int *portPart1, int *portPart2) {

    int *all0s=new int[kmap.width];
    int *all1s=new int[kmap.width];
    int *compMajorRow=new int[kmap.width];
    for (int j = 0; j < kmap.width; ++j) {
        all0s[j]=0;
        all1s[j]=1;
        compMajorRow[j]=1-majorRow[j];
    }

    int minErr=kmap.height*kmap.width;
    int *resRow=new int[kmap.width];
    int *resColumn=new int[kmap.height];
    int *tempColumn=new int[kmap.height];
    for (int k = 0; k < (1 << notSureNum); ++k) {
        int temp=k;
        for (int l = deterNum; l < notSureNum+deterNum; ++l) {
            majorRow[notSure[l]] = temp % 2;
            compMajorRow[notSure[l]] = 1 - majorRow[notSure[l]];
            temp = temp / 2;
        }
        int err=0;
        for (int i = 0; i < kmap.height; ++i) {
            tuple<int, int> rowRes;
            switch (mode) {
                case (METHOD_1):
                    rowRes=findDifference(kmap.width, all0s, majorRow, kmap.table[i]);
                    break;
                case (METHOD_2):
                    rowRes=findDifference(kmap.width, majorRow, all1s, kmap.table[i]);
                    break;
                case (METHOD_3):
                    rowRes=findDifference(kmap.width, majorRow, compMajorRow, kmap.table[i]);
                    break;
                default:
                    break;
            }
            err+=get<0>(rowRes);
            tempColumn[i]=get<1>(rowRes);
        }
        if (err<minErr) {
            minErr = err;
            for (int i = 0; i < kmap.height; ++i) resColumn[i]=tempColumn[i];
            for (int j = 0; j < kmap.width; ++j) resRow[j]=majorRow[j];
        }
    }

    unique_ptr<BooleanFunction> p1(new BooleanFunction(portPart1, portSize, resColumn));
    unique_ptr<BooleanFunction> p2(new BooleanFunction(portPart2, portSize, resRow));

    delete[] all0s;
    delete[] all1s;
    delete[] compMajorRow;
    delete[] resRow;
    delete[] resColumn;
    delete[] tempColumn;

    if (mode==METHOD_1)
        return make_tuple(move(p1), move(p2), minErr, OPERATION_AND);
    else if (mode==METHOD_2)
        return make_tuple(move(p1), move(p2), minErr, OPERATION_OR);
    else // METHOD_3
        return make_tuple(move(p1), move(p2), minErr, OPERATION_XOR);

}

string BooleanFunction::toString() {

    stringstream ss;
    int num=0;
    for (int i = 0; i < portSize; ++i) num+=portName[i];
    for (int i = 0; i < (1 << num) - 1; ++i) ss << truthTable[i] << " ";
    ss << truthTable[(1 << num) - 1];

    return ss.str();
}