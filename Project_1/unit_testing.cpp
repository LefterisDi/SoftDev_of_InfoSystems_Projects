#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "./sortingAlg/tablesort.hpp"
#include "./templates/list.hpp"
#include "./opts/getopts.hpp"
#include "./utils/utils.hpp"

using namespace std;

int main()
{
    /****************************************************************
    *                        SORTING TESTING                        *
    ****************************************************************/

    uint32_t rows = 300000;

    uint64_t*   table1;
    uint64_t*   table2;
    MergeTuple* sortedTable;


    default_random_engine gen;
    uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    table1 = new uint64_t[rows];
    table2 = new uint64_t[rows];

    gen.seed(1);

    for (int i = 0 ; i < rows ; i++){
        table1[i] = distribution(gen);
        table2[i] = table1[i];
    }

    cout << "Testing sorting function..." << endl;

    sort(&table1[0] , &table1[rows]);

    sortedTable = TableSortOnKey(&table2 , rows , 1 , 0 , 500);


    for (int i = 0; i < rows ; i++){
        if (table1[i] != sortedTable[i].key){
            cout << "Sorting failed because the elements are not the same" << endl;
            exit(1);
        }
    }

    cout << "Sorting test successful!" << endl;


    /****************************************************************
    *                   MERGING AND LIST TESTING                    *
    ****************************************************************/

    List<uint64_t> list(1048576 , sizeof(uint64_t));

    MergeTuple sortedTable1[8] = {  {1 , 0} ,
                                    {1 , 1} ,
                                    {5 , 2} ,
                                    {8 , 3} ,
                                    {8 , 4} ,
                                    {8 , 5} ,
                                    {12 , 6} ,
                                    {12 , 7} };

    MergeTuple sortedTable2[5] = {  {1 , 0} ,
                                    {3 , 1} ,
                                    {8 , 2} ,
                                    {10 , 3} ,
                                    {12 , 4} };

    /*
     RESULT MUST BE
        0 0
        1 0
        3 2
        4 2
        5 2
        6 4
        7 4
    */

    cout << "Testing MergeTables function and list results ..." << endl;

    MergeTables(list , sortedTable1 , 8 , sortedTable2 , 5);

    cout << "MergeTables function and list results were correct, testing successful!" << endl;

    cout << "List Results" << endl;

    Bucket<uint64_t> bucket = *list.GetFirst();

    uint32_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;

    mask32_left <<= 32;

    cout << "| RowID1 | RowID2 |" << endl;
    for (int i = 0 ; i < list.GetTotalItems() ; i++) {
        for (int j = 0 ; j < bucket.GetBucketItems() ; j++) {
            cout << "| " << (bucket[j] & mask32_left) << "\t| " << (bucket[j] & mask32_right) << "\t|" << endl;
        }
        bucket = *bucket.GetNextBucket();
    }
}
