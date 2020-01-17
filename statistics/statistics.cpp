#include "../utils/relationStructs.hpp"
#include "../utils/utils.hpp"

#include <unordered_map>
#include <vector>
#include <cstring>

typedef unsigned int (*HashFunction)(uint64_t);

class BloomFilter {
    unsigned int numberOfCells;
    unsigned int numberOfFunctions;
    bool* cell;
    // std::vector<HashFunction> hashFunctions;
    unsigned int (*hash)(uint64_t);

public:

    BloomFilter(unsigned int numbCells, unsigned int (*hf)(uint64_t)) : numberOfCells(numbCells), hash(hf) {
        cell = (bool*)calloc(numbCells, sizeof(bool));
    }

    bool addElement(uint64_t str) {
        bool ret = true;
        // for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
        uint32_t pos = hash(str) % numberOfCells;

        ret &= cell[pos];
        cell[pos] = true;
        // }
        return ret;
    }

    // bool searchElement(uint64_t str) {
    //     bool strInSet = true;

    //     for (std::vector<HashFunction>::iterator iter = hashFunctions.begin(); iter != hashFunctions.end(); iter++) {
    //         if (cell[(*iter)(str) % numberOfCells] == false) {
    //             strInSet = false;
    //             break;
    //         }
    //     }

    //     return strInSet;
    // }

    ~BloomFilter() {
        free(cell);
        cell = NULL;
    }
};

unsigned int sdbm(uint64_t str) {
    unsigned int hash = 1254;

    hash = ((hash << 7) + (hash << 12) - (hash >> 5) ) + str;

    return hash;
}

void InitialStats(RelationTable*& relTable, uint32_t N)
{
    for (uint32_t i = 0; i < relTable->cols; i++) 
    {
        bool* hash = (bool*)calloc(relTable->rows * 4, sizeof(bool)); 

        uint32_t distincts = 0;

        for (int j = 0 ; j < relTable->rows ; j++) {
            
            uint32_t hpos = sdbm( relTable->table[i][j] )  % (relTable->rows * 4);

            if ( hash[hpos] ) {
                hash[hpos] = false;
                distincts--;
            }
            else {
                hash[hpos] = true;
                distincts++;
            }
        }
        
        std::cout << std::endl << relTable->rows << std::endl;
        std::cout << distincts << std::endl;
    }
}

void FilterEqualToValStats(RelationTable*& relTable , uint64_t rowNum , uint64_t val , uint32_t dataNum){

    uint64_t f_all_old = relTable->colStats[rowNum].f_all;

    relTable->colStats[rowNum].l_lower = val;
    relTable->colStats[rowNum].u_upper = val;

    if (dataNum != 1){
        relTable->colStats[rowNum].f_all = 0;
        relTable->colStats[rowNum].d_distinct = 0;
    }
    else {
        relTable->colStats[rowNum].f_all = relTable->colStats[rowNum].f_all / relTable->colStats[rowNum].d_distinct;
        relTable->colStats[rowNum].d_distinct = 1;
    }

    for (uint64_t i = 0 ; i < relTable->cols ; i++){
        if (i != rowNum){
            relTable->colStats[i].d_distinct = \
            relTable->colStats[i].d_distinct * (1 - raiseToPower( (1 -  (relTable->colStats[rowNum].f_all / f_all_old) ) , relTable->colStats[i].f_all / relTable->colStats[i].d_distinct) );
        
            relTable->colStats[i].f_all = relTable->colStats[rowNum].f_all;
        }
    }
}

