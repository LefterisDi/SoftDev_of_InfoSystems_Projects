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

    private:
        T&       operator =      (int const &) const;

    public:
        Bucket(const uint32_t & = 1);
        ~Bucket();

        Bucket*  GetNextBucket    (void) const;
        uint32_t GetBucketSize    (void) const;
        uint32_t GetBucketItems   (void) const;
        uint32_t GetRemainingSlots(void) const;
        T&       operator []      (int const &) const;
        // void     BucketPrint   (void)       const;

        bool   isFull(void);
        void   LinkNextBucket     (Bucket<T> *);
        int8_t BucketInsertEntry  (const T &);
        int8_t BucketDeleteEntry  (const uint32_t &);
        // int8_t DeleteEntry   (int const &);
        int8_t ClearBucket        (void);
};

template <typename T>
class List {
    Bucket<T>* head; 
    Bucket<T>* tail;
    Bucket<T>* last_used;

    uint32_t   last_used_pos;

    uint32_t   total_items;
    uint32_t   total_buckets;

    const uint32_t data_size;
    const uint32_t bucket_size;

    public:
        List(const uint32_t &, const uint32_t &);
        ~List();

        // void           ListPrint    (void) const;
        int8_t         ListInsert      (const T &);
        int8_t         DeleteBucket    (int const &);
        int8_t         DeleteLastBucket(void);
        Bucket<T>*     GetFirst        (void) const;
        Bucket<T>*     GetLast         (void) const;
        const uint32_t GetTotalItems   (void) const;
        const uint32_t GetTotalBuckets (void) const;
        // void           ResetCurrent    (void);
        Bucket<T>*     operator []     (int const &);
        List<T>&       operator +      (List<T> &);
        List<T>&       operator +=     (List<T> &);
};


#endif // __LIST_TEMPLATE__
