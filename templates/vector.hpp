#ifndef __MVECTOR_HPP__
#define __MVECTOR_HPP__

#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <cstdint>


template <class T> 
class MiniVector {
    
    private:
        uint64_t vCapacity; // amount of available space
        uint64_t vSize;     // number of elements in the vector
        T* vArr;       // the dynamic array

        void RemoveManyTillIndex(uint64_t startIndex , uint64_t endIndex , uint64_t steps){

            // if (steps > startIndex + 1){
            //     std::cout << "DDDDDD" << std::endl;
            //     sleep(5);
            // }
            
            for (uint64_t i = startIndex+1 ; i < endIndex ; i++){
                    vArr[i-steps] = vArr[i];
            }

            

        }

        void MoveBy(uint64_t start , uint64_t end , uint64_t steps){

            if (start != end && end - start != 1){

                RemoveManyTillIndex(start , end , steps);
                
            }
        }

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

        void RemoveManyFromTo(MiniVector* remIndexes){

            if (remIndexes->GetTotalItems() == 0){
                return;
            }

            if (remIndexes->GetTotalItems() == 1){
                Remove( (*remIndexes)[0] );
                return;
            }

            uint64_t elemIndex1 = 0;
            uint64_t elemIndex2 = 0;
            uint64_t numOfSteps = 1;

            for (uint64_t i = 1; i < remIndexes->GetTotalItems() ;i++){

                elemIndex1 = (*remIndexes)[i-1];
                elemIndex2 = (*remIndexes)[i];
                
                MoveBy(elemIndex1 , elemIndex2 , numOfSteps);

                numOfSteps++;
                
            }

            MoveBy(elemIndex2 , GetTotalItems() , remIndexes->GetTotalItems());

            vCapacity += remIndexes->GetTotalItems();
            vSize -= remIndexes->GetTotalItems();
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

        void Reverse(){

            T *newArr;
            uint64_t i;
            uint64_t n = GetTotalItems();
            
            // allocate a new dynamic array with n elements
            newArr = new T[n];
            if (newArr == NULL){
                std::cout << "miniVector reserve(): memory allocation failure" << std::endl;
                exit(1);
            }
            
            // if copy is true, copy elements from the old list to the new list
            
            for(i = 0; i < vSize; i++)
                newArr[n-i-1] = vArr[i];
            
            // delete original dynamic array. if vArr is NULL, the vector was
            // originally empty and there is no memory to delete
            if (vArr != NULL)
                delete [] vArr;
            
            // set vArr to the value newArr. update vCapacity
            vArr = newArr;
            vCapacity = n;
        }

}; 

#endif