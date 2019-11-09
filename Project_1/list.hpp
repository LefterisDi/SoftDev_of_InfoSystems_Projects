
#include <iostream>

template <typename T>
class Bucket {
    T*        data;
    uint32_t  slots;
    uint32_t  remaining_slots;
    uint32_t* next_bucket;

    public:
        Bucket(const uint32_t& = 1);
        ~Bucket();

        const uint32_t GetBucketSize(void)       const;
        const        T BucketSearch (const T&)   const;
        const       T& operator []  (int const&) const;

        bool   isFull(void);
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
        // T ListSearch(const T&) const;

};
