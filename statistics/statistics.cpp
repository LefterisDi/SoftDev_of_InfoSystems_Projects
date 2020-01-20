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

    BloomFilter(unsigned int numbCells, unsigned int (*hf)(uint64_t)) : numberOfCells(numbCells), numberOfFunctions(0), hash(hf) {
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
    // hash = 1254;

    uint32_t hash_val = ((1254 << 7) + (1254 << 12) - (1254 >> 5) );

    uint32_t mod = relTable->rows * 4;

    bool* hash = (bool*)calloc(mod, sizeof(bool)); 
    
    uint64_t** tb = relTable->table;

    uint64_t rows = relTable->rows;
    uint64_t cols = relTable->cols;

    for (uint32_t i = 0; i < cols; i++) 
    {
        memset(hash,0,mod*sizeof(bool));

        uint32_t distincts = 0;

        for (int j = 0 ; j < rows ; j++) {
            
            uint32_t hpos = (hash_val + tb[i][j]) % mod;

            if ( hash[hpos] ) {
                hash[hpos] = false;
                distincts--;
            }
            else {
                hash[hpos] = true;
                distincts++;
            }
        }
        
        // std::cout << std::endl << relTable->rows << std::endl;
        // std::cout << distincts << std::endl;

    }
    
    free(hash);
}

void _InitialStats(RelationTable*& relTable, uint32_t N)
{
    bool *distinctVal  = NULL;
    uint32_t boolTableSize = 0;
    uint64_t criteria = 0;
    relTable->colStats = new Stats[relTable->cols];

    for (uint32_t i = 0; i < relTable->cols; i++) 
    {
        relTable->colStats[i].l_lower = relTable->table[i][0];
        relTable->colStats[i].u_upper = relTable->table[i][0];
        relTable->colStats[i].f_all   = relTable->rows;

        for (uint32_t j = 1 ; j < relTable->rows ; j++) {

            if (relTable->colStats[i].l_lower > relTable->table[i][j])
                relTable->colStats[i].l_lower = relTable->table[i][j];
            
            if (relTable->colStats[i].u_upper < relTable->table[i][j])
                relTable->colStats[i].u_upper = relTable->table[i][j];
        }

        if (relTable->colStats[i].u_upper - relTable->colStats[i].l_lower + 1 > N) {

            boolTableSize = N;
            relTable->colStats[i].N = N;
            distinctVal = new bool[N]();

        
            for (uint32_t j = 0; j < relTable->rows; j++)
                distinctVal[(relTable->table[i][j] - relTable->colStats->l_lower) % N] = true;
        
        } else {
        
            boolTableSize = relTable->colStats[i].u_upper - relTable->colStats[i].l_lower + 1;
            relTable->colStats[i].N = 0;
            distinctVal = new bool[boolTableSize]();

            for (uint32_t j = 0; j < relTable->rows; j++)
                distinctVal[relTable->colStats[i].u_upper - relTable->table[i][j]] = true;
        }

        for (uint32_t k = 0 ; k < boolTableSize ; k++){
            if (distinctVal[k] == true){
                relTable->colStats[i].d_distinct++;
            }
        }

        relTable->colStats[i].distinctArray = distinctVal;
        
        distinctVal = NULL;
        boolTableSize = 0;

        
        // std::cout << std::endl << relTable->rows << std::endl;
        // std::cout << relTable->colStats[i].d_distinct << std::endl;
    }
}

void FilterEqualToValStats(TableStats& relTableStats , uint64_t colNum , uint64_t rowNum , uint64_t val){

    uint64_t f_all_old = relTableStats.statsPerCol[rowNum].f_all;
    bool isDistinct = false;

    relTableStats.statsPerCol[rowNum].l_lower = val;
    relTableStats.statsPerCol[rowNum].u_upper = val;

    if (relTableStats.statsPerCol[rowNum].N != 0 && relTableStats.statsPerCol[rowNum].distinctArray[(val - relTableStats.statsPerCol->l_lower) % relTableStats.statsPerCol[rowNum].N] == true){
        isDistinct = true;
    }
    else if (relTableStats.statsPerCol[rowNum].N == 0 && relTableStats.statsPerCol[rowNum].distinctArray[relTableStats.statsPerCol[rowNum].u_upper - val] == true){
        isDistinct = true;
    }

    if (isDistinct == false){
        relTableStats.statsPerCol[rowNum].f_all = 0;
        relTableStats.statsPerCol[rowNum].d_distinct = 0;
    }
    else {
        relTableStats.statsPerCol[rowNum].f_all = relTableStats.statsPerCol[rowNum].f_all / relTableStats.statsPerCol[rowNum].d_distinct;
        relTableStats.statsPerCol[rowNum].d_distinct = 1;
    }

    for (uint64_t i = 0 ; i < colNum ; i++){
        if (i != rowNum){
            relTableStats.statsPerCol[i].d_distinct = \
            relTableStats.statsPerCol[i].d_distinct * (1 - raiseToPower( (1 -  (relTableStats.statsPerCol[rowNum].f_all / f_all_old) ) , relTableStats.statsPerCol[i].f_all / relTableStats.statsPerCol[i].d_distinct) );
        
            relTableStats.statsPerCol[i].f_all = relTableStats.statsPerCol[rowNum].f_all;
        }
    }
}

