#ifndef __RELATIONSTRUCTS_H__
#define __RELATIONSTRUCTS_H__

#include <iostream>

typedef struct RelationTable{
    uint64_t rows;
    uint64_t cols;
    uint64_t** table;
}RelationTable;

#endif
