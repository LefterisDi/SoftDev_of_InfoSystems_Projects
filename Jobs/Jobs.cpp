#include <iostream>
#include <semaphore.h>
#include <cinttypes>
#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "../utils/relationStructs.hpp"
#include "../templates/list.hpp"
#include "../templates/vector.hpp"
#include "../utils/predicates.hpp"
#include "../JobScheduler/JobScheduler.hpp"
#include "../utils/String/String.hpp"
#include "./Jobs.hpp"
#include "../sortingAlg/quicksort.hpp"
#include "../utils/utils.hpp"


using namespace std;

void SortJob(void* arg){

    SortJobArgs* sja = (SortJobArgs*)arg;

    uint32_t hist[UCHAR_MAX+1] = {};

    if (sja->key == 8)
        return;

    // Checks if the quicksort conditions are met
    if (sja->size <= sja->qsAfterNumOfEntries/* 64KB or 8192 entries */) {
        quickSort(sja->table1 , 0 , sja->size - 1);

        for (int i = 0 ; i < sja->size ; i++) {
            sja->table2[i] = sja->table1[i];
        }
        return;
    }

    // Makes the histogram
    for (int i = 0 ; i < sja->size ; i++)
        hist[BitConversion(sja->table1[i].key , sja->key)]++;

    int psumCount = 0;

    for (int i = 0 ; i <= UCHAR_MAX ; i++)
        if (hist[i] != 0)
            psumCount++;

    uint32_t psum[psumCount][2] = {};

    int ind     = 0;
    int itCount = 0;

    // Creates psum table
    for (int i = 0 ; i <= UCHAR_MAX ; i++) {
        if (hist[i] != 0) {
            psum[ind][0] = i;
            psum[ind][1] = itCount;
            itCount += hist[i];
            ind++;
        }
    }

    int table2Ind = 0;
    // Sorting the second array using 8 bits dependent on the key given
    for(int i = 0 ; i < psumCount ; i++) {
        for (int j = 0 ; j < sja->size ; j++) {
            if (psum[i][0] == BitConversion(sja->table1[j].key , sja->key)) {
                sja->table2[table2Ind] = sja->table1[j];
                table2Ind++;
            }
        }
    }

    // Writes the results on the first array since the second array is now sorted
    for (int i = 0; i < sja->size; i++) {
        sja->table1[i] = sja->table2[i];
    }

    JobScheduler* js = new JobScheduler(psumCount , psumCount+1);
    SortJobArgs* sjaNew = new SortJobArgs[psumCount];

    int newKey = sja->key + 1;

    for (int i = 0 ; i < psumCount ; i++) {
        
        if (i < psumCount-1){
            sjaNew[i].table1 = &sja->table2[psum[i][1]];
            sjaNew[i].table2 = &sja->table1[psum[i][1]];
            sjaNew[i].size = psum[i+1][1] - psum[i][1];
            sjaNew[i].key = newKey;
            sjaNew[i].qsAfterNumOfEntries = sja->qsAfterNumOfEntries;

            js->addNewJob(&SortJob , (void*)&sjaNew[i]);
        }

        else if (i == psumCount-1){
            sjaNew[i].table1 = &sja->table2[psum[i][1]];
            sjaNew[i].table2 = &sja->table1[psum[i][1]];
            sjaNew[i].size = ((uint32_t)sja->size) - psum[i][1];
            sjaNew[i].key = newKey;
            sjaNew[i].qsAfterNumOfEntries = sja->qsAfterNumOfEntries;

            js->addNewJob(&SortJob , (void*)&sjaNew[i]);
        }
    }

    js->destroyScheduler(1);
    delete[] sjaNew;
}

void QueryJob(void* arg){

    QueryJobArgs* qja = (QueryJobArgs*)arg;
    Query* query = qja->query;
    jd::String resStr = "";
    

    bool* relExistsInRL = new bool[query->total_rels]();
    for (uint32_t j = 0; j < query->total_rels ; j++)
        relExistsInRL[j] = false;

    List<FullResList>* resList = new List<FullResList>(sizeof(ResStruct) , sizeof(ResStruct));

    DoAllCompPreds( query->query_rels , query->comp_preds , resList , relExistsInRL);//must make parallel
    DoAllJoinPreds( query->query_rels , query->join_preds , resList , relExistsInRL);

    for (uint32_t l = 0 ; l < query->proj->GetTotalItems() ; l++) {

        Projection* pr  = &( (*(*query->proj)[l])[0] );
        ResStruct*  res = NULL;

        for (uint32_t m = 0 ; m < resList->GetTotalItems() ; m++) {

            FullResList* fres = &( (*(*resList)[m])[0] );
            res = FindInResList(fres->tableList , pr->rel);
            if (res != NULL){
                break;
            }
        }

        if (res == NULL)
            exit(1);

        uint64_t sum = 0;
        for (uint32_t m = 0 ; m < res->rowIDvec->GetTotalItems() ; m++) {
            uint64_t rowID = (*res->rowIDvec)[m];
            sum += query->query_rels[pr->rel]->table[pr->colRel][rowID];
        }

        if (sum == 0){
            if (l != query->proj->GetTotalItems()-1){
                resStr += "NULL ";
            }
            else {
                resStr += "NULL";
            }

        }
        else{
            if (l != query->proj->GetTotalItems()-1){
                char buff[21];
                sprintf(buff, "%" PRIu64, sum);
                resStr += buff;
                resStr += " ";
                
            }
            else {
                char buff[21];
                sprintf(buff, "%" PRIu64, sum);
                resStr += buff;
            }
        }
    }
    qja->res = resStr;

    delete[] relExistsInRL;
    for (uint32_t l = 0 ; l < resList->GetTotalItems() ; l++) {

        List<ResStruct>* tableList = (*(*resList)[l])[0].tableList;
        for (uint32_t k = 0 ; k < tableList->GetTotalItems() ; k++)
            delete (*(*tableList)[k])[0].rowIDvec;
        delete tableList;
    }
    delete resList;

    delete query->comp_preds;
    delete query->join_preds;
    delete query->proj;
    delete[] query->query_rels;
}