void FilterBetweenTwoValsStats(TableStats& relTableStats , uint64_t colNum ,uint64_t rowNum , uint64_t val1 , uint64_t val2){
    
    if (val1 > val2){
        uint64_t tmp = val1;
        val1 = val2;
        val2 = tmp;
    }

    uint64_t f_all_old = relTableStats.statsPerCol[rowNum].f_all;

    if (val1 < relTableStats.statsPerCol[rowNum].l_lower){
        val1 = relTableStats.statsPerCol[rowNum].l_lower;
    }
    if (val2 > relTableStats.statsPerCol[rowNum].u_upper){
        val2 = relTableStats.statsPerCol[rowNum].u_upper;
    }

    relTableStats.statsPerCol[rowNum].d_distinct = ( (val2 - val1) / (relTableStats.statsPerCol[rowNum].u_upper - relTableStats.statsPerCol[rowNum].l_lower) ) * relTableStats.statsPerCol[rowNum].d_distinct;
    relTableStats.statsPerCol[rowNum].f_all = ( (val2 - val1) / (relTableStats.statsPerCol[rowNum].u_upper - relTableStats.statsPerCol[rowNum].l_lower) ) * relTableStats.statsPerCol[rowNum].f_all;

    relTableStats.statsPerCol[rowNum].l_lower = val1;
    relTableStats.statsPerCol[rowNum].u_upper = val2;

    for (uint64_t i = 0; i < colNum ; i++){

        if (i != rowNum){
            relTableStats.statsPerCol[i].d_distinct = \
            relTableStats.statsPerCol[i].d_distinct * (1 - raiseToPower( (1 -  (relTableStats.statsPerCol[rowNum].f_all / f_all_old) ) , relTableStats.statsPerCol[i].f_all / relTableStats.statsPerCol[i].d_distinct) );
        
            relTableStats.statsPerCol[i].f_all = relTableStats.statsPerCol[rowNum].f_all;
        }
    }
}

void FilterBetweenTwoColumnsStats(TableStats& relTableStats , uint64_t colNum , uint64_t rowNum1 , uint64_t rowNum2){

    if (relTableStats.statsPerCol[rowNum1].l_lower > relTableStats.statsPerCol[rowNum2].l_lower){
        relTableStats.statsPerCol[rowNum2].l_lower = relTableStats.statsPerCol[rowNum1].l_lower;
    }
    else {
        relTableStats.statsPerCol[rowNum1].l_lower = relTableStats.statsPerCol[rowNum2].l_lower;
    }

    if (relTableStats.statsPerCol[rowNum1].u_upper < relTableStats.statsPerCol[rowNum2].u_upper){
        relTableStats.statsPerCol[rowNum2].u_upper = relTableStats.statsPerCol[rowNum1].u_upper;
    }
    else {
        relTableStats.statsPerCol[rowNum1].u_upper = relTableStats.statsPerCol[rowNum2].u_upper;
    }

    uint64_t f_all_old = relTableStats.statsPerCol[rowNum1].f_all;
    uint64_t n = relTableStats.statsPerCol[rowNum1].u_upper - relTableStats.statsPerCol[rowNum1].l_lower + 1;

    relTableStats.statsPerCol[rowNum1].f_all = f_all_old / n ;
    relTableStats.statsPerCol[rowNum2].f_all = relTableStats.statsPerCol[rowNum1].f_all;

    relTableStats.statsPerCol[rowNum1].d_distinct = \
            relTableStats.statsPerCol[rowNum1].d_distinct * (1 - raiseToPower( 1 -  (relTableStats.statsPerCol[rowNum1].f_all / f_all_old)  , f_all_old / relTableStats.statsPerCol[rowNum1].d_distinct) );
    
    relTableStats.statsPerCol[rowNum2].d_distinct = relTableStats.statsPerCol[rowNum1].d_distinct;

    for (uint64_t i = 0; i < colNum ; i++){

        if (i != rowNum1 && i != rowNum2){
            relTableStats.statsPerCol[i].d_distinct = \
            relTableStats.statsPerCol[i].d_distinct * (1 - raiseToPower( 1 -  (relTableStats.statsPerCol[rowNum1].f_all / f_all_old) , relTableStats.statsPerCol[i].f_all / relTableStats.statsPerCol[i].d_distinct) );
        
            relTableStats.statsPerCol[i].f_all = relTableStats.statsPerCol[rowNum1].f_all;
        }
    }

}

