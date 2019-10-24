#include <iostream>
#include <cstdlib> 
#include <ctime> 
#include <bitset>
#include <climits>

using namespace std;

void TestSort(uint64_t* row , int size){

    unsigned int table1[size][8] , table2[size][8];
    unsigned int hist[UCHAR_MAX+1] = {};

    for (int i = 0 ; i < size ; i++){
        uint64_t num = row[i];
        //  std::bitset<64> r(num);
        //  cout << "Starting" << endl << r << endl;
        for (int j = 0; j < 8 ; j++){
            table1[i][7-j] = num & ((1 << 8) - 1);
            num = num >> 8;
        }
        // for (int j = 0; j < 8 ; j++){
        //     std::bitset<8> x(table1[i][j]);
        //     cout << x;
        // }

        // cout << endl;
    }

    for (int i = 0 ; i < size ; i++){
        cout << table1[i][0] << endl;
        hist[table1[i][0]]++;
    }

    for (int i = 0 ; i <= UCHAR_MAX ; i++){
        if (hist[i] != 0){
            cout << i << " " << hist[i] << endl;
        }
    }
}

int main(int argc , char* argv[]){

    uint64_t table[5];

    // srand((int) time(0));
    srand(2);

    for (int i = 0 ; i < 5 ; i++){
        table[i] = rand();
        cout << table[i] << endl;
    }

    TestSort(table , 5);
    

}

