#include <cstdint>
#include <iostream>
#include <semaphore.h> 
#include <unistd.h>

#include "../utils/relationStructs.hpp"
#include "../sortingAlg/tablesort.hpp"
#include "../templates/list.hpp"
#include "../templates/vector.hpp"
#include "../templates/hashmap.hpp"
#include "../Jobs/Jobs.hpp"
#include "../JobScheduler/JobScheduler.hpp"


// A function to print all combination of a given length from the given array.
void GenSubSet(MiniVector<MiniVector<uint32_t>* >& resVec , int reqLen, int start, int currLen, bool check[], uint16_t len) 
{
	// Return if the currrLen is more than the required length.
	if(currLen > reqLen)
	return;
	// If currLen is equal to required length then print the sequence.
	else if (currLen == reqLen) 
	{
		MiniVector<uint32_t>* vec = new MiniVector<uint32_t>;
		for (int i = 0; i < len; i++) 
		{
			if (check[i] == true) 
			{
				vec->PushBack(i);
			}
		}
		resVec.PushBack(vec);
		return;
	}
	// If start equals to len then return since no further element left.
	if (start == len) 
	{
		return;
	}
	// For every index we have two options.
	// First is, we select it, means put true in check[] and increment currLen and start.
	check[start] = true;
	GenSubSet(resVec , reqLen, start + 1, currLen + 1, check, len);
	// Second is, we don't select it, means put false in check[] and only start incremented.
	check[start] = false;
	GenSubSet(resVec , reqLen, start + 1, currLen, check, len);
}

bool existsInS(MiniVector<uint32_t>* set , int relNum){
	
	for (int i = 0 ; i < set->GetTotalItems() ; i++){
		if (relNum == (*set)[i] ){
			return true;
		}
	}

	return false;
}

bool connected( MiniVector<uint32_t>* set , int relNum , List<JoinPred>* joinList){
	
	for(uint32_t i = 0 ; i < joinList->GetTotalItems() ; i++){
		
		JoinPred* jp = &( (*(*joinList)[i])[0] );
		
		for (uint32_t j = 0 ; j < set->GetTotalItems() ; j++){

			
			if ( (*set)[j] == jp->rel1 && relNum == jp->rel2 ){
				return true;
			}
			else if ( (*set)[j] == jp->rel2 && relNum == jp->rel1 ){
				return true;
			}
		}
	}

	return false;
}

int findPlace(MiniVector<uint32_t>* set , int relNum){

	int res = 0;
	for (uint32_t i = 0 ; i < set->GetTotalItems() ; i++){
		res = res + raiseToPower(2 , (*set)[i] );
	}
	res = res + raiseToPower(2 , relNum);
}

uint32_t TreeCost(){


}

void JoinEnumeration(RelationTable** relTable , uint16_t relTSize , List<JoinPred>* joinList)
{
    MyHashMap< uint32_t , MiniVector<JoinPred> > hmap( raiseToPower(2 , relTSize) - 1 );
	uint32_t numEntries = 0;

    MyHashMap< uint , JoinHashEntry > hmap( raiseToPower(2 , relTSize) - 1);
    
	int num = 1;
    for (int i = 0 ; i < relTSize ; i++){
        JoinHashEntry jhe;
		JoinPred jp;
		jp.rel1 = i;
		jp.rel2 = i;
		jp.colRel1 = 0;
		jp.colRel2 = 0;
        jhe.vectJP.PushBack(jp);
		jhe.cost = 0;
        hmap.set( (num<<1) , jhe);
    }

	for (int i = 0 ; i < relTSize-1 ; i++){
		MiniVector< MiniVector<uint32_t>* > sets;
		bool check[relTSize];
		GenSubSet(sets , i+1 , 0 , 0 , check , relTSize);
		for (int sCount = 0 ; sCount < sets.GetTotalItems() ; sCount++){
			
			for (int j = 0 ; j < relTSize ; j++){
	
				bool isNull = false;

				if (existsInS(sets[sCount] , j)){
					continue;
				}
				else if ( connected(sets[sCount] , j , joinList) == false ){
					continue;
				}

				try {
					hmap.get( findPlace(sets[sCount] , j) );
				}catch(const std::invalid_argument& arg){
					isNull = true;
				}

				// if (isNull == true || )

			}

			delete sets[sCount];
		}
		
	}
   
} 