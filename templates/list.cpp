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
    return (slots - remaining_slots);
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
    total_buckets++;
    tail      = head;
    last_used = head;
    last_used_pos = 0;
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
        total_buckets++;
        tail->LinkNextBucket(*tmp);
        tail = tmp;
    }

    tail->BucketInsert(item);
    total_items++;
    return 0;
}

template <typename T>
int8_t List<T>::DeleteBucket(int const& pos)
{
    Bucket<T>* last_used_backup     = last_used;
    uint32_t   last_used_pos_backup = last_used_pos;

    Bucket<T>* prev_bckt = (*this)[pos-1];

    // We keep these values in case the "last_used" bucket will be deleted.
    // We save time by holding this information instead of searching the
    // list from the beginning after the deletion, because we just did it.
    Bucket<T>* last_used_new     = last_used;
    uint32_t   last_used_pos_new = last_used_pos;

    Bucket<T>* del_bckt  = (*this)[pos];

    if (!pos)
        prev_bckt = head;

    if (total_buckets > 1) {
        if (!pos)
            head = head->GetNextBucket();

        else if (del_bckt == tail)
            tail = prev_bckt;

        del_bckt->LinkNextBucket( *(del_bckt->GetNextBucket()->GetNextBucket()) );
        total_buckets--;

    } else {
        head->LinkNextBucket(*del_bckt->GetNextBucket());
    }

    total_items -= del_bckt->GetBucketItems();

    delete del_bckt;

    // If "last_used" bucket is about to be deleted,
    // we have to update the pointers
    if (last_used == del_bckt) {
        // We update "last_used" pointer to point to the new head
        if (!pos) {
            last_used     = head;
            last_used_pos = 0;

        // We set previous bucket as "last_used"
        } else {
            last_used     = last_used_new;
            last_used_pos = last_used_pos_new;
        }

    // Otherwise, we reset the values to their previous values
    } else {
        last_used     = last_used_backup;
        last_used_pos = last_used_pos_backup;
    }

    return 1;
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
Bucket<T>* List<T>::GetLast(void) const
{
    return tail;
}

template <typename T>
const uint32_t List<T>::GetTotalItems(void) const
{
    return total_items;
}

template <typename T>
const uint32_t List<T>::GetTotalBuckets(void) const
{
    return total_buckets;
}

// template <typename T>
// void List<T>::ResetCurrent (void)
// {
//     last_used     = head;
//     last_used_pos = 0;
// }

template <typename T>
Bucket<T>* List<T>::operator [](int const& pos)
{
    if (pos < 0 || pos > total_buckets)
        return NULL;

    if (last_used_pos == pos)
        return last_used;

    if (last_used_pos > pos) {
        last_used     = head;
        last_used_pos = 0;
        // this->ResetCurrent();

        if (last_used == NULL)
            return NULL;
    }

    while (last_used_pos < pos) {
        last_used = last_used->GetNextBucket();
        last_used_pos++;
    }

    return last_used;
}

template class Bucket<CompPred>;
template class Bucket<FullResList>;
template class Bucket<JoinPred>;
template class Bucket<ResStruct>;
template class Bucket<uint64_t>;
template class List<CompPred>;
template class List<FullResList>;
template class List<JoinPred>;
template class List<ResStruct>;
template class List<uint64_t>;
