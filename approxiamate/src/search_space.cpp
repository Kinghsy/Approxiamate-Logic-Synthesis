//
// Created by king on 16-12-5.
//

#include "search.h"
#include "conts.h"
#include "boolean_function.h"
#include "../../common/truth_table.h"


#include <binary_tree_impl.h>
#include <tree_impl.h>
#include <graph_common.h>

#include <iostream>
#include <sstream>
#include <tuple>
#include <memory>
#include <stack>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

string randString();

//==========Search Space===================

SearchSpace::SearchSpace(BinaryTree<SearchNodeOpPtr > &oldTree) {
    btree = unique_ptr<BinaryTree<SearchNodeOpPtr > > (oldTree.clone());
    // FIXME add polymorphic copy constructor
    BinaryTree<SearchNodeOpPtr>::VertexID_t tmp=findDivideNode();
    currentDivideRange=0;
    if (tmp!=btree->nullId())
        divideNode=btree->valueOf(findDivideNode());
    currentDivide=1;
    if (growAble) currentDivideRange=divideNode->node->getDivideRange();
    totalError=calculTotalError();
}

SearchSpace::SearchSpace(unique_ptr<BinaryTree<SearchNodeOpPtr> > oldTreePtr) {
    btree = move(oldTreePtr);
    BinaryTree<SearchNodeOpPtr>::VertexID_t tmp=findDivideNode();
    currentDivideRange=0;
    if (tmp!=btree->nullId())
        divideNode=btree->valueOf(tmp);
    currentDivide=1;
    if (growAble) currentDivideRange=divideNode->node->getDivideRange();
    totalError=calculTotalError();
}

SearchSpace::SearchSpace() {
    btree=NULL;
    divideNode=NULL;
    currentDivide=0;
    currentDivideRange=0;
    totalError=0;
    growAble=false;
}

SearchSpace::~SearchSpace() {
    return ;
}

bool SearchSpace::searchSpaceGrow() {
    if (!growAble) return growAble;
    if (currentDivide > currentDivideRange - 2) return false;
    return true;
}

SearchSpacePtr SearchSpace::searchSpaceGenerate() {
    return searchSpaceGenerate(currentDivide++);
}

SearchSpacePtr SearchSpace::searchSpaceGenerate(int divideMethod) {

    if (divideMethod > currentDivideRange-2) return NULL;
    if (divideMethod < 1) return NULL;

    tuple<SearchNodeOpPtr, SearchNodeOpPtr, SearchNodeOpPtr>
            divideRes = divideNode->divide(divideMethod);

    SearchNodeOpPtr newNodeOp = move(get<0>(divideRes));
    SearchNodeOpPtr leftNodeOp = move(get<1>(divideRes));
    SearchNodeOpPtr rightNodeOp = move(get<2>(divideRes));

    unique_ptr<BinaryTree<SearchNodeOpPtr> > newBTree(btree->clone());
    BinaryTree<SearchNodeOpPtr>::VertexID_t treeNodeID = findDivideNode();
    btree->addLeft(treeNodeID, leftNodeOp);
    btree->addRight(treeNodeID, rightNodeOp);

    unique_ptr<BinaryTree<SearchNodeOpPtr>> tmpBTree;

    tmpBTree = move(btree);
    btree = move(newBTree);
    newBTree = move(tmpBTree);

    treeNodeID = findDivideNode();
    btree->valueOf(treeNodeID) = newNodeOp;
    divideNode=newNodeOp;

    SearchSpacePtr newSearchSpace(new SearchSpace(*newBTree));
    return move(newSearchSpace);
}

int SearchSpace::getTotalError() {
    return totalError;
}

BinaryTree<SearchNodeOpPtr>::VertexID_t SearchSpace::findDivideNode() {
    BinaryTree<SearchNodeOpPtr>::VertexID_t node;
    node = btree->root();
    if (node==btree->nullId())
        growAble=false;
    else
        growAble=true;
    BinaryTree<SearchNodeOpPtr >::VertexID_t tmp=findDivideNodeHelper(node);
    if (tmp == btree->nullId())
        growAble=false;
    else
        growAble=true;
    return tmp;
}

BinaryTree<SearchNodeOpPtr>::VertexID_t SearchSpace::findDivideNodeHelper(
        BinaryTree<SearchNodeOpPtr>::VertexID_t node) {
    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        BinaryTree<SearchNodeOpPtr>::VertexID_t leftRes=findDivideNodeHelper(btree->left(node));
        if (leftRes!=btree->nullId()) return leftRes;
        BinaryTree<SearchNodeOpPtr>::VertexID_t rightRes=findDivideNodeHelper(btree->right(node));
        if (rightRes!=btree->nullId()) return rightRes;
        return btree->nullId();
    }
    if (btree->valueOf(node)->isDiviable()) return node;
    return btree->nullId();
}

