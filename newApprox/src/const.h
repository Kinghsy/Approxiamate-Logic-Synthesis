//
// Created by king on 17-4-11.
//

#ifndef VE490_CONST_H
#define VE490_CONST_H

#include <boost/dynamic_bitset.hpp>

typedef std::string NodeName;
typedef boost::dynamic_bitset<> DBitset;

#define MAX_VALUE (1 << 30)



typedef int ModeType;
#define CASE_1 1
#define CASE_2 2
#define CASE_3 3
#define CASE_4 4



#define DIVIDE_ROUND (1 << 3)


typedef int ActivedMode;
#define FULL_SEARCH 1
#define BRANCH_AND_BOUND 2
#define BFS_SEARCH 3
#define BFS_SEARCH_PRE_WIDTH 4

//ActivedMode ApplicatedMode = FULL_SEARCH;


#endif //VE490_CONST_H
