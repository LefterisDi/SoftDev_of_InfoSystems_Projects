/* File: main.cpp */

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

int main(int argc , char* argv[])
{
    struct opt_types args[2];

    /*
     * +------------------------------------+
     * | args[0].optType.cp = <relA file>   |
     * | args[1].optType.cp = <relB file>   |
     * | args[1].optType.cp = <query file>  |
     * +------------------------------------+
     */
    if (!getopts(argc,argv,(char*)"da:p,db:p",args))
        return -1;

    uint32_t tbl1_cols, tbl1_rows;
    uint32_t tbl2_cols, tbl2_rows;

    uint64_t** table1 = ReadFile(args[0].optType.cp, tbl1_rows, tbl1_cols);
    uint64_t** table2 = ReadFile(args[1].optType.cp, tbl2_rows, tbl2_cols);

    List<uint64_t> list(1048576 , sizeof(uint64_t));

    MergeTuple* sortedTable1 = TableSortOnKey(table1 , tbl1_cols , tbl1_rows , 0 , 8192);
    MergeTuple* sortedTable2 = TableSortOnKey(table2 , tbl2_cols , tbl2_rows , 0 , 8192);

    MergeTables(list, sortedTable1 , tbl1_cols , sortedTable2 , tbl2_cols);

    Bucket<uint64_t>* bucket = list.GetFirst();

    ofstream results;
    results.open("results.txt");

    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
 
    mask32_left <<= 32;

    int i = 0;

    while (bucket != NULL) {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
            results << (((*bucket)[j] & mask32_left) >> 32) << " , " << ((*bucket)[j] & mask32_right) << endl;
            i++;
        }

        bucket = bucket->GetNextBucket();
    }
    results.close();

    for(uint32_t i = 0; i < tbl1_rows; ++i)
        delete[] table1[i];
    delete[] table1;

    for(uint32_t i = 0; i < tbl2_rows; ++i)
        delete[] table2[i];
    delete[] table2;

    delete[] sortedTable1;
    delete[] sortedTable2;
}
