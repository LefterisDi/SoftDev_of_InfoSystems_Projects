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
#include "./utils/utils.hpp"

using namespace std;

int main(int argc , char* argv[])
{
    int size1x = 50, size1y = 4;
    int size2x = 3 , size2y = 2;

    uint32_t*  rowIDs1;
    uint32_t*  rowIDs2;
    uint64_t** table1;
    uint64_t** table2;

    default_random_engine gen;
    uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    table1 = new uint64_t*[size1y];
    for(int i = 0; i < size1y; i++)
        table1[i] = new uint64_t[size1x];

    table2 = new uint64_t*[size2y];
    for(int i = 0; i < size2y; i++)
        table2[i] = new uint64_t[size2x];

    rowIDs1 = new uint32_t[size1x];
    rowIDs2 = new uint32_t[size2x];

    gen.seed((unsigned int) time(NULL));
    // gen.seed(2);

    for (int i = 0 ; i < size1x ; i++)
        rowIDs1[i] = i+1;

    for (int i = 0 ; i < size2x ; i++)
        rowIDs2[i] = i+1;

    for (int i = 0 ; i < size1y ; i++) {
        for (int j = 0 ; j < size1x ; j++) {
            table1[i][j] = distribution(gen);
        }
    }

    for (int i = 0 ; i < size2y ; i++) {
        for (int j = 0 ; j < size2x ; j++) {
            table2[i][j] = distribution(gen);
        }
    }

    TableSortOnKey(table1 , rowIDs1 , size1x , size1y , 0);
    TableSortOnKey(table2 , rowIDs2 , size2x , size2y , 0);

    for(int i = 0; i < size1y; ++i)
        delete[] table1[i];
    delete[] table1;

    for(int i = 0; i < size2y; ++i)
        delete[] table2[i];
    delete[] table2;

    delete[] rowIDs1;
    delete[] rowIDs2;
}
