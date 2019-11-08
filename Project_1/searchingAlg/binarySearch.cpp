#include <iostream>
#include <cstdlib> 

#include "./binarySearch.hpp"

using namespace std;


int binarySearch(uint64_t arr[],bool* positionCTable , int l, int r, uint64_t x) { 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == x && positionCTable[mid] == false){
            return mid; 
        }
  
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid] > x){
            return binarySearch(arr,positionCTable , l, mid - 1, x); 
        }
        else if (arr[mid] < x){
            return binarySearch(arr,positionCTable , mid + 1, r, x); 
        }
        else if (arr[mid] == x && positionCTable[mid] == true){
            int ind = binarySearch(arr,positionCTable, l, mid - 1, x);
            if (ind == -1){
                return binarySearch(arr,positionCTable , mid + 1, r, x);
            }
            else {
                return ind;
            }
        }
  
        // Else the element can only be present 
        // in right subarray 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
}  
