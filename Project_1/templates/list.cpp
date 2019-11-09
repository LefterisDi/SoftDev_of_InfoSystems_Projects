/* File: list.cpp */

#include <iostream>

#include "list.hpp"

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
void Bucket<T>::BucketPrint (void) const
{
    for (uint32_t i = 0 ; i < slots - remaining_slots ; i++) {
        std::cout << "\t" << data[i] << std::endl;
    }
}

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
    tail = head;
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
        tail->LinkNextBucket(*tmp);
        tail = tmp;
    }

    tail->BucketInsert(item);
    return 0;
}

template <typename T>
void List<T>::ListPrint(void) const
{
    Bucket<T>* tmp = head;

    int i = 0;

    while (tmp != NULL) {
        std::cout << "Bucket " << i << std::endl;
        tmp->BucketPrint();
        tmp = tmp->GetNextBucket();
        i++;
    }
}

// template class Bucket<int>;
template class List<uint64_t>;


//
// class Bucket {
//     uint32_t  bk_size;
//     uint32_t* next_bucket;
//
//     public:
//         Bucket(const uint32_t& bucket_size = 1024) : bk_size(bucket_size), next_bucket(NULL) {}
//
//         const uint32_t GetBucketSize(void) const { return this->bk_size; }
// };

// template<class V>
// struct Bucket {
//   V val;
//   Bucket(const V& v) : val(v) {}
// };
//
// template<class T> struct myHash {
// };
//
// template<> struct myHash<string> {
//   static size_t hashFunction(const string& s) {
//     return hash<string>()(s);
//   }
// };
//
// template<> struct myHash<int> {
//   static size_t hashFunction(const int m) {
//     return hash<int>()(m);
//   }
// };
//
// template<class V, class HashGenerator = myHash<K> >
// class List {
//     public:
//         vector<vector<Bucket<K, V> > > table;
//
//         HashTable(int size) {
//             for(int i = 0; i < size; i++) {
//                 vector<Bucket<K, V> > v;
//                 table.push_back(v);
//             }
//         }
//         ~HashTable() {}
//         void set(const K &k, const V &v) {
//             Bucket<K, V> b(k, v);
//             for(int i = 0; i < table[hash(k)].size(); i++)
//             if(table[hash(k)][i].key == k) {
//                 table[hash(k)][i] = b;
//                 return;
//             }
//             table[hash(k)].push_back(b);
//         }
//         V get(const K &k) {
//             for(int i = 0; i < table[hash(k)].size(); i++)
//             if(table[hash(k)][i].key == k)
//             return table[hash(k)][i].val;
//         }
//         bool exist(const K &k) {
//             for(int i = 0; i < table[hash(k)].size(); i++)
//             if(table[hash(k)][i].key == k)
//             return true;
//             return false;
//         }
//         size_t hash(const K &k) {
//             return HashGenerator::hashFunction(k) % table.size();
//         }
// };
