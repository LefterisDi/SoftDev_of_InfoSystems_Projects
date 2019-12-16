/* File: predicates.cpp */

#ifndef __PREDICATES_HEADER__
#define __PREDICATES_HEADER__

#include "../relationStructs.hpp"

ResStruct* FindInResList      (List<ResStruct> *, uint64_t);

int        DeleteTargeted     (FullResList   *, int, List<uint64_t> *);
int        DeleteTargetedSL   (ResStruct     *, int, List<uint64_t> *);

int        JoinSelf           (List<RelationTable>*, JoinPred &, List<FullResList> *);
int        JoinPredicate      (List<RelationTable>*, JoinPred &, List<FullResList> *);
int        ComparisonPredicate(List<RelationTable>*, CompPred &, List<FullResList> *);

int        DoAllCompPreds     (List<RelationTable>*, List<CompPred> *, List<FullResList> *, bool *);
int        DoAllJoinPreds     (List<RelationTable>*, List<JoinPred> *, List<FullResList> *, bool *);

#endif // __PREDICATES_HEADER__
