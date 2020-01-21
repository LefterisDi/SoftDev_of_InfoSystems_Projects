/* File: utils.hpp */

#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

#include "../utils/relationStructs.hpp"
#include "../templates/list.hpp"

typedef struct tuple {
    uint64_t key;
    uint64_t rowID;
} MergeTuple;

List<Query>*         ReadQueryBatches(const char *, const char *, List<RelationTable> &);
List<RelationTable>* ReadRelations   (const char * , bool);
uint32_t             BitConversion   (uint64_t, uint32_t);
void                 SwitchElements  (uint64_t **, uint32_t, uint32_t, uint32_t);
void                 MergeTables     (List<uint64_t> &, MergeTuple*, uint32_t, MergeTuple*, uint32_t);
void                 MergeJobFirst   (List<uint64_t>**, MergeTuple*, uint32_t, MergeTuple*, uint32_t ,uint32_t** , uint32_t , uint32_t** , uint32_t);
uint64_t             raiseToPower    (uint64_t, uint64_t = 0);

#endif // __UTILS_HEADER__
