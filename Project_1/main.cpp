#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <bitset>
#include <climits>
#include <random>

#include "./sortingAlg/quicksort.hpp"
#include "./searchingAlg/binarySearch.hpp"

using namespace std;

// void BitConversionFull(uint64_t* table , unsigned int** resTable, int size){


//     for (int i = 0 ; i < size ; i++){
//         uint64_t num = table[i];

//         for (int j = 0; j < 8 ; j++){
//             resTable[i][7-j] = num & ((1 << 8) - 1);
//             num = num >> 8;
//         }

//     }

// }

// uint64_t BitDeconvertion(unsigned int* givenTable){

//     uint64_t res = 0;
//     for (int j = 0 ; j < 8 ; j++){
//        res = res << 8;
//        res = res | givenTable[j];
//     }

//     return res;
// }

unsigned int BitConversion(uint64_t num, int key){//key must be between 0 and 7
    return ( ( (1 << 8) - 1) & (num >> ((7-key) * 8) ) );
}



void SimpleSortRec(uint64_t* table1 , uint64_t* table2 , int size , int key , int qsAfterNumOfEntries){

    unsigned int hist[UCHAR_MAX+1] = {};

        // cout << endl;
        // cout << "KEY " << key << " SIZE " << size << endl;
        // cout << "Previous table " << endl;
        // for (int i = 0 ; i < size ; i++){
        //     cout << BitConversion(table1[i] , 0) << endl;
        // }
        // cout << endl << "Current table " << endl;
        // for (int i = 0 ; i < size ; i++){
        //     cout << BitConversion(table1[i] , key) << endl;
        // }

    if (size < 2 || key == 8){
        return ;
    }

    if (size <= qsAfterNumOfEntries/*64KB or 8192 entries*/){
        quickSort(table1 , 0 , size - 1);

        for (int i = 0 ; i < size ; i++){
            table2[i] = table1[i];
        }
        return;
    }

    for (int i = 0 ; i < size ; i++){
        hist[BitConversion(table1[i] , key)]++;
    }


    int psumCount = 0;
    for (int i = 0 ; i <= UCHAR_MAX ; i++){
        if (hist[i] != 0){
            psumCount++;
        }
    }


    unsigned int psum[psumCount][2] = {};

    int ind = 0;
    int itCount = 0;
    for (int i = 0 ; i <= UCHAR_MAX ; i++){
        if (hist[i] != 0){
            psum[ind][0] = i;
            psum[ind][1] = itCount;
            itCount += hist[i];
            ind++;
        }
    }


    int table2Ind = 0;
    for(int i = 0 ; i < psumCount ; i++){

        for (int j = 0 ; j < size ; j++){

            if (psum[i][0] == BitConversion(table1[j] , key)){
                table2[table2Ind] = table1[j];
                table2Ind++;
            }

        }

    }

    for (int i = 0 ; i < size ; i++){
        table1[i] = table2[i];
    }


    int newKey = key + 1;
    for (int i = 0 ; i < psumCount ; i++){
        if (i < psumCount-1){
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , psum[i+1][1] - psum[i][1] , newKey , qsAfterNumOfEntries);
        }
        else if (i == psumCount-1){
            SimpleSortRec(&table2[psum[i][1]] , &table1[psum[i][1]] , ((unsigned int)size) - psum[i][1] , newKey , qsAfterNumOfEntries);            
        }
    }

}

void SwitchElements(uint64_t** tableMain , int sizeY , int firstElem , int secondElem){

    uint64_t temp;
    for (int k = 0 ; k < sizeY ; k++){
        temp = tableMain[k][firstElem];
        tableMain[k][firstElem] = tableMain[k][secondElem];
        tableMain[k][secondElem] = temp;
    }
}


void TableSortOnKey(uint64_t** tableMain , int sizeX , int sizeY , int key){

    uint64_t* table1;
    uint64_t* table2;
    int entriesQuicksort = 5000;

    table1 = new uint64_t[sizeX];
    table2 = new uint64_t[sizeX];


    for (int i = 0 ; i < sizeX ; i++){
        table1[i] = tableMain[key][i];
    }

    cout << "SORTING..." << endl;
    SimpleSortRec(table1 , table2 , sizeX , 0 , entriesQuicksort);

    cout << "SORTED" << endl;

    // for (int i = 0 ; i < sizeY ; i++){
    //     for (int j = 0 ; j < sizeX ; j++){
    //         cout << tableMain[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;


    cout << "SWITCHING..." << endl;
    for (int i = 0 ; i < sizeX ; i++){
        int index = binarySearch(table2 , 0 , sizeX-1 , tableMain[key][i]);
        if (index == -1){
            cout << "ELEMENT NOT FOUND ERROR!" << endl;
            exit(1);
        }
        cout << "FOUND " << i+1 << " " << tableMain[key][i] << endl;
        if (index != i){
            SwitchElements(tableMain , sizeY , i , index);
        }
    }
    cout << "SWITCHED" << endl;
    

    // for (int i = 0 ; i < sizeY ; i++){
    //     for (int j = 0 ; j < sizeX ; j++){
    //         cout << tableMain[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;


    delete[] table1;
    delete[] table2;

}

int main(int argc , char* argv[]){

    int size1x = 20 , size1y = 4;
    int size2x = 3 , size2y = 2;    
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

    // gen.seed ((unsigned int) time (NULL));
    gen.seed(2);

    for (int i = 0 ; i < size1y ; i++){
        for (int j = 0 ; j < size1x ; j++){
            table1[i][j] = distribution(gen);
            // cout << table1[i][j] << " ";
        }
        // cout << endl;
    }
    // cout << endl;

    for (int i = 0 ; i < size2y ; i++){
        for (int j = 0 ; j < size2x ; j++){
            table2[i][j] = distribution(gen);
            // cout << table2[i][j] << " ";
        }
        // cout << endl;
    }



    TableSortOnKey(table1 , size1x , size1y , 0);
    TableSortOnKey(table2 , size2x , size2y , 0);
    
    for(int i = 0; i < size1y; ++i)
        delete[] table1[i];
    delete[] table1;

    for(int i = 0; i < size2y; ++i)
        delete[] table2[i];
    delete[] table2;
}

