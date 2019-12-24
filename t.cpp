#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>

#include "./opts/getopts.hpp"
#include "./utils/predicates.hpp"
#include "./utils/utils.hpp"

using namespace std;

int main(void){
    MiniVector<uint64_t> vec;

    vec.PushBack(2);
    vec.PushBack(42);
    vec.PushBack(12);
    vec.PushBack(21);
    vec.PushBack(1);

    vec.Remove(1);
    vec.Remove(1);

    vec.PushBack(100);

    vec.Remove(0);
    vec.Remove(2);


    for (int i = 0 ; i < vec.GetTotalItems() ; i++){
        cout << vec[i] << endl;
    }

}
