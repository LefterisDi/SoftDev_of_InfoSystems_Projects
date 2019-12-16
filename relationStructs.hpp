/* File: relationStructs.hpp */

#ifndef __RELATION_STRCUS_HEADER__
#define __RELATION_STRCUS_HEADER__

#include "./templates/list.hpp"

typedef struct RelationTable {
    uint64_t   rows;
    uint64_t   cols;
    uint64_t** table;
} RelationTable;

typedef struct JoinPred {
    uint32_t type;
    uint64_t rel1;
    uint64_t rel2;
    uint64_t colRel1;
    uint64_t colRel2;
} JoinPred;

typedef struct CompPred {
    char     comp;
    uint32_t type;
    uint64_t rel1;
    uint64_t colRel1;
    uint64_t num;
} CompPred;

typedef struct ResStruct {
    uint64_t tableID;
    List<uint64_t>* rowIDlist;
} ResStruct;

typedef struct FullResList {
    List<ResStruct>* tableList;
} FullResList;

#endif // __RELATION_STRCUS_HEADER__
