/* File: list.hpp */

#ifndef __LIST_TEMPLATE__
#define __LIST_TEMPLATE__

#include <iostream>

template <typename T>
class Bucket {
    T*       data;
    uint32_t slots;
    uint32_t remaining_slots;
    Bucket*  next_bucket;

    public:
        Bucket(const uint32_t& = 1);
        ~Bucket();

        Bucket*  GetNextBucket(void)       const;
        uint32_t GetBucketSize(void)       const;
        void     BucketPrint  (void)       const;
        T&       operator []  (int const&) const;

        bool   isFull(void);
        void   LinkNextBucket(Bucket<T>&);
        int8_t BucketInsert(const T&);
};

template <typename T>
class List {
    Bucket<T>* head;
    Bucket<T>* tail;

    uint32_t   data_size;
    uint32_t   bucket_size;
    uint32_t   bucket_items;

    public:
        List(const uint32_t&, const uint32_t&);
        ~List();

        int8_t ListInsert(const T&);
        void   ListPrint (void) const;
};

#endif // __LIST_TEMPLATE__
