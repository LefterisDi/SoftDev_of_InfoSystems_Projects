/* File: relationStructs.hpp */

#ifndef __RELATION_STRCUS_HEADER__
#define __RELATION_STRCUS_HEADER__

#include "../templates/list.hpp"
#include "../templates/vector.hpp"

typedef struct Stats {
    uint64_t l_lower;
    uint64_t u_upper;
    uint32_t f_all;
    uint32_t d_distinct;
    bool*    distinctArray;
    uint32_t N;
}Stats;

typedef struct RelationTable {
    uint32_t   tableID;
    uint64_t   rows;
    uint64_t   cols;
    uint64_t** table;
    Stats*     colStats;
} RelationTable;

typedef struct JoinPred {
    uint64_t rel1;
    uint64_t rel2;
    uint64_t colRel1;
    uint64_t colRel2;
} JoinPred;

typedef struct JoinHashEntry {
    MiniVector<JoinPred> vectJP;
    uint32_t cost;
}JoinHashEntry;

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
