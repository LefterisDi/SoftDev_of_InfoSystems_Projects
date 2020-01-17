#include "../utils/relationStructs.hpp"
#include "../utils/utils.hpp"

void InitialStats(RelationTable*& relTable, uint32_t N)
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
    }
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
    }
}

void FilterEqualToValStats(RelationTable*& relTable , uint64_t rowNum , uint64_t val){

    uint64_t f_all_old = relTable->colStats[rowNum].f_all;
    bool isDistinct = false;

    relTable->colStats[rowNum].l_lower = val;
    relTable->colStats[rowNum].u_upper = val;

    if (relTable->colStats[rowNum].N != 0 && relTable->colStats[rowNum].distinctArray[(val - relTable->colStats->l_lower) % relTable->colStats[rowNum].N] == true){
        isDistinct = true;
    }
    else if (relTable->colStats[rowNum].N == 0 && relTable->colStats[rowNum].distinctArray[relTable->colStats[rowNum].u_upper - val] == true){
        isDistinct = true;
    }

    if (isDistinct == false){
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

void FilterBetweenTwoValsStats(RelationTable*& relTable , uint64_t rowNum , uint64_t val1 , uint64_t val2){
    
    if (val1 > val2){
        uint64_t tmp = val1;
        val1 = val2;
        val2 = tmp;
    }

    uint64_t f_all_old = relTable->colStats[rowNum].f_all;

    if (val1 < relTable->colStats[rowNum].l_lower){
        val1 = relTable->colStats[rowNum].l_lower;
    }
    if (val2 > relTable->colStats[rowNum].u_upper){
        val2 = relTable->colStats[rowNum].u_upper;
    }

    relTable->colStats[rowNum].d_distinct = ( (val2 - val1) / (relTable->colStats[rowNum].u_upper - relTable->colStats[rowNum].l_lower) ) * relTable->colStats[rowNum].d_distinct;
    relTable->colStats[rowNum].f_all = ( (val2 - val1) / (relTable->colStats[rowNum].u_upper - relTable->colStats[rowNum].l_lower) ) * relTable->colStats[rowNum].f_all;

    relTable->colStats[rowNum].l_lower = val1;
    relTable->colStats[rowNum].u_upper = val2;

    for (uint64_t i = 0; i < relTable->cols ; i++){

        if (i != rowNum){
            relTable->colStats[i].d_distinct = \
            relTable->colStats[i].d_distinct * (1 - raiseToPower( (1 -  (relTable->colStats[rowNum].f_all / f_all_old) ) , relTable->colStats[i].f_all / relTable->colStats[i].d_distinct) );
        
            relTable->colStats[i].f_all = relTable->colStats[rowNum].f_all;
        }
    }
}

void FilterBetweenTwoColumnsStats(RelationTable*& relTable , uint64_t rowNum1 , uint64_t rowNum2){

    if (relTable->colStats[rowNum1].l_lower > relTable->colStats[rowNum2].l_lower){
        relTable->colStats[rowNum2].l_lower = relTable->colStats[rowNum1].l_lower;
    }
    else {
        relTable->colStats[rowNum1].l_lower = relTable->colStats[rowNum2].l_lower;
    }

    if (relTable->colStats[rowNum1].u_upper < relTable->colStats[rowNum2].u_upper){
        relTable->colStats[rowNum2].u_upper = relTable->colStats[rowNum1].u_upper;
    }
    else {
        relTable->colStats[rowNum1].u_upper = relTable->colStats[rowNum2].u_upper;
    }

    uint64_t f_all_old = relTable->colStats[rowNum1].f_all;
    uint64_t n = relTable->colStats[rowNum1].u_upper - relTable->colStats[rowNum1].l_lower + 1;

    relTable->colStats[rowNum1].f_all = f_all_old / n ;
    relTable->colStats[rowNum2].f_all = relTable->colStats[rowNum1].f_all;

    relTable->colStats[rowNum1].d_distinct = \
            relTable->colStats[rowNum1].d_distinct * (1 - raiseToPower( (1 -  (relTable->colStats[rowNum1].f_all / f_all_old) ) , f_all_old / relTable->colStats[rowNum1].d_distinct) );
    
    relTable->colStats[rowNum2].d_distinct = relTable->colStats[rowNum1].d_distinct;

    for (uint64_t i = 0; i < relTable->cols ; i++){

        if (i != rowNum1 && i != rowNum2){
            relTable->colStats[i].d_distinct = \
            relTable->colStats[i].d_distinct * (1 - raiseToPower( (1 -  (relTable->colStats[rowNum1].f_all / f_all_old) ) , relTable->colStats[i].f_all / relTable->colStats[i].d_distinct) );
        
            relTable->colStats[i].f_all = relTable->colStats[rowNum1].f_all;
        }
    }

}

void SelfJoinStats(RelationTable*& relTable , uint64_t rowNum){

    uint64_t n = relTable->colStats[rowNum].u_upper - relTable->colStats[rowNum].l_lower + 1;

    relTable->colStats[rowNum].f_all = raiseToPower(relTable->colStats[rowNum].f_all , 2) / n;

    for (uint64_t i = 0 ; i < relTable->cols ; i++){

        if (i != rowNum){
            relTable->colStats[i].f_all = relTable->colStats[rowNum].f_all; 
        }
    }
}