#ifndef __JOBS_HPP__
#define __JOBS_HPP__

#include "../utils/relationStructs.hpp"
#include "../utils/String/String.hpp"

typedef struct QueryJobArgs{
    Query* query;
    uint16_t qNum;
    jd::String res;
}QueryJobArgs;

void QueryJob(void*); 

#endif