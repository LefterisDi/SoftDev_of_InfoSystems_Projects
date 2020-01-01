/* File: relationStructs.hpp */

#ifndef __RELATION_STRCUS_HEADER__
#define __RELATION_STRCUS_HEADER__

#include "../templates/list.hpp"
#include "../templates/vector.hpp"

typedef struct RelationTable {
    uint32_t   tableID;
    uint64_t   rows;
    uint64_t   cols;
    uint64_t** table;
} RelationTable;

typedef struct JoinPred {
    uint64_t rel1;
    uint64_t rel2;
    uint64_t colRel1;
    uint64_t colRel2;
} JoinPred;

typedef struct CompPred {
    char     comp;
    uint64_t rel1;
    uint64_t colRel1;
    uint64_t num;
} CompPred;

typedef struct Projection {
    uint64_t rel;
    uint64_t colRel;
} Projection;

typedef struct Query {
    uint32_t          total_rels;
    RelationTable**   query_rels;
    List<CompPred>*   comp_preds;
    List<JoinPred>*   join_preds;
    List<Projection>* proj;
} Query;

typedef struct ResStruct {
    uint64_t tableID;
    MiniVector<uint64_t>* rowIDvec;
} ResStruct;

typedef struct FullResList {
    List<ResStruct>* tableList;
} FullResList;

#endif // __RELATION_STRCUS_HEADER__