int SearchSpace::calculTotalError() {//BooleanFunction &initBoolFunc) {

    BooleanFunctionPtr result = calculTotalErrorHelper(btree->root());
    int sum = btree->valueOf(btree->root())->node->getBooleanFunction()->booleanCompare(*result);
    // int sum = initBoolFunc.booleanCompare(*result);
    return sum;

}

BooleanFunctionPtr SearchSpace::calculTotalErrorHelper(
        BinaryTree<SearchNodeOpPtr>::VertexID_t node) {

    if ((btree->hasLeft(node)) && (btree->hasRight(node))) {
        BooleanFunctionPtr r1 = calculTotalErrorHelper(btree->left(node));
        BooleanFunctionPtr r2 = calculTotalErrorHelper(btree->right(node));
        BooleanFunctionPtr res = r1->combine(*r2, btree->valueOf(node)->oper);
        return res;
    }

    return move(btree->valueOf(node)->node->getBooleanFunction());
}

void SearchSpace::printSearchSpaceHelper(
        BinaryTree<SearchNodeOpPtr>::VertexID_t node) {
    SearchNodeOpPtr &nodeOpPtr = btree->valueOf(node);
    if (btree->hasLeft(node)) {
        cout << " ( " ;
        printSearchSpaceHelper(btree->left(node));
        cout << " ) " ;
    }
    if ((!btree->hasLeft(node)) && (!btree->hasRight(node))) {
        BooleanFunctionPtr bfPtr = btree->valueOf(node)->node->getBooleanFunction();
        int *portName=bfPtr->getPortName();
        int size=bfPtr->getPortSize();
        for (int i = 0; i < size; ++i)
            if (portName[i] == 1) cout << "P" << i;
        cout << " [ ";
        cout << bfPtr->toString();
        cout << " ] ";
    } else {
        switch (btree->valueOf(node)->oper) {
            case OPERATION_XOR:
                cout << "xor";
                break;
            case OPERATION_OR:
                cout << "or";
                break;
            case OPERATION_AND:
                cout << "and";
                break;
            case OPERATION_DROP:
                cout << "drop";
                break;
            default:
                cout << "this shou never be run";
                assert(-1);
                break;
        }
    }
    if (btree->hasRight(node)) {
        cout << " ( ";
        printSearchSpaceHelper(btree->right(node));
        cout << " ) ";
    }
    return ;
}

void SearchSpace::printSearchSpace() {
    BinaryTree<SearchNodeOpPtr>::VertexID_t vertexId = btree->root();
    printSearchSpaceHelper(vertexId);
    cout << "              totoal error rate: " << getTotalError() <<  " \n";
    cout << "                    total boolean function:";
    BooleanFunctionPtr bfPtr=getFinalBooleanFuntion();
    cout << bfPtr->toString() << "\n";

}

BooleanFunctionPtr SearchSpace::getFinalBooleanFuntion() {
    return calculTotalErrorHelper(btree->root());
}

void SearchSpace::generateBlifFile(string BlifFileName, TruthTable &TruthTab) {
    ofstream BlifFile;
    BlifFile.open(BlifFileName, ios_base::out | ios_base::trunc);

    // the first line
    BlifFile << "# This is an Blif file that record "
            << "an approximate logical circuit. Generate time: ";
    char timeString[100];
    time_t tt=time(NULL);
    tm *t=localtime(&tt);
    sprintf(timeString, "%d-%02d-%02d %02d:%02d:%02d\n",
           t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
           t->tm_hour, t->tm_min, t->tm_sec);
    BlifFile << timeString;

    // the second line
    BlifFile << ".model Approximate_Circuit\n";

    // inputs, outputs
    BlifFile << ".inputs";
    for (int i = 0; i < TruthTab.numInput(); ++i) BlifFile << " " << TruthTab.getName(i);
    BlifFile << "\n";
    BlifFile << ".outputs ";
    BlifFile << TruthTab.outName << "\n";

    // main context
    generateBlifFileHelper(btree->root(), BlifFile, TruthTab);

    // ends
    BlifFile << ".end\n";
    BlifFile.close();
    return ;
}

