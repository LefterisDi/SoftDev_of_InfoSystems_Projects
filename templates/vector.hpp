#ifndef __MVECTOR_HPP__
#define __MVECTOR_HPP__

#include <iostream>
#include <stdexcept>

template <class T> 
class MiniVector {
    
    private:
        uint64_t vCapacity; // amount of available space
        uint64_t vSize;     // number of elements in the vector
        T* vArr;       // the dynamic array

    public:

        // template <typename T>
        MiniVector() : vCapacity(1) , vSize(0) {
            vArr = new T[vCapacity]();
        }
        
        ~MiniVector(){
            delete[] vArr;
        }

        T& operator [] (uint64_t i){
            return vArr[i];
        }

        const uint64_t GetTotalItems(){
            return vSize;
        }
        
        // template <typename T>
        void Remove(uint64_t index){

            for (uint64_t i = index+1 ; i < vSize ; i++){
                vArr[i-1] = vArr[i];
            }

            vCapacity++;
            vSize--;
        }

        // template <typename T>
        void Reserve(uint64_t n, bool copy){
            T *newArr;
            uint64_t i;
            
            // allocate a new dynamic array with n elements
            newArr = new T[n];
            if (newArr == NULL){
                std::cout << "miniVector reserve(): memory allocation failure" << std::endl;
                exit(1);
            }
            
            // if copy is true, copy elements from the old list to the new list
            if (copy)
                for(i = 0; i < vSize; i++)
                newArr[i] = vArr[i];
            
            // delete original dynamic array. if vArr is NULL, the vector was
            // originally empty and there is no memory to delete
            if (vArr != NULL)
                delete [] vArr;
            
            // set vArr to the value newArr. update vCapacity
            vArr = newArr;
            vCapacity = n;
        }

        // template <typename T>
        void PushBack(const T& item){
            // if space is full, allocate more capacity
            if (vSize == vCapacity){
                if (vCapacity == 0)
                    // if capacity is 0, set capacity to 1.
                    // set copy to false because there are
                    // no existing elements
                    Reserve(1,false);
                else
                    // double the capacity
                    Reserve(2 * vCapacity, true);
            }
            
            // add item to the list, update vSize
            vArr[vSize] = item;
            vSize++;
        }

}; 

#endif