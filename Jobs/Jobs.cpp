#include "iostream"

#include "../utils/relationStructs.hpp"
#include "../templates/list.hpp"
#include "../templates/vector.hpp"
#include "../utils/predicates.hpp"
#include "../JobScheduler/JobScheduler.hpp"

using namespace std;

void QueryJob(void* arg){

    Query* query = (Query*)arg;

    bool* relExistsInRL = new bool[query->total_rels]();
    for (uint32_t j = 0; j < query->total_rels ; j++)
        relExistsInRL[j] = false;

    List<FullResList>* resList = new List<FullResList>(sizeof(ResStruct) , sizeof(ResStruct));

    DoAllCompPreds( query->query_rels , query->comp_preds , resList , relExistsInRL);
    DoAllJoinPreds( query->query_rels , query->join_preds , resList , relExistsInRL);

    for (uint32_t l = 0 ; l < query->proj->GetTotalItems() ; l++) {

        Projection* pr  = &( (*(*query->proj)[l])[0] );
        ResStruct*  res = NULL;

        for (uint32_t m = 0 ; m < resList->GetTotalItems() ; m++) {

            FullResList* fres = &( (*(*resList)[m])[0] );
            res = FindInResList(fres->tableList , pr->rel);
            if (res != NULL){
                break;
            }
        }

        if (res == NULL)
            exit(1);

        uint64_t sum = 0;
        for (uint32_t m = 0 ; m < res->rowIDvec->GetTotalItems() ; m++) {
            uint64_t rowID = (*res->rowIDvec)[m];
            sum += query->query_rels[pr->rel]->table[pr->colRel][rowID];
        }

        if (sum == 0){
            if (l != query->proj->GetTotalItems()-1){
                cout << "NULL ";
            }
            else {
                cout << "NULL";
            }

        }
        else{
            if (l != query->proj->GetTotalItems()-1){
                cout << sum << " ";
            }
            else {
                cout << sum;
            }
        }
    }
    cout << endl;

    delete[] relExistsInRL;
    for (uint32_t l = 0 ; l < resList->GetTotalItems() ; l++) {

        List<ResStruct>* tableList = (*(*resList)[l])[0].tableList;
        for (uint32_t k = 0 ; k < tableList->GetTotalItems() ; k++)
            delete (*(*tableList)[k])[0].rowIDvec;
        delete tableList;
    }
    delete resList;

    delete query->comp_preds;
    delete query->join_preds;
    delete query->proj;
    delete[] query->query_rels;
}