#ifndef __JOBS_HPP__
#define __JOBS_HPP__

#include "../utils/relationStructs.hpp"
#include "../utils/string.hpp"
#include "../utils/utils.hpp"

typedef struct QueryJobArgs{
    Query* query;
    uint16_t qNum;
    str::string res;
}QueryJobArgs;

typedef struct SortJobArgs{
    MergeTuple* table1;
    MergeTuple* table2; 
    uint32_t size;
    int key;
    int qsAfterNumOfEntries;
    uint32_t** psum;
    uint32_t psumCount;
}SortJobArgs;

typedef struct MergeJobArgs{
    List<uint64_t>** list;
    MergeTuple* sortedTable1;
    uint32_t size1X;
    MergeTuple* sortedTable2;
    uint32_t size2X;
}MergeJobArgs;

void QueryJob(void*); 
void SortJob(void*);
void MergeJob(void*);

#endif