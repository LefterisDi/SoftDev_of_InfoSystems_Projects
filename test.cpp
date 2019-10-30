#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <bitset>
#include <climits>
#include <random>

using namespace std;

// void TestSort(uint64_t* row , int size){

//     unsigned int table1[size][8] , table2[size][8];
//     unsigned int hist[UCHAR_MAX+1] = {};

//     for (int i = 0 ; i < size ; i++){
//         uint64_t num = row[i];
//          std::bitset<64> r(num);
//          cout << "Starting" << endl << r << endl;
//         for (int j = 0; j < 8 ; j++){
//             table1[i][7-j] = num & ((1 << 8) - 1);
//             num = num >> 8;
//         }
//         // for (int j = 0; j < 8 ; j++){
//         //     std::bitset<8> x(table1[i][j]);
//         //     cout << x;
//         // }

//         // cout << endl;
//     }
    

//     for (int i = 0 ; i < size ; i++){
//         // cout << table1[i][0] << endl;//prints first field
//         hist[table1[i][0]]++;
//     }

//     int psumCount = 0;
//     for (int i = 0 ; i <= UCHAR_MAX ; i++){
//         if (hist[i] != 0){
//             psumCount++;
//         }
//     }

//     // cout << psumCount << endl;

//     unsigned int psum[psumCount][2] = {};

//     int ind = 0;
//     int itCount = 0;
//     for (int i = 0 ; i <= UCHAR_MAX ; i++){
//         if (hist[i] != 0){
//             psum[ind][0] = i;
//             psum[ind][1] = itCount;
//             itCount += hist[i];
//             ind++;

//             // cout << i << " " << hist[i] << endl;
//         }
//     }

//     // cout << endl;
//     // for (int i = 0 ; i < psumCount ; i++){
//     //     cout << psum[i][0] << " " << psum[i][1] << endl;
//     // }

//     int table2Ind = 0;
//     for(int i = 0 ; i < psumCount ; i++){
//         for (int j = 0 ; j < size ; j++){
//             if (psum[i][0] == table1[j][0]){
//                 for (int k = 0 ; k < 8 ; k++){
//                     table2[table2Ind][k] = table1[j][k];
//                 }
//                 table2Ind++;
//             }
//         }
//     }

//     for (int i = 0 ; i < size ; i++){
//         cout << table1[i][0] << endl;
//     }
//     cout << endl;

//     for (int i = 0 ; i < size ; i++){
//         cout << table2[i][0] << endl;
//     }

// }

void BitConversion(uint64_t* table , unsigned int** resTable, int size){


    for (int i = 0 ; i < size ; i++){
        uint64_t num = table[i];

        for (int j = 0; j < 8 ; j++){
            resTable[i][7-j] = num & ((1 << 8) - 1);
            num = num >> 8;
        }

        for (int j = 0; j < 8 ; j++){
            std::bitset<8> x(resTable[i][j]);
            cout << x;
        }

        cout << endl;
    }


}

void SimpleSortRec(unsigned int** table1 , unsigned int** table2 , int size){

    unsigned int hist[UCHAR_MAX+1] = {};

    for (int i = 0 ; i < size ; i++){
        // cout << table1[i][0] << endl;//prints first field
        hist[table1[i][0]]++;
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

            // cout << i << " " << hist[i] << endl;
        }
    }

    int table2Ind = 0;
    for(int i = 0 ; i < psumCount ; i++){
        for (int j = 0 ; j < size ; j++){
            if (psum[i][0] == table1[j][0]){
                for (int k = 0 ; k < 8 ; k++){
                    table2[table2Ind][k] = table1[j][k];
                }
                table2Ind++;
            }
        }
    }

    for (int i = 0 ; i < size ; i++){
        cout << table1[i][0] << endl;
    }
    cout << endl;

    for (int i = 0 ; i < size ; i++){
        cout << table2[i][0] << endl;
    }


    for (int i = 0 ; i < psumCount ; i++){

    }

}


void TestSort(uint64_t** tableMain , int sizeX , int sizeY , int key){

    unsigned int** table1;
    unsigned int** table2;
    unsigned int hist[UCHAR_MAX+1] = {};

    table1 = new unsigned int*[sizeX];
    for(int i = 0; i < sizeX; i++)
        table1[i] = new unsigned int[8];

    table2 = new unsigned int*[sizeX];
    for(int i = 0; i < sizeX; i++)
        table2[i] = new unsigned int[8];

    BitConversion(tableMain[key] , table1 , sizeX);
    SimpleSortRec(table1 , table2 , sizeX);

    // for (int i = 0 ; i < sizeX ; i++){
    //     // uint64_t num = tableMain[0][i];
    //     //  std::bitset<64> r(num);
    //     //  cout << "Starting" << endl << r << endl;
    //     // for (int j = 0; j < 8 ; j++){
    //     //     table1[i][7-j] = num & ((1 << 8) - 1);
    //     //     num = num >> 8;
    //     // }
    //     cout << "TWO" << endl;
    //     for (int j = 0; j < 8 ; j++){
    //         std::bitset<8> x(table1[i][j]);
    //         cout << x;
    //     }

    //     cout << endl;
    // }
    

    // for (int i = 0 ; i < sizeX ; i++){
    //     // cout << table1[i][0] << endl;//prints first field
    //     hist[table1[i][0]]++;
    // }

    // int psumCount = 0;
    // for (int i = 0 ; i <= UCHAR_MAX ; i++){
    //     if (hist[i] != 0){
    //         psumCount++;
    //     }
    // }

    // // cout << psumCount << endl;

    // unsigned int psum[psumCount][2] = {};

    // int ind = 0;
    // int itCount = 0;
    // for (int i = 0 ; i <= UCHAR_MAX ; i++){
    //     if (hist[i] != 0){
    //         psum[ind][0] = i;
    //         psum[ind][1] = itCount;
    //         itCount += hist[i];
    //         ind++;

    //         // cout << i << " " << hist[i] << endl;
    //     }
    // }

    // // cout << endl;
    // // for (int i = 0 ; i < psumCount ; i++){
    // //     cout << psum[i][0] << " " << psum[i][1] << endl;
    // // }

    // int table2Ind = 0;
    // for(int i = 0 ; i < psumCount ; i++){
    //     for (int j = 0 ; j < sizeX ; j++){
    //         if (psum[i][0] == table1[j][0]){
    //             for (int k = 0 ; k < 8 ; k++){
    //                 table2[table2Ind][k] = table1[j][k];
    //             }
    //             table2Ind++;
    //         }
    //     }
    // }

    // for (int i = 0 ; i < sizeX ; i++){
    //     cout << table1[i][0] << endl;
    // }
    // cout << endl;

    // for (int i = 0 ; i < sizeX ; i++){
    //     cout << table2[i][0] << endl;
    // }

    

}

int main(int argc , char* argv[]){

    int size1x = 3 , size1y = 4;
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
            cout << table1[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    for (int i = 0 ; i < size2y ; i++){
        for (int j = 0 ; j < size2x ; j++){
            table2[i][j] = distribution(gen);
            cout << table2[i][j] << " ";
        }
        cout << endl;
    }

    cout << "DONE" << endl;

    TestSort(table1 , size1x , size1y , 0);
    
    for(int i = 0; i < size1y; ++i)
        delete[] table1[i];
    delete[] table1;

    for(int i = 0; i < size2y; ++i)
        delete[] table2[i];
    delete[] table2;
}