tuple<string, BooleanFunctionPtr, int > SearchSpace::
    generateBlifFileHelper(BinaryTree<SearchNodeOpPtr>::VertexID_t node,
                            ofstream &BlifFile, TruthTable &TruthTab) {

    SearchNodeOpPtr &snOpPtr = btree->valueOf(node);
    string outName;

    if (!(snOpPtr->isDiviable())) {
        // reach the lowest level in logical, here is a translate layer,
        // for example, [0.1]--->[1,1] or [0,1]--->[1,0]
        // could be more than one inputs, e.g. [00,01,10,11]-->[0,0,0,0]
        /*if (btree->isRoot(node)) {
            BlifFile << ".names " << TruthTab.outName << "\n";
            return make_tuple("", nullptr, 1);
        }*/
        BooleanFunctionPtr bfPtr = snOpPtr->node->getBooleanFunction();
        int portSize=bfPtr->getPortSize();
        int *portName=bfPtr->getPortName();
        if ( !(bfPtr->getInputNum()==1) ) {
            // (all 0s or all 1s) and (inputNum>1)
            int flag=(bfPtr->isAll1s())? 1:0;
            int *port=new int[portSize];
            int num=0;
            for (int i = 0; i < portSize; ++i)
                if (portName[i]==1) port[num++]=i;

            string inName1; string inName2;
            inName1 = TruthTab.getName(port[0]);
            inName2 = TruthTab.getName(port[1]);
            for (int i = 2; i < num; ++i) {
                outName=randString();
                BlifFile << ".names " << inName1 << " " << inName2 << " " << outName << "\n";
                if (flag==1) BlifFile << "-- 1" << "\n";
                inName1=outName;
                inName2=TruthTab.getName(port[i]);
            }
            if (btree->isRoot(node))
                outName=TruthTab.outName;
            else
                outName=randString();
            BlifFile << ".names " << inName1 << " " << inName2 << " " << outName << "\n";
            if (flag==1) BlifFile << "-- 1" << "\n";
            delete[] port;
        } else {
            //inputNum == 1
            for (int i = 0; i < portSize; ++i)
                if (portName[i]==1) outName=TruthTab.getName(i);
        }
        return make_tuple(outName, bfPtr,1);
    }



    tuple<string, BooleanFunctionPtr, int > leftNodeInfo =
            generateBlifFileHelper( btree->left(node), BlifFile, TruthTab);
    tuple<string, BooleanFunctionPtr, int > rightNodeInfo =
            generateBlifFileHelper( btree->right(node), BlifFile, TruthTab);

    string leftName;
    BooleanFunctionPtr leftBFPtr;
    int leftFlag;
    tie(leftName, leftBFPtr, leftFlag) = leftNodeInfo;
    string rightName;
    BooleanFunctionPtr rightBFPtr;
    int rightFlag;
    tie(rightName, rightBFPtr, rightFlag) = rightNodeInfo;
    if (btree->isRoot(node))
        outName = TruthTab.outName;
    else
        outName = randString();
    BlifFile << ".names " << leftName << " " << rightName << " " << outName << "\n";

    for (int cases = 0; cases < 4 ; ++cases) {
        int actualLeftNum = cases / 2;
        int mappedLeftNum = actualLeftNum;
        int actualRightNum = cases % 2;
        int mappedRightNum = actualRightNum;
        int result;
        if ( leftFlag == 1 ) {
            int *truthTable = leftBFPtr->getTruthTable();
            mappedLeftNum = truthTable[actualLeftNum];
        }
        if ( rightFlag == 1 ) {
            int *truthTable = rightBFPtr->getTruthTable();
            mappedRightNum = truthTable[actualRightNum];
        }
        switch (btree->valueOf(node)->oper) {
            case OPERATION_AND:
                result = mappedLeftNum & mappedRightNum;
                break;
            case OPERATION_OR:
                result = mappedLeftNum | mappedRightNum;
                break;
            case OPERATION_XOR:
                result = (mappedLeftNum != mappedRightNum)? 1:0;
                break;
            case OPERATION_DROP:
                result = mappedLeftNum;
                break;
            case OPERATION_NONE:
                assert(0);
                break;
            default:
                break;
        }
        if (result == 1)
            BlifFile << actualLeftNum << actualRightNum << " " << result << "\n";
    }

    return make_tuple(
            outName, leftBFPtr->combine(*(rightBFPtr), btree->valueOf(node)->oper ), 0
    );

}



//========================================================================
string randString() {
    ostringstream sstr;
    static int number=0;
    sstr <<  "tmpNode" << number;
    number++;
    return sstr.str();
}