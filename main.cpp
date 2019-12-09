#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

// #include <list> 
// #include <iterator>

#include "./relationStructs.h"
#include "./sortingAlg/tablesort.hpp"
#include "./sortingAlg/quicksort.hpp"
#include "./templates/list.hpp"
#include "./opts/getopts.hpp"
#include "./utils/utils.hpp"


using namespace std;


ResStruct* FindInResList(List<ResStruct>& resList , uint64_t elemID){

    Bucket<ResStruct>* bucket = resList.GetFirst();
    ResStruct* found = NULL;

    while (bucket != NULL) {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
            if (elemID == (*bucket)[j].tableID){
                found = &(*bucket)[j];
                return found;
            }
        }

        bucket = bucket->GetNextBucket();
    }

    return found;
}

List<uint64_t>* ComparisonPredicate(RelationTable* relTable , CompPred& cpred , List<FullResList>& resList){
    
    Bucket<FullResList>* bucketFL = resList.GetFirst();
    ResStruct* existingRel = NULL;

    while (bucketFL != NULL) {
        for (int j = 0 ; j < bucketFL->GetBucketItems() ; j++) {
            existingRel = FindInResList( *( (*bucketFL)[j].tableList ) , cpred.rel1);
            if (existingRel != NULL){
                break;
            }
        }

        bucketFL = bucketFL->GetNextBucket();
    }

    if (existingRel == NULL){
        
    }


    // ResStruct* tableStruct = FindInResList(resList , cpred.rel1);
    // if (tableStruct == NULL){
    //     ResStruct* tableRes = new ResStruct;
    //     tableRes->tableID = cpred.rel1;
    //     for (uint64_t i = 0 ; i < relTable[cpred.rel1].rows ; i++){
    //         tableRes->rowIDlist->ListInsert(i);
    //     }
    //     resList.ListInsert(*tableRes);
    // }
    // switch (cpred.comp)
    // {
    // case '>':
    //     for (int i = 0; i < relTable[cpred.rel1].rows;i++){
    //         cout << relTable[cpred.rel1].table[cpred.colRel1][i] << endl;
    //         if (relTable[cpred.rel1].table[cpred.colRel1][i] > cpred.num){
    //             keyList->ListInsert((uint64_t)i);
    //         }
    //     }    
    //     break;
    
    // case '<':
    //     for (int i = 0; i < relTable[cpred.rel1].rows;i++){
    //         if (relTable[cpred.rel1].table[cpred.colRel1][i] < cpred.num){
    //             keyList->ListInsert((uint64_t)i);

    //         }
    //     }

    //     break;
    
    // case '=':
    //     for (int i = 0; i < relTable[cpred.rel1].rows;i++){
    //         if (relTable[cpred.rel1].table[cpred.colRel1][i] == cpred.num){
    //             keyList->ListInsert((uint64_t)i);

    //         }
    //     }

    //     break;
    // }

    // return keyList;
}

int main(int argc , char* argv[])
{
    list<JoinPred> joinList;
    list<JoinPred>::iterator itjp;
    list<CompPred> compList;
    list<CompPred>::iterator itcp;


    // uint32_t*  rowIDs1;
    // uint32_t*  rowIDs2;

    int relations = 3;
    RelationTable* relTable = new RelationTable[relations];
    relTable[0].rows = 4;
    relTable[0].cols = 5;
    relTable[0].table = new uint64_t*[relTable[0].cols];
    for(int i = 0; i < relTable[0].cols; i++)
        relTable[0].table[i] = new uint64_t[relTable[0].rows];

    relTable[1].rows = 6;
    relTable[1].cols = 3;
    relTable[1].table = new uint64_t*[relTable[1].cols];
    for(int i = 0; i < relTable[1].cols; i++)
        relTable[1].table[i] = new uint64_t[relTable[1].rows];

    relTable[2].rows = 6;
    relTable[2].cols = 4;
    relTable[2].table = new uint64_t*[relTable[2].cols];
    for(int i = 0; i < relTable[2].cols; i++)
        relTable[2].table[i] = new uint64_t[relTable[2].rows];



    default_random_engine gen;
    uniform_int_distribution<uint64_t> distribution(1,5);
    // uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    // rowIDs1 = new uint32_t[size1x];
    // rowIDs2 = new uint32_t[size2x];

    // gen.seed((unsigned int) time(NULL));
    gen.seed(2);

    // for (int i = 0 ; i < size1x ; i++)
    //     rowIDs1[i] = i;

    // for (int i = 0 ; i < size2x ; i++)
    //     rowIDs2[i] = i;
    for (int l = 0 ; l < relations ; l++){
        for (int i = 0 ; i < relTable[l].cols ; i++) {
            for (int j = 0 ; j < relTable[l].rows ; j++) {
                relTable[l].table[i][j] = distribution(gen);
            }
        }
    }

    for (int l = 0 ; l < relations ; l++){
        for (int i = 0 ; i < relTable[l].rows ; i++) {
            for (int j = 0 ; j < relTable[l].cols ; j++) {
                cout << relTable[l].table[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    

    JoinPred jp;
    CompPred cp;
    jp.type = 0;
    jp.rel1 = 0;
    jp.rel2 = 1;
    jp.colRel1 = 1;
    jp.colRel2 = 2;
    joinList.push_back(jp);
    jp.rel1 = 1;
    jp.rel2 = 2;
    jp.colRel1 = 0;
    jp.colRel2 = 1;
    joinList.push_back(jp);
    cp.type = 1;
    cp.rel1 = 0;
    cp.colRel1 = 1;
    cp.comp = '<';
    cp.num = (uint64_t)3536385184864502858;
    compList.push_back(cp);

    
    List<uint64_t>* rowIDlist;
    List<FullResList>* resList = new List<FullResList>(sizeof(ResStruct) , sizeof(ResStruct));

    for (itcp = compList.begin(); itcp != compList.end(); itcp++){
        //do every comp
        rowIDlist = ComparisonPredicate(relTable , *itcp , *resList);
       
        Bucket<uint64_t>* bucket = rowIDlist->GetFirst();

        while (bucket != NULL) {
            for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
                cout << (*bucket)[j] << endl;
            }

            bucket = bucket->GetNextBucket();
        }
    }

    // List<uint64_t>* rowIDlistJoin;
    // list<uint64_t>::iterator itj;

    // for (itjp = joinList.begin(); itjp != joinList.end() ; itjp++){
    //     //do joins
    //     rowIDlistJoin = JoinPredicate(relTable , *itjp);

    //     Bucket<uint64_t>* bucket = rowIDlistJoin->GetFirst();

    //     uint64_t mask32_left  = 0xFFFFFFFF;
    //     uint32_t mask32_right = 0xFFFFFFFF;
    //     mask32_left <<= 32;

    //     while (bucket != NULL) {
    //         for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
    //             cout << (((*bucket)[j] & mask32_left) >> 32) << " , " << ((*bucket)[j] & mask32_right) << endl;
    //         }

    //         bucket = bucket->GetNextBucket();
    //     }
    // }



    for (int l = 0 ; l < relations ; l++){
        for(int i = 0; i < relTable[l].cols; i++)
            delete[] relTable[l].table[i];
        delete[] relTable[l].table;
    }
    
}
 