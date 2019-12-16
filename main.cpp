/* File: main.cpp */

#include <bitset>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

// #include <list>
// #include <iterator>

#include "./opts/getopts.hpp"
// #include "./relationStructs.hpp"
// #include "./sortingAlg/quicksort.hpp"
// #include "./sortingAlg/tablesort.hpp"
// #include "./templates/list.hpp"
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
    if (!getopts(argc,argv,(char*)"w:p",args))
        return -1;

    ReadRelations(args[0].optType.cp);

    return 0;

    List<JoinPred>* joinList = new List<JoinPred>(sizeof(JoinPred) , sizeof(JoinPred));
    List<CompPred>* compList = new List<CompPred>(sizeof(CompPred) , sizeof(CompPred));


    // uint32_t*  rowIDs1;
    // uint32_t*  rowIDs2;

    int relations = 3;
    bool* relExistsInRL = new bool[relations];
    for (uint32_t i = 0; i < relations ; i++){
        relExistsInRL[i] = false;
    }
    // RelationTable* relTable = new RelationTable[relations];
    List<RelationTable>* relTableList = new List<RelationTable>(sizeof(RelationTable), sizeof(RelationTable));
    RelationTable rt;
    rt.rows = 4;
    rt.cols = 5;
    rt.table = new uint64_t*[rt.cols];
    for(int i = 0; i < rt.cols; i++)
        rt.table[i] = new uint64_t[rt.rows];
    
    relTableList->ListInsert(rt);

    rt.rows = 6;
    rt.cols = 3;
    rt.table = new uint64_t*[rt.cols];
    for(int i = 0; i < rt.cols; i++)
        rt.table[i] = new uint64_t[rt.rows];

    relTableList->ListInsert(rt);
    

    rt.rows = 6;
    rt.cols = 4;
    rt.table = new uint64_t*[rt.cols];
    for(int i = 0; i < rt.cols; i++)
        rt.table[i] = new uint64_t[rt.rows];

    relTableList->ListInsert(rt);


    

    default_random_engine gen;
    uniform_int_distribution<uint64_t> distribution(1,5);
    // uniform_int_distribution<uint64_t> distribution(1,ULLONG_MAX);

    // rowIDs1 = new uint32_t[size1x];
    // rowIDs2 = new uint32_t[size2x];

    // gen.seed((unsigned int) time(NULL));
    gen.seed(2);

    // for (int i = 0 ; i < size1x ; i++)
    //     rowIDs1[i] = i;

    // for (int i = 0 ; i < size2x ; i++)
    //     rowIDs2[i] = i;
    // for (int l = 0 ; l < relations ; l++){
    //     for (int i = 0 ; i < relTable[l].cols ; i++) {
    //         for (int j = 0 ; j < relTable[l].rows ; j++) {
    //             relTable[l].table[i][j] = distribution(gen);
    //         }
    //     }
    // }


    for (int l = 0 ; l < relTableList->GetTotalItems(); l++){
        RelationTable* rtable = &((*(*relTableList)[l])[0]);
        for (int i = 0 ; i < rtable->cols ; i++) {
            for (int j = 0 ; j < rtable->rows ; j++) {
                rtable->table[i][j] = distribution(gen);
            }
        }
    }

    // for (int l = 0 ; l < relations ; l++){
    //     for (int i = 0 ; i < relTable[l].rows ; i++) {
    //         for (int j = 0 ; j < relTable[l].cols ; j++) {
    //             cout << relTable[l].table[j][i] << " ";
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // }

    for (int l = 0 ; l < relTableList->GetTotalItems(); l++){
        RelationTable* rtable = &((*(*relTableList)[l])[0]);
        for (int i = 0 ; i < rtable->rows ; i++) {
            for (int j = 0 ; j < rtable->cols ; j++) {
                cout << rtable->table[j][i] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }



    JoinPred jp;
    CompPred cp;

    jp.type = 0;
    jp.rel1 = 1;
    jp.rel2 = 0;
    jp.colRel1 = 2;
    jp.colRel2 = 1;
    joinList->ListInsert(jp);

    jp.rel1 = 1;
    jp.rel2 = 2;
    jp.colRel1 = 0;
    jp.colRel2 = 1;
    joinList->ListInsert(jp);

    // cp.type = 1;
    // cp.rel1 = 0;
    // cp.colRel1 = 2;
    // cp.comp = '>';
    // cp.num = (uint64_t)1;
    // compList->ListInsert(cp);

    cp.type = 1;
    cp.rel1 = 2;
    cp.colRel1 = 1;
    cp.comp = '>';
    cp.num = (uint64_t)0;
    compList->ListInsert(cp);

    // cp.type = 1;
    // cp.rel1 = 1;
    // cp.colRel1 = 1;
    // cp.comp = '>';
    // cp.num = (uint64_t)9726819158084351267;
    // compList->ListInsert(cp);

    List<FullResList>* resList = new List<FullResList>(sizeof(ResStruct) , sizeof(ResStruct));

    cout << "ONE\n" << endl;

    DoAllCompPreds(relTableList , compList , resList , relExistsInRL);


    for (uint32_t i = 0 ; i < resList->GetTotalItems() ; i++){
        cout << "BUCKET " << i << endl;
        FindInResList( (* (*resList)[i])[0].tableList , 5);
    }
    cout << endl;

    // cin.get();

    cout << "TWO\n" << endl;

    DoAllJoinPreds(relTableList , joinList , resList , relExistsInRL);

    for (uint32_t i = 0 ; i < resList->GetTotalItems() ; i++){
        cout << "BUCKET " << i << endl;
        ResStruct* existingRel = NULL;
        FindInResList( (* (*resList)[i])[0].tableList , 5);

        cout << endl;
    }



    // for (int l = 0 ; l < relations ; l++){
    //     for(int i = 0; i < relTable[l].cols; i++)
    //         delete[] relTable[l].table[i];
    //     delete[] relTable[l].table;
    // }

    // List<uint64_t>l(sizeof(uint64_t) , sizeof(uint64_t));

    // for (int i = 0 ; i < 10 ; i++){
    //     uint64_t t = distribution(gen);
    //     l.ListInsert(t);
    //     cout << t << " ";
    //     if ((i+1)%1==0){
    //         cout << endl;
    //     }
    // }
    // cout << endl << endl;

    // l.DeleteBucket(0);
    // cout << (*l[0])[0] << endl;
    // cout << (*l[5])[0] << endl;
    // l.DeleteBucket(5);
    // cout << (*l[5])[0] << endl;
    // l.DeleteBucket(7);
    // cout << (*l[4])[0] << endl;
    // cout << (*l[5])[0] << endl;
    // cout << (*l[6])[0] << endl;

    for (uint32_t i = 0 ; i < resList->GetTotalItems() ; i++){
        List<ResStruct>* tableList = (*(*resList)[i])[0].tableList;
        for (uint32_t j = 0 ; j < tableList->GetTotalItems() ; j++){
            delete (*(*tableList)[j])[0].rowIDlist;
        }
        delete tableList;
    }
    delete resList;
    delete joinList;
    delete compList;
    delete relTableList;

    // for (int j = 0; j < relations ; j++){
    //     for(int i = 0; i < relTable[j].cols; i++)
    //         delete[] relTable[j].table[i];
    //     delete[] relTable[j].table;
    // }
    // delete[] relTable;'
    delete[] relExistsInRL;

}