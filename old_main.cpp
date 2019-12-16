/* File: main.cpp */

#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include <list> 
#include <iterator>


#include "./relationStructs.hpp"
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

List<uint64_t>* JoinPredicate(RelationTable* relTable , JoinPred& jpred , List<ResStruct>& resList){

    List<uint64_t>* doubleKeyList = new List<uint64_t>(1048576 , sizeof(uint64_t));
    uint64_t** table1;
    uint64_t** table2;
    if (FindInResList(resList , jpred.rel1) != NULL){

        ResStruct* tableRes = FindInResList(resList , jpred.rel1);
        table1 = new uint64_t*[relTable[jpred.rel1].cols];
        uint32_t totalItems = tableRes->rowIDlist->GetTotalItems();
        for(int i = 0; i < relTable[jpred.rel1].cols; i++)
            table1[i] = new uint64_t[totalItems];


        Bucket<uint64_t>* bucket = tableRes->rowIDlist->GetFirst();

        //columns change with the first parameter and rowIDs with the second
        for (int j =  0 ; j < totalItems ; j++){
            for (int i = 0 ; i < relTable[jpred.rel1].cols ; i++){
                table1[i][j] =  relTable[jpred.rel1].table[i][ (*bucket)[0] ];
            }
            bucket = bucket->GetNextBucket();
        }


        for(int i = 0; i < relTable[jpred.rel1].cols; i++)
            delete[] table1[i];
        delete table1;
    }
    else {
        ResStruct* tableRes = new ResStruct;
        tableRes->tableID = jpred.rel1;
        for (uint64_t i = 0 ; i < relTable[jpred.rel1].rows ; i++){
            tableRes->rowIDlist->ListInsert(i);
        }
        resList.ListInsert(*tableRes);

        table1 = relTable[jpred.rel1].table;

        delete tableRes;
    }

    if (FindInResList(resList , jpred.rel2) != NULL){

        ResStruct* tableRes = FindInResList(resList , jpred.rel2);
        table2 = new uint64_t*[relTable[jpred.rel2].cols];
        uint32_t totalItems = tableRes->rowIDlist->GetTotalItems();
        for(int i = 0; i < relTable[jpred.rel2].cols; i++)
            table2[i] = new uint64_t[totalItems];


        Bucket<uint64_t>* bucket = tableRes->rowIDlist->GetFirst();

        //columns change with the first parameter and rowIDs with the second
        for (int j =  0 ; j < totalItems ; j++){
            for (int i = 0 ; i < relTable[jpred.rel2].cols ; i++){
                table2[i][j] =  relTable[jpred.rel2].table[i][ (*bucket)[0] ];
            }
            bucket = bucket->GetNextBucket();
        }

        for(int i = 0; i < relTable[jpred.rel2].cols; i++)
            delete[] table2[i];
        delete table2;
    }
    else {
        ResStruct* tableRes = new ResStruct;
        tableRes->tableID = jpred.rel2;
        for (uint64_t i = 0 ; i < relTable[jpred.rel2].rows ; i++){
            tableRes->rowIDlist->ListInsert(i);
        }
        resList.ListInsert(*tableRes);

        table2 = relTable[jpred.rel2].table;

        delete tableRes;
    }

    MergeTuple* sortedTable1 = TableSortOnKey(table1 , FindInResList(resList , jpred.rel1)->rowIDlist->GetTotalItems() , jpred.colRel1 , 8192);
    MergeTuple* sortedTable2 = TableSortOnKey(table2 , FindInResList(resList , jpred.rel2)->rowIDlist->GetTotalItems() , jpred.colRel2 , 8192);

    // for (int i = 0 ; i < relTable[jpred.rel1].rows ; i++){
    //     cout << sortedTable1[i].key << endl;
    // }

    MergeTables(*doubleKeyList, sortedTable1 , relTable[jpred.rel1].rows , sortedTable2 , relTable[jpred.rel2].rows);

    uint32_t size = doubleKeyList->GetTotalItems();

    uint32_t* rowIDt1 = new uint32_t[size];
    uint32_t* rowIDt2 = new uint32_t[size];

    Bucket<uint64_t>* bucketDK = doubleKeyList->GetFirst();

    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
    mask32_left <<= 32;

    uint32_t i = 0;
    while(bucketDK != NULL){
        
        for (int j = 0 ; j < bucketDK->GetBucketItems() ; j++) {
            rowIDt1[i] = ( (*bucketDK)[j] & mask32_left ) >> 32;
            rowIDt2[i] = ( (*bucketDK)[j] & mask32_right );
            i++;
        }
    }

    quickSort(rowIDt1 , 0 , size-1);
    quickSort(rowIDt2 , 0 , size-1);

    ResStruct* tmp = FindInResList(resList , jpred.rel1);
    
    Bucket<uint64_t>* bucketRL = tmp->rowIDlist->GetFirst();

    i = 0;
    

    @@@@@@@@@@ DELETE THE BUCKETS THAT DONT FIT IN THE CRITERIA @@@@@@@@@@@
    while ( (*bucketRL)[0] != rowIDt1[0] ){
        //delete the first element
    }

    delete[] rowIDt1;
    delete[] rowIDt2;

    return doubleKeyList;
}


