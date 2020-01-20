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


void JoinEnumeration(RelationTable** relTable , uint16_t relTSize , List<JoinPred>* joinList){

    MyHashMap< uint , MiniVector<JoinPred> > hmap( raiseToPower(2 , relTSize) - 1);
	uint numEntries = 0;
    
	int num = 1;
    for (uint i = 0 ; i < relTSize ; i++){
        MiniVector<JoinPred> v;
        v.PushBack(i);
        hmap.set( (num<<1) , v);
    }
	numEntries++;

    MiniVector< MiniVector<uint32_t>* > sets;
    bool check[2];
    GenSubSet(sets , 2 , 0 , 0 , check , relTSize);

    for (int i = 0 ; i < v.GetTotalItems() ; i++) {
        for (int j = 0 ; j < sets.GetTotalItems() ; j++) {
            
        }
        
    }

} 

uint TreeCost(RelationTable** relTable , MyHashMap< uint , MiniVector<uint> > givenTree , uint16_t relTSize , uint numEntries){

	// try {
	// 	hmap.get(12);
	// }catch(const std::invalid_argument& arg){
	// 	cout << "EXception" << endl;
	// }

	

}
