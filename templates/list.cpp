/* File: list.cpp */

#include <iostream>

#include "list.hpp"
#include "../relationStructs.h"

template <typename T>
Bucket<T>::Bucket(const uint32_t& items) : slots(items), remaining_slots(items), next_bucket(NULL)
{
    data = new T[items];
}

template <typename T>
Bucket<T>::~Bucket()
{
    delete[] data;
}

template <typename T>
Bucket<T>* Bucket<T>::GetNextBucket(void) const
{
    return next_bucket;
}

template <typename T>
uint32_t Bucket<T>::GetBucketSize(void) const
{
    return slots * sizeof(T);
}

template <typename T>
uint32_t Bucket<T>::GetBucketItems(void) const
{
    return slots - remaining_slots;
}

// template <typename T>
// void Bucket<T>::BucketPrint (void) const
// {
//     for (uint32_t i = 0 ; i < slots - remaining_slots ; i++) {
//         std::cout << "\t" << data[i] << std::endl;
//     }
// }


template <typename T>
T& Bucket<T>::operator [](int const& pos) const
{
    return data[pos];
}

template <typename T>
bool Bucket<T>::isFull(void)
{
    return (remaining_slots ? 0 : 1);
}

template <typename T>
void Bucket<T>::LinkNextBucket(Bucket<T>& new_bucket)
{
    next_bucket = &new_bucket;
}

template <typename T>
int8_t Bucket<T>::BucketInsert(const T& item)
{
    if (!remaining_slots)
        return -1;

    data[slots - remaining_slots] = item;
    remaining_slots--;
    return 0;
}

/* ----------------------------------------------------------------------------------------- */

// We save space by allocating the exact amount of elements
template <typename T>
List<T>::List(const uint32_t& bk_size, const uint32_t& dt_size) : bucket_size(bk_size), data_size(dt_size)
{
    if (data_size > bk_size) {
        std::cout << "Error: Data size is larger than bucket size" << std::endl;
        return;
    }

    head = new Bucket<T>(bucket_size / data_size);
    bucket_num++;
    tail = head;
    current = head;
    currentPos = 0;
}

template <typename T>
List<T>::~List()
{
    Bucket<T>* curr_bucket = head;

    while (curr_bucket != NULL) {
        head = head->GetNextBucket();
        delete curr_bucket;
        curr_bucket = head;
    }
}

template <typename T>
int8_t List<T>::ListInsert(const T& item)
{
    if (tail->isFull()) {
        Bucket<T>* tmp = new Bucket<T>(bucket_size / data_size);
        bucket_num++;
        tail->LinkNextBucket(*tmp);
        tail = tmp;
    }

    tail->BucketInsert(item);
    total_items++;
    return 0;
}

// template <typename T>
// void List<T>::ListPrint(void) const
// {
//     Bucket<T>* tmp = head;

//     int i = 0;

//     while (tmp != NULL) {
//         std::cout << "Bucket " << i << std::endl;
//         tmp->BucketPrint();
//         tmp = tmp->GetNextBucket();
//         i++;
//     }
// }

template <typename T>
Bucket<T>* List<T>::GetFirst(void) const
{
    return head;
}

template <typename T>
const uint32_t List<T>::GetTotalItems(void) const
{
    return total_items;
}

template <typename T>
const uint32_t List<T>::GetBucketNum (void) const
{
    return bucket_num;
}

template <typename T>
void List<T>::ResetCurrent (void) const
{
    this->current = this->GetFirst();
    currentPos = 0;
}

template <typename T>
Bucket<T>* List<T>::operator [](int const& pos) const
{

    if (pos < 0 || pos > this->bucket_num){
        return NULL;
    }

    if (currentPos == pos){
        return current;
    }
    else if (currentPos < pos){
        while (currentPos < pos){
            current = current->GetNextBucket();
            currentPos++;
        }
    }
    else {
        this->ResetCurrect();
        if (this->current == NULL){
            return NULL;
        }
        while (currentPos < pos){
            current = current->GetNextBucket();
            currentPos++;
        }
    }




    return bucket;
}

template class Bucket<uint64_t>;
template class List<uint64_t>;
template class List<ResStruct>;
template class Bucket<ResStruct>;
template class List<JoinPred>;
template class Bucket<JoinPred>;
template class List<CompPred>;
template class Bucket<CompPred>;

