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

using namespace std;

void SimpleSortRec(MergeTuple* table1 , MergeTuple* table2 , int size , int key , int qsAfterNumOfEntries)
{
    uint32_t hist[UCHAR_MAX+1] = {};

    if (size < 2 || key == 8)
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

MergeTuple* TableSortOnKey(uint64_t** tableMain, int rows , int key , int entriesQuicksort)
{
    MergeTuple* table1;
    MergeTuple* table2;

    table1 = new MergeTuple[rows];
    table2 = new MergeTuple[rows];

    for (int i = 0 ; i < rows ; i++) {
        table1[i].key   = tableMain[key][i];
        table1[i].rowID = i;
    }

    SimpleSortRec(table1 , table2 , rows , 0 , entriesQuicksort);

    delete[] table1;

    return table2;
}