void SelfJoinStats(TableStats& relTableStats , uint64_t rowNum , uint64_t& cost){

    uint64_t n = relTableStats.statsPerCol[rowNum].u_upper - relTableStats.statsPerCol[rowNum].l_lower + 1;

    relTableStats.statsPerCol[rowNum].f_all = raiseToPower(relTableStats.statsPerCol[rowNum].f_all , 2) / n;

    for (uint64_t i = 0 ; i < relTableStats.cols ; i++) {
        if (i != rowNum) {
            relTableStats.statsPerCol[i].f_all = relTableStats.statsPerCol[rowNum].f_all; 
        }
    }

    cost = relTableStats.statsPerCol[rowNum].f_all;
}

int JoinStats(TableStats& relTableStats1 , TableStats& relTableStats2 , uint64_t rowNum1 , uint64_t rowNum2 , uint64_t& cost) 
{
    uint64_t new_lower;
    uint64_t new_upper;

    if (relTableStats1.statsPerCol[rowNum1].l_lower > relTableStats2.statsPerCol[rowNum2].l_lower) {
        new_lower = relTableStats1.statsPerCol[rowNum1].l_lower;
    
    } else {
        new_lower = relTableStats2.statsPerCol[rowNum2].l_lower;
    }

    if (relTableStats1.statsPerCol[rowNum1].u_upper < relTableStats2.statsPerCol[rowNum2].u_upper) {
        new_upper = relTableStats1.statsPerCol[rowNum1].u_upper;
    
    } else {
        new_upper = relTableStats2.statsPerCol[rowNum2].u_upper;
    }

    if (new_lower > new_upper) {
        return 1;
    } 

    FilterBetweenTwoValsStats(relTableStats1 , relTableStats1.cols , rowNum1 , new_lower , new_upper);
    FilterBetweenTwoValsStats(relTableStats2 , relTableStats2.cols , rowNum2 , new_lower , new_upper);

    relTableStats1.statsPerCol[rowNum1].l_lower = new_lower;
    relTableStats2.statsPerCol[rowNum2].l_lower = new_lower;

    relTableStats1.statsPerCol[rowNum1].u_upper = new_upper;
    relTableStats2.statsPerCol[rowNum2].u_upper = new_upper;

    uint64_t n = relTableStats1.statsPerCol[rowNum1].u_upper - relTableStats1.statsPerCol[rowNum1].l_lower + 1;
    uint64_t d_distinct_old1 = relTableStats1.statsPerCol[rowNum1].d_distinct;
    uint64_t d_distinct_old2 = relTableStats2.statsPerCol[rowNum2].d_distinct;

    relTableStats1.statsPerCol[rowNum1].f_all = (relTableStats1.statsPerCol[rowNum1].f_all * relTableStats2.statsPerCol[rowNum2].f_all) / n;
    relTableStats2.statsPerCol[rowNum2].f_all = relTableStats1.statsPerCol[rowNum1].f_all;

    relTableStats1.statsPerCol[rowNum1].d_distinct = (relTableStats1.statsPerCol[rowNum1].d_distinct * relTableStats2.statsPerCol[rowNum2].d_distinct) / n;
    relTableStats2.statsPerCol[rowNum2].d_distinct = relTableStats1.statsPerCol[rowNum1].d_distinct;

    for (uint64_t i = 0 ; i < relTableStats1.cols ; i++){
        
        if (i != rowNum1){
            relTableStats1.statsPerCol[i].f_all = relTableStats1.statsPerCol[rowNum1].f_all;
            relTableStats1.statsPerCol[i].d_distinct = \
            relTableStats1.statsPerCol[i].d_distinct * (1 - raiseToPower( 1 - (relTableStats1.statsPerCol[rowNum1].d_distinct / d_distinct_old1) , relTableStats1.statsPerCol[i].f_all / relTableStats1.statsPerCol[i].d_distinct) );
        }
    }

    for (uint64_t i = 0 ; i < relTableStats2.cols ; i++){
        
        if (i != rowNum2){
            relTableStats2.statsPerCol[i].f_all = relTableStats2.statsPerCol[rowNum2].f_all;
            relTableStats2.statsPerCol[i].d_distinct = \
            relTableStats2.statsPerCol[i].d_distinct * (1 - raiseToPower( 1 - (relTableStats2.statsPerCol[rowNum2].d_distinct / d_distinct_old2) , relTableStats2.statsPerCol[i].f_all / relTableStats2.statsPerCol[i].d_distinct) );
        }
    }

    cost = relTableStats1.statsPerCol[rowNum1].f_all;

    return 0;
}