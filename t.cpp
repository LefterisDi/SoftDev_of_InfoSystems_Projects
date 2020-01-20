#include <bitset>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>

#include "./templates/vector.hpp"
#include "./templates/hashmap.hpp"

using namespace std;

// A function to print all combination of a given length from the given array.
void GenSubSet(MiniVector< MiniVector<uint>* >& resVec , int reqLen, int start, int currLen, bool check[], uint16_t len) 
{
	// Return if the currrLen is more than the required length.
	if(currLen > reqLen)
	return;
	// If currLen is equal to required length then print the sequence.
	else if (currLen == reqLen) 
	{
		MiniVector<uint>* vec = new MiniVector<uint>;
		for (int i = 0; i < len; i++) 
		{
			if (check[i] == true) 
			{
				vec->PushBack(i);
			}
		}
		resVec.PushBack(vec);
		return;
	}
	// If start equals to len then return since no further element left.
	if (start == len) 
	{
		return;
	}
	// For every index we have two options.
	// First is, we select it, means put true in check[] and increment currLen and start.
	check[start] = true;
	GenSubSet(resVec , reqLen, start + 1, currLen + 1, check, len);
	// Second is, we don't select it, means put false in check[] and only start incremented.
	check[start] = false;
	GenSubSet(resVec , reqLen, start + 1, currLen, check, len);
} 

int main()
{
	int i , k;
	uint16_t n = 4;
	bool check[n];
	MiniVector< MiniVector<uint>* > minVec;

	// Take the input of the array.
	for(i = 0; i < n; i++)
	{
		check[i] = false;
	}

	uint32_t ui = ULONG_MAX;
	long double db = UINTMAX_MAX;
	cout << db << endl;
	cout << ui << endl;

	MyHashMap<uint , uint> hmap(5);

	try {
		hmap.get(12);
	}catch(const std::invalid_argument& arg){
		cout << "EXception " << arg.what() << endl;
	}


	// cout<<"\nEnter the length of the subsets required: ";
	// cin>>k;
 
	// cout<<"\nThe combination of length "<<k<<" for the given array set:\n";
	// GenSubSet( minVec , k, 0, 0, check, n);

	// for (int i = 0 ; i < minVec.GetTotalItems() ; i++){
	// 	for (int j = 0 ; j < minVec[i]->GetTotalItems() ; j++){
	// 		cout << (*minVec[i])[j] << " ";
	// 	}

	// 	delete minVec[i];
	// 	cout << endl;
	// }

 
	return 0;
}