//
// Created by king on 16-11-2.
//

#ifndef PROJECT_CONTS_H
#define PROJECT_CONTS_H





//--------search.h
#define NODE_SIZE 10

#define WIDTH_PRE_LEVEL 3


//--------boolean_function.h
#define PORT_SIZE 10
#define TRUTH_TABLE_SIZE 1024 // this should be 2^(PORT_SIZE).

#define FASTMODEOPTION 1 // fast mode is on when this is 1.
#define DONTCAREBOND 10 // when fast mode is on this number matters.

#define OPERATION_AND 0x1234 // and operation, appears in divide into major rows and 0s.
#define OPERATION_OR 0x1235 // or operation, appears in divide into major rows and 1s.
#define OPERATION_XOR 0x1236 // xor operation, appears in divide into major rows and complements of major rows.
#define OPERATION_DROP 0x1237 // another operation, that is f(x1,x2)=x1, appears in divide into 0s and 1s.
#define OPERATION_NONE 0x1238 // none opeartion, used in initalization.

#define REMOVE_0S 0x1233
#define REMOVE_1S 0x1232
#define NO_ACTION 0x1231

#define METHOD_1 0x1230
#define METHOD_2 0x122F
#define METHOD_3 0x122E

class ERROR__BOOLEANFUNC_COMPARE {
};

//----------actived mode
#define FULL_SEARCH 0x0001
#define BRANCH_AND_BOUND 0x0002
#define GREEDY 0x0002
#define BFS_SEARCH 0x0004
#define XOR_IGNORE 0x0008



#endif //PROJECT_CONTS_H
