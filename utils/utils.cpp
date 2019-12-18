/* File: utils.cpp */

#include <iostream>
#include <fstream>
#include <string.h>

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

void MergeTables(List<uint64_t>& list, MergeTuple* sortedTable1, uint32_t size1X, MergeTuple* sortedTable2, uint32_t size2X)
{
    uint32_t tableA_index = 0;
    uint32_t tableB_index = 0;
    uint32_t tableB_pin   = 0;

    while (tableA_index < size1X)
    {
        // If the keys are the same then the pair is inserted in the list
        if (sortedTable1[tableA_index].key == sortedTable2[tableB_index].key) {

            // We store the 2 uint32_t rowIDs into a single uint64_t integer for better performance.
            // We combine the two numbers into one by placing the rowID1 in the lift-most 32 bits
            // and the rowID2 in the right-most 32 bits.
            uint64_t list_entry = sortedTable1[tableA_index].rowID;
            list_entry <<= 32;
            list_entry |= sortedTable2[tableB_index].rowID;

            list.ListInsert(list_entry);

            // std::cout << sortedTable1[tableA_index].rowID << " " << sortedTable2[tableB_index].rowID << std::endl;
            // std::cout << sortedTable1[tableA_index].key << " " << sortedTable2[tableB_index].key << std::endl;
            // std::cout << std::endl;

            tableB_index++;

            // If the first array has two consecutive elements that are the same and we reach
            // the end of table 2 while searching pairs for the first of the two elements then
            // we reset the pointer for the second table so that the remaining element of the
            // first table gets to be compared with the second array's elements.
            if (tableB_index == size2X){
                tableB_index = tableB_pin;
                tableA_index++;
            }

        } else if (sortedTable1[tableA_index].key < sortedTable2[tableB_index].key) {

            tableA_index++;
            if (tableA_index == size1X)
                break;

            if (sortedTable1[tableA_index-1].key == sortedTable1[tableA_index].key)
                tableB_index = tableB_pin;
            else
                tableB_pin = tableB_index;

        } else if (sortedTable1[tableA_index].key > sortedTable2[tableB_index].key) {
            tableB_index++;
            if (tableB_index == size2X){
                break;
            }
        }
    }
}

List<Query>* ReadQueryBatches(const char* workloads_path, const char* queries_path, List<RelationTable>& rels)
{
    char*  line = NULL;
    size_t len  = 0;

    size_t work_len  = strlen(workloads_path);
    size_t qr_len    = strlen(queries_path);

    char qr_path[work_len + qr_len + 2];
    memset(qr_path, 0, work_len + qr_len + 2);

    // Build the new path
    strcpy(qr_path, workloads_path);
    qr_path[work_len] = '/';
    strcat(qr_path, queries_path);

    // std::cout << qr_path << '\n';

    FILE* query_fp;
    static long int fp_pos;

    query_fp = fopen(qr_path,"r");

    fseek(query_fp, fp_pos, SEEK_SET);
    // if (query_fp == NULL) {
        // std::cout << "ERROR" << '\n';
    // }

    List<Query>* queries = new List<Query>(sizeof(Query), sizeof(Query));
    // List<RelationTable> relations(sizeof(RelationTable), sizeof(RelationTable));

    int res = 0;
    while ((res = getline(&line, &len, query_fp)) != -1 && strcmp(line, "F\n"))
    {
        char* tables      = NULL;
        char* predicates  = NULL;
        char* projections = NULL;

        // char bak_line[strlen(line)];
        // strcpy(bak_line, line);

        tables      = strtok(line, "|" );
        predicates  = strtok(NULL, "|" );
        projections = strtok(NULL, "\n");

        // Count the relations that will be used
        int index    = 0;
        int cnt_rels = 0;
        while (tables[index] != '\0')
            if (tables[index++] == ' ')
                cnt_rels++;
        cnt_rels++;

        // Create the query_list's entry
        Query* qr      = new Query();
        qr->query_rels = new RelationTable*[cnt_rels];
        qr->total_rels = cnt_rels;

        std::cout << "\n\n" << qr->total_rels;


        // >>>> Read and Store Relations that will be used <<<<
        char* tbl = NULL;
        index = 0;
        while ( (tbl = strtok( (tbl == NULL) ? tables : NULL, " \0") ) != NULL)
            qr->query_rels[index++] = &(*rels[atoi(tbl)])[0];


        // >>>> Read and Store (Comparison & Join) Predicates <<<<
        qr->comp_preds = new List<CompPred>(sizeof(CompPred), sizeof(CompPred));
        qr->join_preds = new List<JoinPred>(sizeof(JoinPred), sizeof(JoinPred));

        char* pred = NULL;
        while ( (pred = strtok( (pred == NULL) ? predicates : NULL, "&\0") ) != NULL) {

            char* bak_tok    = NULL;
            char* left_pred  = NULL;
            char* right_pred = NULL;

            char orig_pred[strlen(pred) + 1];
            strcpy(orig_pred, pred);

            std::cout << "\nPRED  = " << pred;

            left_pred  = strtok_r(pred, "=<>", &bak_tok);
            right_pred = strtok_r(NULL, ""   , &bak_tok);

            char pred_symb = orig_pred[strlen(left_pred)];

            if (pred_symb == '=') {

                char* rel2    = strtok_r(right_pred, ".", &bak_tok);
                char* colRel2 = strtok_r(NULL      , "" , &bak_tok);

                // If evaluates to true, we have a Join Predicate
                if (colRel2 != NULL) {

                    JoinPred* jp = new JoinPred;

                    jp->rel1    = atoi(strtok_r(left_pred, ".", &bak_tok));
                    jp->colRel1 = atoi(strtok_r(NULL     , "" , &bak_tok));

                    jp->rel2    = atoi(rel2);
                    jp->colRel2 = atoi(colRel2);

                    qr->join_preds->ListInsert(*jp);

                    // delete jp;

                // Otherwise, we have a Comparison Predicate (=)
                } else {

                    CompPred* cp = new CompPred;

                    cp->comp = '=';
                    cp->rel1    = atoi(strtok_r(left_pred, ".", &bak_tok));
                    cp->colRel1 = atoi(strtok_r(NULL     , "" , &bak_tok));
                    cp->num     = atoi(right_pred);

                    qr->comp_preds->ListInsert(*cp);

                    // delete cp;
                }

            // We have a Comparison Predicate (< or >)
            } else {

                CompPred* cp = new CompPred;

                if (pred_symb == '>')
                    cp->comp = '>';
                else if (pred_symb == '<')
                    cp->comp = '<';

                cp->rel1    = atoi(strtok_r(left_pred, ".", &bak_tok));
                cp->colRel1 = atoi(strtok_r(NULL     , "" , &bak_tok));
                cp->num     = atoi(right_pred);

                qr->comp_preds->ListInsert(*cp);
            }
        } // end of reading predicates


        // >>>> Read and Store Projections <<<<
        qr->proj = new List<Projection>(sizeof(Projection), sizeof(Projection));

        tbl = NULL;
        index = 0;
        while ( (tbl = strtok( (tbl == NULL) ? projections : NULL, " \0") ) != NULL) {

            Projection* pr = new Projection;

            char* bak_tok  = NULL;
            pr->rel    = atoi(strtok_r(tbl , ".", &bak_tok));
            pr->colRel = atoi(strtok_r(NULL, "" , &bak_tok));

            qr->proj->ListInsert(*pr);

            // delete pr;
        }

        queries->ListInsert(*qr);

        // delete qr;
    }

    fp_pos = ftell(query_fp);
    fclose(query_fp);
    free(line);

    if (res == -1)
        return NULL;

    return queries;
}

