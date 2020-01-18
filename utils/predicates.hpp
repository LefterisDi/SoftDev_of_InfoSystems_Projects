/* File: predicates.cpp */

#ifndef __PREDICATES_HEADER__
#define __PREDICATES_HEADER__

#include <cstdint>

#include "../utils/relationStructs.hpp"

ResStruct* FindInResList      (List<ResStruct> *, uint64_t);
ResStruct* FindInResVec       (MiniVector<ResStruct>* , uint64_t);

int        DeleteTargeted     (FullResList   *, int, List<uint64_t> *);
int        DeleteTargetedSL   (ResStruct     *, int, List<uint64_t> *);

int        JoinSelf           (RelationTable **, JoinPred &, List<FullResList> *);
int        JoinPredicate      (RelationTable **, JoinPred &, List<FullResList> *);
int        ComparisonPredicate(RelationTable **, CompPred &, List<FullResList> *);

int        DoAllCompPreds     (RelationTable **, List<CompPred> *, List<FullResList> *, bool *);
int        DoAllJoinPreds     (RelationTable **, List<JoinPred> *, List<FullResList> *, bool *);

#endif // __PREDICATES_HEADER__
