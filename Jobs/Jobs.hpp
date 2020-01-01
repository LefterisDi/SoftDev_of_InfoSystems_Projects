#ifndef __JOBS_HPP__
#define __JOBS_HPP__

#include "../utils/relationStructs.hpp"
#include "../utils/String/String.hpp"
#include "../utils/utils.hpp"

typedef struct QueryJobArgs{
    Query* query;
    uint16_t qNum;
    jd::String res;
}QueryJobArgs;

typedef struct SortJobArgs{
    MergeTuple* table1;
    MergeTuple* table2; 
    uint32_t size;
    int key;
    int qsAfterNumOfEntries;
}SortJobArgs;

void QueryJob(void*); 
void SortJob(void*);

#endif