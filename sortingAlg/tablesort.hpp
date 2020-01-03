/* File: tablesort.hpp */

#ifndef __TABLE_SORT__
#define __TABLE_SORT__

#include <iostream>

#include "../utils/utils.hpp"

MergeTuple* TableSortOnKey(uint64_t **, uint32_t, int , int , uint32_t**& , uint32_t&);
void SimpleSortRec(MergeTuple* , MergeTuple* , uint32_t , int , int);

#endif // __TABLE_SORT__
