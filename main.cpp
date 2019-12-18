/* File: main.cpp */

#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>

#include "./opts/getopts.hpp"
#include "./utils/predicates.hpp"
#include "./utils/utils.hpp"

using namespace std;

int main(int argc , char* argv[])
{
    struct opt_types args[2];

    /*
     * +------------------------------------+
     * | args[0].optType.cp = <relA file>   |
     * +------------------------------------+
     */
    if (!getopts(argc,argv,(char*)"w:p,q:p",args))
        return -1;


    List<RelationTable>* relTableList;

    // List<JoinPred>* joinList = new List<JoinPred>(sizeof(JoinPred) , sizeof(JoinPred));
    // List<CompPred>* compList = new List<CompPred>(sizeof(CompPred) , sizeof(CompPred));
    List<Query>* batchQueries = NULL;

    relTableList = ReadRelations(args[0].optType.cp);

    // ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
    // ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
    // ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
    // ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
    // ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << '\n';
    // cout << ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << '\n';
    // cout << ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << '\n';
    // cout << ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);
    // std::cout << '\n';


    while( (batchQueries = ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList) ) != NULL ) {
        std::cout << "\n\n";

        for (uint32_t i = 0; i < batchQueries->GetTotalItems() ; i++){

            Query* query = &( (*(*batchQueries)[i])[0] );

            // cout << "\nPROJECTIONS!!!!!!!!!!!! = " << query->proj->GetTotalItems() << endl;
            // cout << query->join_preds->GetTotalItems() << endl;
            // cout << query->comp_preds->GetTotalItems() << endl;
            // cout << query->total_rels << endl;

            bool* relExistsInRL = new bool[(*(*batchQueries)[i])[0].total_rels];
            for (uint32_t j = 0; j < (*(*batchQueries)[i])[0].total_rels ; j++)
                relExistsInRL[i] = false;

            List<FullResList>* resList = new List<FullResList>(sizeof(ResStruct) , sizeof(ResStruct));


            DoAllCompPreds( query->query_rels , query->comp_preds , resList , relExistsInRL );

            DoAllJoinPreds( query->query_rels , query->join_preds , resList , relExistsInRL );

            for (uint32_t l = 0 ; l < query->proj->GetTotalItems() ; l++){

                Projection* pr = &( (*(*query->proj)[l])[0] );
                ResStruct* res = NULL;

                for (uint32_t m = 0 ; m < resList->GetTotalItems() ; m++){
                    FullResList* fres = &( (*(*resList)[m])[0] );
                    res = FindInResList(fres->tableList , pr->rel);
                    if (res != NULL){
                        break;
                    }
                }
                if (res == NULL){
                    exit(1);
                }
                uint64_t sum = 0;
                for (uint32_t m = 0 ; m < res->rowIDlist->GetTotalItems() ; m++){
                    uint64_t rowID = (*(*res->rowIDlist)[m])[0];
                    sum += query->query_rels[pr->rel]->table[pr->colRel][rowID];
                }

                cout << sum << " ";

            }
            cout << endl;

            delete[] relExistsInRL;
            for (uint32_t l = 0 ; l < resList->GetTotalItems() ; l++){
                List<ResStruct>* tableList = (*(*resList)[l])[0].tableList;
                for (uint32_t k = 0 ; k < tableList->GetTotalItems() ; k++){
                    delete (*(*tableList)[k])[0].rowIDlist;
                }
                delete tableList;
            }
            delete resList;

        }

        // for (uint32_t j = 0; j < batchQueries->GetTotalItems() ; j++){
        //     Query* q = &( (*(*batchQueries)[j])[0] );

        //     delete q->comp_preds;
        //     delete q->join_preds;
        //     delete q->proj;

        // }

        // delete batchQueries;

        std::cout << "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << '\n';
    }

    for (uint32_t l = 0; l < relTableList->GetTotalItems() ; l++){
        RelationTable* rtable = &((*(*relTableList)[l])[0]);
        for (uint32_t i = 0 ; i < rtable->cols ; i++){
            delete[] rtable->table[i];
        }
        delete[] rtable->table;
    }

    delete relTableList;
    return 0;

    for (int l = 0 ; l < relTableList->GetTotalItems(); l++) {
        cout << "TableID -> " << l << endl;
        sleep(1);
        RelationTable* rtable = &((*(*relTableList)[l])[0]);
        for (int i = 0 ; i < rtable->rows ; i++) {
            for (int j = 0 ; j < rtable->cols ; j++) {
                cout << rtable->table[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    // delete relTableList;
    // delete relExistsInRL;
    // delete joinList;
    // delete compList;

    return 0;
}