List<RelationTable>* ReadRelations(const char* workloads_path)
{
    char*  line = NULL;
    size_t len  = 0;

    size_t work_len  = strlen(workloads_path);

    List<RelationTable>* relations = new List<RelationTable>(sizeof(RelationTable), sizeof(RelationTable));
    // List<RelationTable> relations(sizeof(RelationTable), sizeof(RelationTable));

    while (getline(&line, &len, stdin) != -1) {

        RelationTable* tmp_rel_node = new RelationTable;

        char rel_path[work_len + strlen(line) + 2];
        memset(rel_path, 0, work_len + strlen(line) + 2);

        // Remove '\n'
        line[strlen(line) - 1] = '\0';

        // Build the new path
        strcpy(rel_path, workloads_path);
        rel_path[work_len] = '/';
        strcat(rel_path, line);

        // std::cout << rel_path << '\n';

        FILE* rel_fp;

        rel_fp = fopen(rel_path,"rb");

        fread(&tmp_rel_node->rows, sizeof(uint64_t), 1, rel_fp);
        fread(&tmp_rel_node->cols, sizeof(uint64_t), 1, rel_fp);

        tmp_rel_node->table  = new uint64_t*[tmp_rel_node->cols];
        for (int i = 0; i < tmp_rel_node->cols; i++)
            tmp_rel_node->table[i] = new uint64_t[tmp_rel_node->rows];

        for (int i = 0 ; i < tmp_rel_node->cols ; i++)
            for (int j = 0 ; j < tmp_rel_node->rows ; j++)
                fread(&tmp_rel_node->table[i][j], sizeof(uint64_t), 1, rel_fp);

        fclose(rel_fp);

        relations->ListInsert(*tmp_rel_node);

        // delete tmp_rel_node;
    }

    return relations;
}

uint64_t** ReadFile(const char* inp_data, uint32_t& table_rows, uint32_t& table_cols)
{
    uint64_t** fl_data;

    FILE* fp;

    fp = fopen(inp_data,"r");

    char*  line = NULL;
    size_t len  = 0;

    uint32_t rows = 0;
    uint32_t cols = 0;

    while (getline(&line, &len, fp) != -1)
        rows++;

    char* columns = NULL;
    while ((columns = strtok((columns == NULL) ? line : NULL, ",")) != NULL)
        cols++;

    free(line);

    // --------------------------------------------------------------- //

    fl_data = new uint64_t*[cols];
    for(uint32_t i = 0 ; i < cols ; i++)
        fl_data[i] = new uint64_t[rows];

    // --------------------------------------------------------------- //

    fseek(fp, 0, SEEK_SET);

    line = NULL;
    len  = 0;

    int col_i = 0;
    while (getline(&line, &len, fp) != -1) {
        char* tmp_col = NULL;

        int row_i = 0;
        while ((tmp_col = strtok((tmp_col == NULL) ? line : NULL, ",\n")) != NULL) {
            fl_data[row_i][col_i] = strtoul(tmp_col, NULL, 0);
            row_i++;
        }
        col_i++;
    }

    free(line);
    fclose(fp);

    table_rows = cols;
    table_cols = rows;

    return fl_data;
}