List<uint64_t>* ComparisonPredicate(RelationTable* relTable , CompPred& cpred , List<ResStruct>& resList){
    
    ResStruct* tableStruct = FindInResList(resList , cpred.rel1);
    if (tableStruct == NULL){
        ResStruct* tableRes = new ResStruct;
        tableRes->tableID = cpred.rel1;
        for (uint64_t i = 0 ; i < relTable[cpred.rel1].rows ; i++){
            tableRes->rowIDlist->ListInsert(i);
        }
        resList.ListInsert(*tableRes);
    }
    switch (cpred.comp)
    {
    case '>':
        for (int i = 0; i < relTable[cpred.rel1].rows;i++){
            cout << relTable[cpred.rel1].table[cpred.colRel1][i] << endl;
            if (relTable[cpred.rel1].table[cpred.colRel1][i] > cpred.num){
                keyList->ListInsert((uint64_t)i);
            }
        }    
        break;
    
    case '<':
        for (int i = 0; i < relTable[cpred.rel1].rows;i++){
            if (relTable[cpred.rel1].table[cpred.colRel1][i] < cpred.num){
                keyList->ListInsert((uint64_t)i);

            }
        }

        break;
    
    case '=':
        for (int i = 0; i < relTable[cpred.rel1].rows;i++){
            if (relTable[cpred.rel1].table[cpred.colRel1][i] == cpred.num){
                keyList->ListInsert((uint64_t)i);

            }
        }

        break;
    }

    return keyList;
}

int FindInList(List<uint64_t>* rowIDlist , uint64_t elemID , int numOfElems = 1){

    if (rowIDlist == NULL || numOfElems > 2 || numOfElems < 1){
        return -1;
    }

    Bucket<uint64_t>* bucket = rowIDlist->GetFirst();

    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
    mask32_left <<= 32;

    while (bucket != NULL) {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
            if (numOfElems == 2){
                if ( ( ( (*bucket)[j] & mask32_left ) >> 32 ) == elemID){
                    return 1;
                }
                else if ( ( (*bucket)[j] & mask32_right ) == elemID){
                    return 1;
                } 
            }
            else if ( (*bucket)[j] == elemID ){
                return 1;
            }
        }

        bucket = bucket->GetNextBucket();
    }

    return 0;

}



// List<uint64_t>* FindInList(List<ResStruct>* rowIDlist , uint64_t elemID){

//     if (rowIDlist == NULL){
//         return NULL;
//     }

//     Bucket<ResStruct>* bucket = rowIDlist->GetFirst();

//     while (bucket != NULL) {
//         for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
//             if ( (*bucket)[j].type == 0 ){
//                 if ((*bucket)[j].tableID1 == elemID || (*bucket)[j].tableID2 == elemID ){
//                     return (*bucket)[j].resList;
//                 }
//             }
//             else if ((*bucket)[j].tableID1 == elemID){
//                 return (*bucket)[j].resList;
//             }
//         }

//         bucket = bucket->GetNextBucket();
//     }

//     return NULL;
// }


// List<ResStruct>* DoAllCompPred(List<CompPred>& compList , RelationTable* relTable){

//     List<ResStruct>* resList = new List<ResStruct>(10*sizeof(ResStruct) , sizeof(ResStruct));
//     Bucket<CompPred>* bucket = compList.GetFirst();
    
//     while (bucket != NULL) {
//         for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
//             // if (FindInList(resList , (*bucket)[j].rel1)){

//             // }
//             ResStruct* res = new ResStruct;
//             res->resList = ComparisonPredicate(relTable , (*bucket)[j]);
//             res->type = 1;
//             res->tableID1 = (*bucket)[j].rel1;
//             res->tableID2 = ULLONG_MAX;
//             resList->ListInsert(*res);
//             free(res);

//         }
//         bucket = bucket->GetNextBucket();
//     }
// }



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
    List<ResStruct>* resList = new List<ResStruct>(sizeof(ResStruct) , sizeof(ResStruct));

    for (itcp = compList.begin(); itcp != compList.end(); itcp++){
        //do every comp
        rowIDlist = ComparisonPredicate(relTable , *itcp);
        ResStruct* strc = new ResStruct;
        strc->type = 1;
        strc->resList = rowIDlist;
        strc->tableID1 = itcp->rel1;
        strc->tableID2 = -1;
        resList->ListInsert(*strc);
        cout << FindInList(resList , itcp->rel1) << endl;
        Bucket<uint64_t>* bucket = rowIDlist->GetFirst();

        while (bucket != NULL) {
            for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
                cout << (*bucket)[j] << endl;
            }

            bucket = bucket->GetNextBucket();
        }
    }

    List<uint64_t>* rowIDlistJoin;
    list<uint64_t>::iterator itj;

    for (itjp = joinList.begin(); itjp != joinList.end() ; itjp++){
        //do joins
        rowIDlistJoin = JoinPredicate(relTable , *itjp);

        Bucket<uint64_t>* bucket = rowIDlistJoin->GetFirst();

        uint64_t mask32_left  = 0xFFFFFFFF;
        uint32_t mask32_right = 0xFFFFFFFF;
        mask32_left <<= 32;

        while (bucket != NULL) {
            for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
                cout << (((*bucket)[j] & mask32_left) >> 32) << " , " << ((*bucket)[j] & mask32_right) << endl;
            }

            bucket = bucket->GetNextBucket();
        }

        // for (itj = rowIDlistJoin.begin() ; itj != rowIDlistJoin.end() ; itj++){
        //     cout << *itj << endl;
        // }
    }



    for (int l = 0 ; l < relations ; l++){
        for(int i = 0; i < relTable[l].cols; i++)
            delete[] relTable[l].table[i];
        delete[] relTable[l].table;
    }
    
    // delete[] rowIDs1;
    // delete[] rowIDs2;
}
 
