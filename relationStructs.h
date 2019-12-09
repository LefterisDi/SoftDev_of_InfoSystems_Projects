#ifndef __RELATIONSTRUCTS_H__
#define __RELATIONSTRUCTS_H__

#include <iostream>

#include "./templates/list.hpp"

typedef struct RelationTable{
    uint64_t rows;
    uint64_t cols;
    uint64_t** table;
}RelationTable;

typedef struct JoinPred{
    int type;
    uint64_t rel1;
    uint64_t rel2;
    uint64_t  colRel1;
    uint64_t  colRel2;
}JoinPred;

typedef struct CompPred{
    int type;
    uint64_t rel1;
    uint64_t  colRel1;
    uint64_t  num;
    char comp;
}CompPred;

typedef struct ResStruct{
    uint64_t tableID;
    List<uint64_t>* rowIDlist;
}ResStruct;

typedef struct FullResList{
    List<ResStruct>* tableList;
}FullResList;



#endif
