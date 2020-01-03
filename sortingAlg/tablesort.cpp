/* File: tablesort.cpp */

#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "../searchingAlg/binarySearch.hpp"
#include "../sortingAlg/quicksort.hpp"
#include "../utils/utils.hpp"
#include "../Jobs/Jobs.hpp"
#include "../JobScheduler/JobScheduler.hpp"

using namespace std;

void SimpleSortRec(MergeTuple* table1 , MergeTuple* table2 , uint32_t size , int key , int qsAfterNumOfEntries)
{
    uint32_t hist[UCHAR_MAX+1] = {};

    if (key == 8)
        return;

    // Checks if the quicksort conditions are met
    if (size <= qsAfterNumOfEntries/* 64KB or 8192 entries */) {
        quickSort(table1 , 0 , size - 1);

        for (int i = 0 ; i < size ; i++) {
            table2[i] = table1[i];
        }
        return;
    }

    // Makes the histogram
    for (int i = 0 ; i < size ; i++)
        hist[BitConversion(table1[i].key , key)]++;

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
        for (int j = 0 ; j < size ; j++) {
            if (psum[i][0] == BitConversion(table1[j].key , key)) {
                table2[table2Ind] = table1[j];
                table2Ind++;
            }
        }
    }

    // Writes the results on the first array since the second array is now sorted
    for (int i = 0; i < size; i++) {
        table1[i] = table2[i];
    }

    int newKey = key + 1;


    for (int i = 0 ; i < psumCount ; i++) {
        if (i < psumCount-1)
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , psum[i+1][1] - psum[i][1] , newKey , qsAfterNumOfEntries);

        else if (i == psumCount-1)
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , ((uint32_t)size) - psum[i][1] , newKey , qsAfterNumOfEntries);
    }
}

void SortJobFirst(SortJobArgs*& arg){

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

    

    if (sja->key == 0){
        sja->psum = new uint32_t*[psumCount];
        for (uint32_t k = 0; k < psumCount ; k++){
            sja->psum[k] = new uint32_t[2];
            sja->psum[k][0] = psum[k][0];
            sja->psum[k][1] = psum[k][1];
            // cout << "DORT " << psum[k][0] << " " << psum[k][1] << endl;
        }
        sja->psumCount = psumCount;
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
    delete js;
    delete[] sjaNew;
}

MergeTuple* TableSortOnKey(uint64_t** tableMain, uint32_t rows , int key , int entriesQuicksort , uint32_t**& psum , uint32_t& psumCount)
{
    MergeTuple* table1;
    MergeTuple* table2;

    table1 = new MergeTuple[rows];
    table2 = new MergeTuple[rows];

    for (uint32_t i = 0 ; i < rows ; i++) {
        table1[i].key   = tableMain[key][i];
        table1[i].rowID = i;
    }

    SortJobArgs* sja = new SortJobArgs;

    sja->table1 = table1;
    sja->table2 = table2;
    sja->size = rows;
    sja->key = 0;
    sja->qsAfterNumOfEntries = entriesQuicksort;
    sja->psum = NULL;
    sja->psumCount = 0;

    SortJobFirst(sja);
    // SimpleSortRec(table1 , table2 , rows , 0 , entriesQuicksort);

    psum = sja->psum;
    psumCount = sja->psumCount;


    // for (uint32_t l = 0 ; l < psumCount ; l++){
    //     cout << "OUT " << psum[l][0] << " " << psum[l][1] << endl;
    // }

    delete sja;

    delete[] table1;

    return table2;
}
