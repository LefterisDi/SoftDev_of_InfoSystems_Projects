#include "../utils/relationStructs.hpp"

void InitialStats(RelationTable*& relTable, uint32_t N)
{
    bool *distinctVal  = NULL;
    uint32_t boolTableSize = 0;
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
            distinctVal = new bool[N]();
        
            for (uint32_t j = 0; j < relTable->rows; j++)
                distinctVal[(relTable->table[i][j] - relTable->colStats->l_lower) % N] = true;
        
        } else {
        
            boolTableSize = relTable->colStats[i].u_upper - relTable->colStats[i].l_lower + 1;
            distinctVal = new bool[boolTableSize]();

            for (uint32_t j = 0; j < relTable->rows; j++)
                distinctVal[relTable->colStats[i].u_upper - relTable->table[i][j]] = true;
        }

        for (uint32_t k = 0 ; k < boolTableSize ; k++){
            if (distinctVal[k] == true){
                relTable->colStats[i].d_distinct++;
            }
        }

        if (distinctVal != NULL) {
            delete[] distinctVal;
            distinctVal = NULL;
        }
    }
}
