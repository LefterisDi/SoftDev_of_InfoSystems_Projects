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

void SimpleSortRec(uint64_t* table1 , uint64_t* table2 , int size , int key , int qsAfterNumOfEntries)
{
    uint32_t hist[UCHAR_MAX+1] = {};

    if (size < 2 || key == 8)
        return;

    if (size <= qsAfterNumOfEntries/*64KB or 8192 entries*/) {
        quickSort(table1 , 0 , size - 1);

        for (int i = 0 ; i < size ; i++) {
            table2[i] = table1[i];
        }
        return;
    }

    for (int i = 0 ; i < size ; i++)
        hist[BitConversion(table1[i] , key)]++;

    int psumCount = 0;

    for (int i = 0 ; i <= UCHAR_MAX ; i++)
        if (hist[i] != 0)
            psumCount++;

    uint32_t psum[psumCount][2] = {};

    int ind     = 0;
    int itCount = 0;

    for (int i = 0 ; i <= UCHAR_MAX ; i++) {
        if (hist[i] != 0) {
            psum[ind][0] = i;
            psum[ind][1] = itCount;
            itCount += hist[i];
            ind++;
        }
    }

    int table2Ind = 0;
    for(int i = 0 ; i < psumCount ; i++) {
        for (int j = 0 ; j < size ; j++) {
            if (psum[i][0] == BitConversion(table1[j] , key)) {
                table2[table2Ind] = table1[j];
                table2Ind++;
            }
        }
    }

    int newKey = key + 1;

    for (int i = 0 ; i < psumCount ; i++) {
        if (i < psumCount-1)
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , psum[i+1][1] - psum[i][1] , newKey , qsAfterNumOfEntries);

        else if (i == psumCount-1)
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , ((uint32_t)size) - psum[i][1] , newKey , qsAfterNumOfEntries);
    }
}

void TableSortOnKey(uint64_t** tableMain, uint32_t* rowIDs, int sizeX, int sizeY, int key)
{
    uint64_t* table1;
    uint64_t* table2;
    // uint64_t* table3 =  new uint64_t[sizeX];

    bool* correctionTable = new bool[sizeX];

    int entriesQuicksort = 8192;

    table1 = new uint64_t[sizeX];
    table2 = new uint64_t[sizeX];

    for (int i = 0 ; i < sizeX ; i++) {
        table1[i]          = tableMain[key][i];
        correctionTable[i] = false;
        // table3[i]          = tableMain[key][i];
    }

    SimpleSortRec(table1 , table2 , sizeX , 0 , entriesQuicksort);

    int sortedElems = 0;
    int ElemInd     = 0;

    while (sortedElems < sizeX)
    {
        if (correctionTable[ElemInd] == true) {
            ElemInd++;
            continue;
        }

        int index = binarySearch(table2 ,correctionTable , 0 , sizeX-1 , tableMain[key][ElemInd]);

        if (index == -1) {
            std::cout << "ELEMENT NOT FOUND ERROR!" << std::endl;
            exit(1);
        }

        correctionTable[index] = true;
        sortedElems++;

        if (ElemInd != index) {
            SwitchElements(tableMain , sizeY , ElemInd , index);
            uint32_t tmp    = rowIDs[ElemInd];
            rowIDs[ElemInd] = rowIDs[index];
            rowIDs[index]   = tmp;
        }


        // for (int i = 0 ; i < sizeY ; i++){
        //     for (int j = 0 ; j < sizeX ; j++){
        //         std::cout << tableMain[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        //  for (int i = 0 ; i < sizeX ; i++){
        //      std::cout << correctionTable[i] << " ";
        //  }

        //  std::cout << std::endl;
        //  std::cout << std::endl;
    }

    // std::sort(&table3[0] , &table3[sizeX]);
    //
    // bool error = false;
    // for (int i = 0 ; i < sizeX ; i++){
    //     // std::cout << table2[i] << " " << table1[i] << std::endl;
    //     if (table2[i] != table3[i]){
    //         error = true;
    //         std::cout << "ERROR SORTING IS NOT CORRECT" << std::endl;
    //         std::cin.get();
    //         exit(1);
    //     }
    // }
    //
    // std::cout << error << std::endl;

    delete[] table1;
    delete[] table2;
    // delete[] table3;
    delete[] correctionTable;
}
