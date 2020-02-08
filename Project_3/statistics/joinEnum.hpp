#ifndef __JOIN_ENUM_HPP__
#define __JOIN_ENUM_HPP__

#include "../utils/relationStructs.hpp"
#include "../templates/list.hpp"

JoinHashEntry* JoinEnumeration(RelationTable** , uint16_t , List<JoinPred>*); 

#endif