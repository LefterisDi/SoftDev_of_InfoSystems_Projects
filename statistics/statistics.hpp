#ifndef __STATISTICS_HPP__
#define __STATISTICS_HPP__

#include "../utils/relationStructs.hpp"

void InitialStats (RelationTable*&, uint32_t);
void _InitialStats(RelationTable*&, uint32_t);
void SelfJoinStats(TableStats& , uint64_t , uint64_t&);
int  JoinStats    (TableStats& , TableStats& , uint64_t , uint64_t , uint64_t&);

#endif
