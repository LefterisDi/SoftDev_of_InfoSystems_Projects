#include <cstdint>
#include <iostream>
#include <semaphore.h> 
#include <unistd.h>
#include <bits/stdc++.h> 

#include "statistics.hpp"
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

int findPlace(MiniVector<uint32_t>* set , int relNum = -1){

	int res = 0;
	uint32_t pl = set->GetTotalItems();
	for (uint32_t i = 0 ; i < pl ; i++){
		uint32_t st = (*set)[i];
		uint32_t slide = (1<<st);
		res = res + slide;
	}

	if (relNum > -1){
		res = res + (1<<relNum);
	}

	return res;
}


bool ExistsInJheRels(JoinHashEntry& jhe , int relNum){

	for (int i = 0 ; i < jhe.rels.GetTotalItems() ; i++){
		if (jhe.rels[i] == relNum){
			return true;
		}
	}

	return false;
}

bool ExistsInJheJoinPreds(JoinHashEntry& jhe , int predNum){

	for (int i = 0 ; i < jhe.vectJPnum.GetTotalItems() ; i++){
		if (jhe.vectJPnum[i] == predNum){
			return true;
		}
	}

	return false;
}

uint64_t TreeCost(TableStats* relTableStats , JoinPred& jp){

		uint64_t cost;

		if (jp.rel1 == jp.rel2 && jp.colRel1 == jp.colRel2){
			SelfJoinStats(relTableStats[jp.rel1] , jp.colRel1 , cost);
		}
		else if (jp.rel1 == jp.rel2){
			std::cout << "RELLLLLLLLS" << std::endl;
		}
		else {
			JoinStats(relTableStats[jp.rel1] , relTableStats[jp.rel2] , jp.colRel1 , jp.colRel2 , cost);
		}

		return cost;
}

JoinHashEntry* CreateJoinTree(RelationTable** relTable , JoinHashEntry jhe , int relNum ,  List<JoinPred>* joinList){

	JoinHashEntry* newJhe = new JoinHashEntry;

	*newJhe = jhe;
	int indexKeeper = -1;
	uint64_t minCost = INT_MAX;

	for (int i = 0 ; i < joinList->GetTotalItems() ; i++){
		JoinPred* jp = &( (*(*joinList)[i])[0] );
		
		if (jp->rel1 == relNum && ExistsInJheRels(jhe , jp->rel2) == true && ExistsInJheJoinPreds(jhe , i) == false ){
			uint64_t newCost;
			if ( ( newCost = TreeCost(jhe.relTableStats , *jp) ) < minCost){
				minCost = newCost;
				indexKeeper = i;
			}
		}
		else if (jp->rel2 == relNum && ExistsInJheRels(jhe , jp->rel1) == true && ExistsInJheJoinPreds(jhe , i) == false ){
			uint64_t newCost;
			if ( ( newCost = TreeCost(jhe.relTableStats , *jp) ) < minCost){
				minCost = newCost;
				indexKeeper = i;
			}
		}
	}

	if (indexKeeper == -1){
		perror("Index keeper -1 error!");
		exit(1);
	}

	newJhe->cost = minCost;
	newJhe->rels.PushBack(relNum);
	newJhe->vectJPnum.PushBack(indexKeeper);

	return newJhe;
}

JoinHashEntry* JoinEnumeration(RelationTable** relTable , uint16_t relTSize , List<JoinPred>* joinList)
{
    MyHashMap< int , JoinHashEntry > hmap( raiseToPower(2 , relTSize) - 1);
	JoinHashEntry* res = new JoinHashEntry;

	int num = 1;
    for (int i = 0 ; i < relTSize ; i++){
        JoinHashEntry jhe;
		
		jhe.vectJPnum.PushBack(-1);
		jhe.rels.PushBack(i);
		jhe.tableNum = relTSize;
		jhe.relTableStats = new TableStats[relTSize];

		for (int i = 0; i < jhe.tableNum ; i++) {
			
			jhe.relTableStats[i].cols = relTable[i]->cols;
			jhe.relTableStats[i].statsPerCol = new Stats[relTable[i]->cols];

			for (int j = 0 ; j < jhe.relTableStats[i].cols ; j++){
				jhe.relTableStats[i].statsPerCol[j] = relTable[i]->colStats[j];
			}
		}

		jhe.cost = 0;
        hmap.set( num , jhe);
		num<<=1;
    }

	for (int i = 0 ; i < relTSize-1 ; i++) {

		MiniVector< MiniVector<uint32_t>* > sets;
		bool check[relTSize] = {};
		GenSubSet(sets , i+1 , 0 , 0 , check , relTSize);
		
        for (int sCount = 0 ; sCount < sets.GetTotalItems() ; sCount++) {

			for (int j = 0 ; j < relTSize ; j++) {
	
				bool isNull = false;
				JoinHashEntry* jhe = NULL;
				JoinHashEntry existingJHE;

                if ( existsInS(sets[sCount] , j))
					continue;
				else{
					try {
						hmap.get( findPlace(sets[sCount]) );
					} catch(const std::invalid_argument& arg) {
						continue;
					}
					if (connected(sets[sCount] , j , joinList) == false){
						continue;
					}
				}

				jhe = CreateJoinTree( relTable , hmap.get( findPlace(sets[sCount]) ) , j , joinList );

				try {
					existingJHE =  hmap.get( findPlace(sets[sCount] , j) );
				} catch(const std::invalid_argument& arg) {
					isNull = true;
				}

				if (isNull == true) {
					hmap.set(findPlace(sets[sCount] , j) , *jhe);
				
                } else if (existingJHE.cost > jhe->cost ) {
					hmap.delete_key( findPlace(sets[sCount] , j) );
					hmap.set(findPlace(sets[sCount] , j) , *jhe);
				}
			}
			delete sets[sCount];
		}
	}

	try {
		*res = hmap.get( raiseToPower(2 , relTSize) - 1 ) ;
	} catch(const std::invalid_argument& arg) {
		perror("Error finding best path!");
		exit(1);
	}
    
   return res;
} 