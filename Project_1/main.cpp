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
    if (!getopts(argc,argv,"da:p,db:p",args))
        return -1;

    // cout << args[0].optType.cp << endl;
    // cout << args[1].optType.cp << endl;

    // int tbl1_cols = 1000000 , tbl1_rows = 4;
    // int tbl2_cols = 10      , tbl2_rows = 2;

    uint32_t tbl1_cols, tbl1_rows;
    uint32_t tbl2_cols, tbl2_rows;

    uint64_t** table1 = ReadFile(args[0].optType.cp, tbl1_rows, tbl1_cols);
    uint64_t** table2 = ReadFile(args[1].optType.cp, tbl2_rows, tbl2_cols);

    // cout << "ROWS = " << tbl1_rows << endl;
    // cout << "COLS = " << tbl1_cols << endl;
    // for (int i = 0; i < tbl1_rows; i++) {
    //     for (int j = 0; j < tbl1_cols; j++) {
    //         cout << table1[i][j] << " , ";
    //     }
    //     cout << endl;
    // }
    // return 0;
    // uint64_t** table2;

    // uint32_t*  rowIDs1;
    // uint32_t*  rowIDs2;

    // 1048576 Bytes = 1 MB
    List<uint64_t> list(1048576 , sizeof(uint64_t));

    // default_random_engine gen;
    // uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    // table1 = new uint64_t*[tbl1_rows];
    // for(int i = 0; i < tbl1_rows; i++)
    //     table1[i] = new uint64_t[tbl1_cols];
    //
    // table2 = new uint64_t*[tbl2_rows];
    // for(int i = 0; i < tbl2_rows; i++)
    //     table2[i] = new uint64_t[tbl2_cols];

    // rowIDs1 = new uint32_t[tbl1_cols];
    // rowIDs2 = new uint32_t[tbl2_cols];

    // gen.seed ((unsigned int) time (NULL));
    // gen.seed(3);

    // for (int i = 0; i < tbl1_cols ; i++){
    //     rowIDs1[i] = i;
    // }
    //
    // for (int i = 0; i < tbl2_cols ; i++){
    //     rowIDs2[i] = i;
    // }

    // for (int i = 0 ; i < tbl1_rows ; i++){
    //     for (int j = 0 ; j < tbl1_cols ; j++){
    //         table1[i][j] = distribution(gen);
    //         // cout << table1[i][j] << " ";
    //     }
    //     // cout << endl;
    // }
    // // cout << endl;
    //
    // for (int i = 0 ; i < tbl2_rows ; i++){
    //     for (int j = 0 ; j < tbl2_cols ; j++){
    //         table2[i][j] = distribution(gen);
    //         // cout << table2[i][j] << " ";
    //     }
    //     // cout << endl;
    // }



    // TableSortOnKey(table1 , rowIDs1 , tbl1_cols , tbl1_rows , 0);
    // TableSortOnKey(table2 , rowIDs2 , tbl2_cols , tbl2_rows , 0);
    MergeTuple* sortedTable1 = TableSortOnKey(table1 , tbl1_cols , tbl1_rows , 0);
    MergeTuple* sortedTable2 = TableSortOnKey(table2 , tbl2_cols , tbl2_rows , 0);



    // for (int i = 0 ; i < tbl1_rows ; i++){
    //     for (int j = 0 ; j < tbl1_cols ; j++){
    //         cout << table1[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << endl;

    // for (int i = 0 ; i < tbl2_rows ; i++){
    //     for (int j = 0 ; j < tbl2_cols ; j++){
    //         cout << table2[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // cout << endl;

    // for (int i = 0 ; i < tbl1_cols ; i++){
    //     cout << rowIDs1[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0 ; i < tbl2_cols ; i++){
    //     cout << rowIDs2[i] << " ";
    // }
    // cout << endl;

    // cout << "DONE" << endl;

    // MergeTables(list, table1 , rowIDs1 , tbl1_cols , 0 , table2 , rowIDs2 , tbl2_cols , 0);
    MergeTables(list, sortedTable1 , tbl1_cols , 0 , sortedTable2 , tbl2_cols , 0);

    for(int i = 0; i < tbl1_rows; ++i)
        delete[] table1[i];
    delete[] table1;

    for(int i = 0; i < tbl2_rows; ++i)
        delete[] table2[i];
    delete[] table2;

    delete[] rowIDs1;
    delete[] rowIDs2;
}
