/* File: utils.cpp */

#include <iostream>
#include <fstream>
#include <string.h>

#include "../templates/list.hpp"
#include "utils.hpp"

uint32_t BitConversion(uint64_t num, uint32_t key)
{//key must be between 0 and 7
    return ( ( (1 << 8) - 1) & (num >> ((7-key) * 8) ) );
}

void SwitchElements(uint64_t** tableMain , uint32_t sizeY , uint32_t firstElem , uint32_t secondElem)
{
    uint64_t temp;

    for (uint32_t k = 0 ; k < sizeY ; k++) {
        temp                     = tableMain[k][firstElem];
        tableMain[k][firstElem]  = tableMain[k][secondElem];
        tableMain[k][secondElem] = temp;
    }
}

// void MergeTables(List<uint64_t>& list, uint64_t** table1, uint32_t* rowIDs1, uint32_t size1X, uint32_t key1, uint64_t** table2, uint32_t* rowIDs2, uint32_t size2X, uint32_t key2)
void MergeTables(List<uint64_t>& list, MergeTuple* sortedTable1, uint32_t size1X, MergeTuple* sortedTable2, uint32_t size2X)
{
    uint32_t tabelA_index = 0;
    uint32_t tableB_index = 0;
    uint32_t tableB_pin   = 0;

    while (tabelA_index < size1X)
    {
        if (sortedTable1[tabelA_index].key == sortedTable2[tableB_index].key) {
            uint64_t list_entry = sortedTable1[tabelA_index].rowID;
            list_entry <<= 32;
            list_entry |= sortedTable2[tableB_index].rowID;

            list.ListInsert(list_entry);

            // std::cout << sortedTable1[tabelA_index].rowID <<  " " << sortedTable2[tableB_index].rowID << std::endl;
            std::cout << sortedTable1[tabelA_index].key << " " << sortedTable2[tableB_index].key << std::endl;
            // std::cout << std::endl;

            tableB_index++;

            if (tableB_index == size2X){
                // tableB_index = 0;
                // tabelA_index++;
                break;
            }

        } else if (sortedTable1[tabelA_index].key < sortedTable2[tableB_index].key) {

            // while (sortedTable1[tabelA_index].key < sortedTable2[tableB_index].key) {
            // }
            tabelA_index++;
            if (tabelA_index == size1X)
                break;

            if (sortedTable1[tabelA_index-1].key == sortedTable1[tabelA_index].key)
                tableB_index = tableB_pin;
            else
                tableB_pin = tableB_index;

        } else if (sortedTable1[tabelA_index].key > sortedTable2[tableB_index].key) {
            tableB_index++;
            if (tableB_index == size2X){
                // tableB_index = 0;
                // tabelA_index++;
                break;
            }
        }
    }
}

uint64_t** ReadFile(const char* inp_data, uint32_t& table_rows, uint32_t& table_cols)
{
    uint64_t** fl_data;
    // std::cout << "!!!!!!!!!!! CHECKPOINT 1" << std::endl;

    FILE* fp;

    fp = fopen(inp_data,"r");
    // std::cout << "!!!!!!!!!!! CHECKPOINT 2" << std::endl;

    char*  line = NULL;
    size_t len  = 0;

    uint32_t rows = 0;
    uint32_t cols = 0;

    while (getline(&line, &len, fp) != -1)
        rows++;
    // std::cout << "!!!!!!!!!!! CHECKPOINT 3" << std::endl;

    char* columns = NULL;
    while ((columns = strtok((columns == NULL) ? line : NULL, ",")) != NULL)
        cols++;
    // std::cout << "!!!!!!!!!!! CHECKPOINT 4" << std::endl;

    free(line);
    // std::cout << "!!!!!!!!!!! CHECKPOINT 5" << std::endl;

    // std::cout << "ROWS = " << cols << std::endl;
    // std::cout << "COLS = " << rows << std::endl;

    // std::cout << "!!!!!!!!!!! CHECKPOINT 6" << std::endl;
    // --------------------------------------------------------------- //

    fl_data = new uint64_t*[cols];
    for(int i = 0 ; i < cols ; i++)
        fl_data[i] = new uint64_t[rows];

    // std::cout << "!!!!!!!!!!! CHECKPOINT 7" << std::endl;
    // --------------------------------------------------------------- //

    fseek(fp, 0, SEEK_SET);
    // std::cout << "!!!!!!!!!!! CHECKPOINT 8" << std::endl;

    line = NULL;
    len  = 0;

    int col_i = 0;
    while (getline(&line, &len, fp) != -1) {
        // std::cout << "!!!!!!!!!!! CHECKPOINT 9" << std::endl;
        char* tmp_col = NULL;

        int row_i = 0;
        // std::cout << "!!!!!!!!!!! CHECKPOINT 10" << std::endl;
        while ((tmp_col = strtok((tmp_col == NULL) ? line : NULL, ",\n")) != NULL) {
            // std::cout << "!!!!!!!!!!! CHECKPOINT 11" << std::endl;
            // std::cout << "DATA     = " << tmp_col << std::endl;
            fl_data[row_i][col_i] = strtoul(tmp_col, NULL, 0);
            // std::cout << "NEW DATA = " << fl_data[row_i][col_i] << std::endl;
            // std::cout << "NEW DATA = " << tmp_col << std::endl;
            row_i++;
        }
        // std::cout << "!!!!!!!!!!! CHECKPOINT 12" << std::endl;
        col_i++;
    }
    // std::cout << "!!!!!!!!!!! CHECKPOINT 13" << std::endl;

    free(line);
    fclose(fp);
    // std::cout << "!!!!!!!!!!! CHECKPOINT 14" << std::endl;

    table_rows = cols;
    table_cols = rows;

    return fl_data;
}
