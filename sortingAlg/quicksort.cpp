/* File: binarySearch.cpp */

#include <iostream>

#include "../utils/utils.hpp"

void swap(MergeTuple* a, MergeTuple* b)
{
    MergeTuple t = *a;
    *a = *b;
    *b = t;
}

/******************************************************
* This function takes last element as pivot, places   *
* the pivot element at its correct position in sorted *
* array, and places all smaller (smaller than pivot)  *
* to left of pivot and all greater elements to right  *
* of pivot                                            *
******************************************************/
int partition(MergeTuple* arr, int low, int high)
{
    uint64_t pivot = arr[high].key; // pivot
    int i = (low - 1);              // Index of smaller element

    for (int j = low ; j <= high - 1 ; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j].key < pivot) {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**********************************************
* The main function that implements QuickSort *
* arr[] --> Array to be sorted,               *
* low   --> Starting index,                   *
* high  --> Ending index                      *
**********************************************/
void quickSort(MergeTuple* arr, int low, int high)
{
    if (low < high)
    {
        // pi is partitioning index, arr[p] is now
        // at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}



// A utility function to swap two elements  
void swap(uint32_t* a, uint32_t* b)  
{  
    uint32_t t = *a;  
    *a = *b;  
    *b = t;  
}  
  
/* This function takes last element as pivot, places  
the pivot element at its correct position in sorted  
array, and places all smaller (smaller than pivot)  
to left of pivot and all greater elements to right  
of pivot */
int partition (uint32_t arr[], int low, int high)  
{  
    uint32_t pivot = arr[high]; // pivot  
    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {  
        // If current element is smaller than the pivot  
        if (arr[j] < pivot)  
        {  
            i++; // increment index of smaller element  
            swap(&arr[i], &arr[j]);  
        }  
    }  
    swap(&arr[i + 1], &arr[high]);  
    return (i + 1);  
}  
  
/* The main function that implements QuickSort  
arr[] --> Array to be sorted,  
low --> Starting index,  
high --> Ending index */
void quickSort(uint32_t arr[], int low, int high)  
{  
    if (low < high)  
    {  
        /* pi is partitioning index, arr[p] is now  
        at right place */
        int pi = partition(arr, low, high);  
  
        // Separately sort elements before  
        // partition and after partition  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}  