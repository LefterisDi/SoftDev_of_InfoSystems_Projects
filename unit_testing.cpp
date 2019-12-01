#include <bits/stdc++.h>
#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
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

    uint32_t rows = 30000;

    uint64_t*   table1;
    uint64_t*   table2;
    MergeTuple* sortedTable;
    bool error = false;

    default_random_engine gen;
    uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    table1 = new uint64_t[rows];
    table2 = new uint64_t[rows];

    gen.seed(1);

    for (int i = 0 ; i < rows ; i++){
        table1[i] = distribution(gen);
        table2[i] = table1[i];
    }

    cout << "\n\033[1;33m-->\033[0m Testing sorting function ..." << endl;

    sort(&table1[0] , &table1[rows]);

    sortedTable = TableSortOnKey(&table2 , rows , 1 , 0 , 500);

    for (int i = 0; i < rows ; i++){
        if (table1[i] != sortedTable[i].key) {
            error = true;
            break;
        }
    }

    ofstream sort_file;
    sort_file.open("sort.txt");
    for (int i = 0 ; i < rows ; i++) {
        sort_file << table1[i] << endl;
    }
    sort_file.close();

    ofstream testing_file;
    testing_file.open("testing_sort.txt");
    for (int i = 0 ; i < rows ; i++) {
        testing_file << table1[i] << endl;;
    }
    testing_file.close();

    if (error)
        cout << "\nSorting test was \033[1;31mfailed\033[0m." << endl;
    else {
        cout << "\nSorting test was \033[1;32msuccessful\033[0m!" << endl << endl;
        cout << "Results of           \033[1;33msort()\033[0m function are in \033[1;34msort.txt\033[0m" << endl;
        cout << "Results of \033[1;33mTableSortOnKey()\033[0m function are in \033[1;34mtesting_sort.txt\033[0m" << endl << endl;
    }


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

    cout << "\n\033[1;33m-->\033[0m Testing MergeTables function and list results ..." << endl;

    uint32_t correct_results[7][2] = {{0,0},{1,0},{3,2},{4,2},{5,2},{6,4},{7,4}};
    uint32_t test_results[7][2]    = {};

    cout << "\n\033[1;33m    Results must be:\033[0m" << endl;
    cout << "    +-----------------+" << endl;
    cout << "    | RowID1 | RowID2 |" << endl;
    cout << "    +--------+--------+" << endl;
    cout << "    | 0      | 0      |" << endl;
    cout << "    | 1      | 0      |" << endl;
    cout << "    | 3      | 2      |" << endl;
    cout << "    | 4      | 2      |" << endl;
    cout << "    | 5      | 2      |" << endl;
    cout << "    | 6      | 4      |" << endl;
    cout << "    | 7      | 4      |" << endl;
    cout << "    +--------+--------+" << endl;

    MergeTables(list , sortedTable1 , 8 , sortedTable2 , 5);

    cout << "\n\033[1;33m    Test Results:\033[0m" << endl;

    Bucket<uint64_t>* bucket = list.GetFirst();

    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;

    mask32_left <<= 32;

    int i = 0;

    cout << "    +-----------------+" << endl;
    cout << "    | RowID1 | RowID2 |" << endl;
    cout << "    +--------+--------+" << endl;
    while (bucket != NULL) {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
            cout << "    | " << (((*bucket)[j] & mask32_left) >> 32) << "      | " << ((*bucket)[j] & mask32_right) << "      |" << endl;
            test_results[i][0] = ((*bucket)[j] & mask32_left) >> 32;
            test_results[i][1] = (*bucket)[j] & mask32_right;
            i++;
        }

        bucket = bucket->GetNextBucket();
    }
    cout << "    +--------+--------+" << endl;

    error = false;
    for (int i = 0; i < 7 && !error; i++) {
        for (int j = 0; j < 2; j++) {
            if (correct_results[i][j] != test_results[i][j]) {
                error = true;
                break;
            }
        }
    }

    if (error)
        cout << "\nMergeTables function and list results were incorrect, testing \033[1;31mfailed\033[0m." << endl << endl;
    else
        cout << "\nMergeTables function and list results were correct, testing \033[1;32msuccessful\033[0m!" << endl << endl;
}
