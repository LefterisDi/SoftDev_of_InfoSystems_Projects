#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <semaphore.h> 

#include "../utils/relationStructs.hpp"
#include "../sortingAlg/tablesort.hpp"
#include "../templates/list.hpp"
#include "../templates/vector.hpp"
#include "../templates/hashmap.hpp"
#include "../Jobs/Jobs.hpp"
#include "../JobScheduler/JobScheduler.hpp"


// A function to print all combination of a given length from the given array.
void GenSubSet(MiniVector< MiniVector<uint>* >& resVec , int reqLen, int start, int currLen, bool check[], uint16_t len) 
{
	// Return if the currrLen is more than the required length.
	if(currLen > reqLen)
	return;
	// If currLen is equal to required length then print the sequence.
	else if (currLen == reqLen) 
	{
		MiniVector<uint>* vec = new MiniVector<uint>;
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


void JoinEnumeration(RelationTable** relTable , uint16_t relTSize , List<JoinPred>* joinList){

    MyHashMap< uint , MiniVector<uint> > hmap( raiseToPower(2 , relTSize) - 1);

    for (uint i = 0 ; i < relTSize ; i++){
        int num = 1;
        MiniVector<uint> v;
        v.PushBack(i);
        hmap.set( (num<<1) , i);
    }

    for (int sNum = 1 ; sNum <= relTSize ; sNum++){
        MiniVector< MiniVector<uint>* > sets;
        bool check[sNum];
        GenSubSet(sets , sNum , 0 , 0 , check , relTSize);
        
    }

} 
