/* File: predicates.cpp */

#include <iostream>

#include "../relationStructs.hpp"
#include "../sortingAlg/tablesort.hpp"

using namespace std;

ResStruct* FindInResList(List<ResStruct>* resList , uint64_t elemID)
{
    if (resList == NULL)
        return NULL;

    Bucket<ResStruct>* bucket = resList->GetFirst();
    ResStruct* found = NULL;

    while (bucket != NULL)
    {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {

            ResStruct* rss = &( (*bucket)[j] );
            cout << "TABLE IDDDDDDDD-> " << rss->tableID << endl;
            for (uint32_t i = 0 ; i < rss->rowIDlist->GetTotalItems() ; i++){
                uint32_t rowID = (*(*rss->rowIDlist)[i])[0];
                cout << (*(*rss->rowIDlist)[i])[0] << endl;
            }
            cout << endl;

            if (elemID == (*bucket)[j].tableID) {
                found = &(*bucket)[j];
                return found;
            }
        }
        bucket = bucket->GetNextBucket();
    }
    return found;
}

int ComparisonPredicate(List<RelationTable>* relTableList , CompPred& cpred , List<FullResList>* resList)
{
    ResStruct*   existingRel = NULL;
    FullResList* frl         = NULL;
    bool         exists      = true;

    for (uint32_t i = 0 ; i < resList->GetTotalBuckets() ; i++) {

        existingRel = FindInResList( (* (*resList)[i])[0].tableList , cpred.rel1);

        if (existingRel != NULL)
            break;
    }

    if (existingRel == NULL) {

        exists         = false;
        frl            = new FullResList;
        frl->tableList = new List<ResStruct>(sizeof(ResStruct) , sizeof(ResStruct));

        existingRel            = new ResStruct;
        existingRel->tableID   = cpred.rel1;
        existingRel->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[cpred.rel1])[0].rows ; i++)
            existingRel->rowIDlist->ListInsert(i);

        // frl->tableList->ListInsert(*existingRel);
        // resList.ListInsert(*frl);
    }

    uint32_t totalBuckets = existingRel->rowIDlist->GetTotalItems();

    switch (cpred.comp)
    {
        case '>':
            for (uint64_t i = 0; i < totalBuckets ;i++){
                uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
                // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
                if ( (  (*(*relTableList)[cpred.rel1])[0].table[cpred.colRel1][rowID] > cpred.num) == false ) {
                    existingRel->rowIDlist->DeleteBucket(i);
                    i--;
                    totalBuckets--;
                }
            }
        break;

        case '<':
            for (uint64_t i = 0; i < totalBuckets ;i++){
                uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
                // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
                if ( ( (*(*relTableList)[cpred.rel1])[0].table[cpred.colRel1][rowID] < cpred.num) == false ) {
                    existingRel->rowIDlist->DeleteBucket(i);
                    i--;
                    totalBuckets--;
                }
            }
        break;

        case '=':
            for (uint64_t i = 0; i < totalBuckets ;i++){
                uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
                // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
                if ( ( (*(*relTableList)[cpred.rel1])[0].table[cpred.colRel1][rowID] == cpred.num) == false ) {
                    existingRel->rowIDlist->DeleteBucket(i);
                    i--;
                    totalBuckets--;
                }
            }
        break;
    }

    for (uint64_t i = 0; i < existingRel->rowIDlist->GetTotalItems() ;i++) {
        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        cout << (*(*relTableList)[cpred.rel1])[0].table[cpred.colRel1][rowID] << endl;
    }
    cout << endl;

    if (exists == false) {
        frl->tableList->ListInsert(*existingRel);
        resList->ListInsert(*frl);
        delete existingRel;
        delete frl;
    }

    return 1;
}

int DeleteTargetedSL(ResStruct* resStr , int mask , List<uint64_t>* doubleKeyList)
{
    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
    mask32_left <<= 32;

    List<uint64_t>* newList = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

    for (uint32_t i = 0 ; i < doubleKeyList->GetTotalBuckets() ; i++) {
        for (uint32_t j = 0 ; j < (*doubleKeyList)[i]->GetBucketItems() ; j++) {

            uint32_t newRowID = 0;
            switch (mask)
            {
                case 0:
                    newRowID = ( ( (*(*doubleKeyList)[i])[j]  & mask32_left ) >> 32);
                break;

                case 1:
                    newRowID = ( (*(*doubleKeyList)[i])[j]  & mask32_right );
                break;
            }
            newList->ListInsert( (*(*resStr->rowIDlist)[newRowID])[0] );
        }
    }

    delete resStr->rowIDlist;
    resStr->rowIDlist = NULL;
    resStr->rowIDlist = newList;

    return 1;
}

int DeleteTargeted( FullResList* listOfArrays , int mask , List<uint64_t>* doubleKeyList)
{
    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
    mask32_left <<= 32;

    for (uint32_t l = 0; l < listOfArrays->tableList->GetTotalItems() ; l++) {

        ResStruct* res = &( (*(*(listOfArrays->tableList))[l])[0] );

        switch (mask)
        {
            case 0:
                DeleteTargetedSL(res , 0 ,doubleKeyList);
            break;

            case 1:
                DeleteTargetedSL(res , 1 , doubleKeyList);
            break;
        }
    }
    return 1;
}

int JoinSelf(List<RelationTable>* relTableList , JoinPred& jpred ,  List<FullResList>* resList)
{
    ResStruct*   existingRel = NULL;
    FullResList* frl         = NULL;
    bool         exists      = true;

    for (uint32_t i = 0 ; i < resList->GetTotalBuckets() ; i++) {

        existingRel = FindInResList( (* (*resList)[i])[0].tableList , jpred.rel1);

        if (existingRel != NULL)
            break;
    }

    if (existingRel == NULL) {

        exists         = false;
        frl            = new FullResList;
        frl->tableList = new List<ResStruct>(sizeof(ResStruct) , sizeof(ResStruct));

        existingRel            = new ResStruct;
        existingRel->tableID   = jpred.rel1;
        existingRel->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[jpred.rel1])[0].rows ; i++)
            existingRel->rowIDlist->ListInsert(i);

        // frl->tableList->ListInsert(*existingRel);
        // resList.ListInsert(*frl);
    }

    uint32_t totalBuckets = existingRel->rowIDlist->GetTotalItems();

    for (uint64_t i = 0; i < totalBuckets ;i++) {

        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
        if ( (  (*(*relTableList)[jpred.rel1])[0].table[jpred.colRel1][rowID] == (*(*relTableList)[jpred.rel2])[0].table[jpred.colRel2][rowID]) == false ) {
            existingRel->rowIDlist->DeleteBucket(i);
            i--;
            totalBuckets--;
        }
    }

    for (uint64_t i = 0; i < existingRel->rowIDlist->GetTotalItems() ;i++) {

        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        cout << (*(*relTableList)[jpred.rel1])[0].table[jpred.colRel1][rowID] << endl;
    }
    cout << endl;

    if (exists == false) {
        frl->tableList->ListInsert(*existingRel);
        resList->ListInsert(*frl);
        delete existingRel;
        delete frl;
    }

    return 1;
}

void TablesExistInMidStruct(List<RelationTable>* relTableList , JoinPred&     jpred       ,
                            ResStruct*&    existingRel1 , ResStruct*&   existingRel2,
                            FullResList*&  frl1         , FullResList*& frl2        ,
                            bool&          exists1      , bool&         exists2)
{
    if (existingRel1 == NULL && existingRel2 == NULL)
    {
        exists1 = false;
        exists2 = false;

        frl1            = new FullResList;
        frl1->tableList = new List<ResStruct>(sizeof(ResStruct) , sizeof(ResStruct));

        existingRel1            = new ResStruct;
        existingRel1->tableID   = jpred.rel1;
        existingRel1->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[jpred.rel1])[0].rows ; i++)
            existingRel1->rowIDlist->ListInsert(i);

        existingRel2            = new ResStruct;
        existingRel2->tableID   = jpred.rel2;
        existingRel2->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[jpred.rel2])[0].rows ; i++)
            existingRel2->rowIDlist->ListInsert(i);

        // frl1->tableList->ListInsert(*existingRel1);
        // frl1->tableList->ListInsert(*existingRel2);
        // resList.ListInsert(*frl);
    }
    // else if (existingRel1 != NULL && existingRel2 != NULL){
    //     // frl1->tableList->GetLast()
    //     //fuse the two
    // }
    else if (existingRel1 != NULL && existingRel2 == NULL) {

        exists1 = true;
        exists2 = false;

        existingRel2            = new ResStruct;
        existingRel2->tableID   = jpred.rel2;
        existingRel2->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[jpred.rel2])[0].rows ; i++)
            existingRel2->rowIDlist->ListInsert(i);

        // frl1->tableList->ListInsert(*existingRel2);
    } else if (existingRel1 == NULL && existingRel2 != NULL) {

        exists1 = false;
        exists2 = true;

        existingRel1            = new ResStruct;
        existingRel1->tableID   = jpred.rel1;
        existingRel1->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < (*(*relTableList)[jpred.rel1])[0].rows ; i++)
            existingRel1->rowIDlist->ListInsert(i);

        // frl2->tableList->ListInsert(*existingRel1);
    }
}

void CreateTableForJoin(List<RelationTable>* relTableList , uint64_t relID , bool exists , uint64_t**& table , uint32_t& rowNum , ResStruct*& existingRel)
{
    if (exists == true) {

        table  = new uint64_t*[(*(*relTableList)[relID])[0].cols];
        rowNum = existingRel->rowIDlist->GetTotalItems();

        for(int i = 0; i < (*(*relTableList)[relID])[0].cols; i++)
            table[i] = new uint64_t[rowNum];

        //columns change with the first parameter and rowIDs with the second
        for (int j =  0 ; j < rowNum ; j++) {
            for (int i = 0 ; i < (*(*relTableList)[relID])[0].cols ; i++) {

                uint64_t rowID = (*(*existingRel->rowIDlist)[j])[0];
                table[i][j] = (*(*relTableList)[relID])[0].table[i][rowID];
            }
        }

    } else {

        table  = (*(*relTableList)[relID])[0].table;
        // table1 = new uint64_t*[relTable[relID].cols];
        rowNum = (uint32_t)(*(*relTableList)[relID])[0].rows;
        // for(int i = 0; i < relTable[relID].cols; i++)
        //     table1[i] = new uint64_t[rowNum1];
        // for (int j =  0 ; j < rowNum1 ; j++){
        //     for (int i = 0 ; i < relTable[relID].cols ; i++){
        //         table1[i][j] = relTable[relID].table[i][j];
        //     }
        // }
    }
}

void InsertAndFuseInMidStruct(List<uint64_t>*& doubleKeyList , List<FullResList>*& resList , uint32_t& pos2 ,
                              ResStruct*&      existingRel1  , ResStruct*&         existingRel2             ,
                              FullResList*&    frl1          , FullResList*&       frl2                     ,
                              bool&            exists1       , bool&               exists2)
{
    if (exists1 == false && exists2 == false) {

        DeleteTargetedSL(existingRel1 , 0 , doubleKeyList);
        DeleteTargetedSL(existingRel2 , 1 , doubleKeyList);

        cout << "T ID Solo " << existingRel1->tableID << endl;
        for (uint32_t i = 0 ; i < existingRel1->rowIDlist->GetTotalItems() ; i++) {
            cout << (*(*existingRel1->rowIDlist)[i])[0] << endl;
        }
        cout << endl;

        cout << "T ID Solo " << existingRel2->tableID << endl;
        for (uint32_t i = 0 ; i < existingRel2->rowIDlist->GetTotalItems() ; i++)
            cout << (*(*existingRel2->rowIDlist)[i])[0] << endl;

        frl1->tableList->ListInsert(*existingRel1);
        frl1->tableList->ListInsert(*existingRel2);
        resList->ListInsert(*frl1);

    } else if (exists1 == true && exists2 == true) {

        cout << "FUUUUUUUU-SION HAAAAAAA" << endl;
        DeleteTargeted(frl1 , 0 , doubleKeyList);
        DeleteTargeted(frl2 , 1 , doubleKeyList);

        (*frl1->tableList) += (*frl2->tableList);

        resList->DeleteBucket(pos2);

    } else if (exists1 == true && exists2 == false) {

        DeleteTargeted(frl1 , 0 , doubleKeyList);
        DeleteTargetedSL(existingRel2 , 1 , doubleKeyList);

        for (uint32_t i = 0 ; i < frl1->tableList->GetTotalItems() ; i++) {

            ResStruct* rs = &( (*(*frl1->tableList)[i])[0] );
            cout << "T ID DUO " << rs->tableID << endl;
            for (uint32_t j = 0 ; j < rs->rowIDlist->GetTotalItems() ; j++){
                cout << (*(*rs->rowIDlist)[j])[0] << " ";
            }
            cout << endl;
        }
        cout << endl;

        cout << "T ID Solo " << existingRel2->tableID << endl;
        for (uint32_t i = 0 ; i < existingRel2->rowIDlist->GetTotalItems() ; i++)
            cout << (*(*existingRel2->rowIDlist)[i])[0] << endl;

        frl1->tableList->ListInsert(*existingRel2);

    } else {

        DeleteTargeted(frl2 , 1 , doubleKeyList);
        DeleteTargetedSL(existingRel1 , 0 , doubleKeyList);

        cout << "T ID Solo " << existingRel1->tableID << endl;
        for (uint32_t i = 0 ; i < existingRel1->rowIDlist->GetTotalItems() ; i++) {
            cout << (*(*existingRel1->rowIDlist)[i])[0] << endl;
        }
        cout << endl;

        for (uint32_t i = 0 ; i < frl2->tableList->GetTotalItems() ; i++) {

            ResStruct* rs = &( (*(*frl2->tableList)[i])[0] );
            cout << "T ID DUO " << rs->tableID << endl;
            for (uint32_t j = 0 ; j < rs->rowIDlist->GetTotalItems() ; j++) {
                cout << (*(*rs->rowIDlist)[j])[0] << " ";
            }
            cout << endl;
        }
        cout << endl;

        frl2->tableList->ListInsert(*existingRel1);
    }

}

int JoinPredicate(List<RelationTable>* relTableList , JoinPred& jpred ,  List<FullResList>* resList)
{
    ResStruct* existingRel1 = NULL;
    ResStruct* existingRel2 = NULL;

    FullResList* frl1 = NULL;
    FullResList* frl2 = NULL;

    uint32_t pos2 = -1;

    bool exists1 = true;
    bool exists2 = true;

    List<uint64_t>* doubleKeyList = new List<uint64_t>(1048576 , sizeof(uint64_t));

    uint64_t** table1;
    uint64_t** table2;

    uint32_t rowNum1 = 0;
    uint32_t rowNum2 = 0;


    for (uint32_t i = 0 ; i < resList->GetTotalItems() ; i++) {

        existingRel1 = FindInResList( (* (*resList)[i])[0].tableList , jpred.rel1);

        if (existingRel1 != NULL) {
            frl1 = &( (*(*resList)[i])[0] );
            break;
        }
    }

    for (uint32_t i = 0 ; i < resList->GetTotalItems() ; i++) {

        existingRel2 = FindInResList( (* (*resList)[i])[0].tableList , jpred.rel2);

        if (existingRel2 != NULL) {
            frl2 = &( (*(*resList)[i])[0] );
            pos2 = i;
            break;
        }
    }

    TablesExistInMidStruct(relTableList , jpred , existingRel1 , existingRel2 , frl1 , frl2 , exists1 , exists2);

    CreateTableForJoin(relTableList , jpred.rel1 , exists1 , table1 , rowNum1 , existingRel1);
    CreateTableForJoin(relTableList , jpred.rel2 , exists2 , table2 , rowNum2 , existingRel2);

    cout << "FIRST TABLE " << endl;
    for (uint32_t i = 0 ; i < rowNum1 ; i++) {
        for (uint32_t j = 0 ; j < (*(*relTableList)[jpred.rel1])[0].cols ; j++) {
            cout << table1[j][i] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "SECOND TABLE " << endl;
    for (uint32_t i = 0 ; i < rowNum2 ; i++) {
        for (uint32_t j = 0 ; j < (*(*relTableList)[jpred.rel2])[0].cols ; j++) {
            cout << table2[j][i] << " ";
        }
        cout << endl;
    }
    cout << endl;

    MergeTuple* sortedTable1 = TableSortOnKey(table1 , rowNum1 , jpred.colRel1 , 8192);
    MergeTuple* sortedTable2 = TableSortOnKey(table2 , rowNum2 , jpred.colRel2 , 8192);

    // for (int i = 0 ; i < rowNum1 ; i++){
    //     cout << sortedTable1[i].rowID << endl;
    // }

    MergeTables(*doubleKeyList, sortedTable1 , rowNum1 , sortedTable2 , rowNum2);

    uint64_t mask32_left  = 0xFFFFFFFF;
    uint32_t mask32_right = 0xFFFFFFFF;
    mask32_left <<= 32;

    for (int i = 0 ; i < doubleKeyList->GetTotalBuckets() ; i++) {
        for (int j = 0 ; j < (*doubleKeyList)[i]->GetBucketItems() ; j++) {
            cout << ( ( (*(*doubleKeyList)[i])[j]  & mask32_left ) >> 32) << " " << ( (*(*doubleKeyList)[i])[j]  & mask32_right );
            cout << endl;
        }
        cout << endl;
    }

    InsertAndFuseInMidStruct(doubleKeyList , resList , pos2 , existingRel1 , existingRel2 , frl1 , frl2 , exists1 , exists2);

    delete doubleKeyList;

    delete[] sortedTable1;
    delete[] sortedTable2;

    if (exists1 == false)
        delete existingRel1;

    else {
        for(int i = 0; i < (*(*relTableList)[jpred.rel1])[0].cols; i++)
            delete[] table1[i];
        delete[] table1;
    }

    if (exists2 == false)
        delete existingRel2;

    else {
        for(int i = 0; i < (*(*relTableList)[jpred.rel2])[0].cols; i++)
            delete[] table2[i];
        delete[] table2;
    }

    if (exists1 == false && exists2 == false)
        delete frl1;

    return 1;
}

int DoAllCompPreds( List<RelationTable>* relTableList , List<CompPred>* compList , List<FullResList>* resList , bool* relExistsInRL)
{
    for (uint32_t i = 0 ; i < compList->GetTotalItems() ; i++) {

        CompPred* cpredp = &( (*(*compList)[i])[0] );
        ComparisonPredicate(relTableList , *cpredp , resList);
        relExistsInRL[cpredp->rel1] = true;
        cout << cpredp->rel1 << endl;
    }

    return 1;
}

int DoAllJoinPreds(List<RelationTable>* relTableList , List<JoinPred>* joinList , List<FullResList>* resList ,  bool* relExistsInRL)
{
    uint32_t i         = 0;
    bool     last      = false;
    bool     firstTime = true;

    while (joinList->GetTotalItems() > 0) {

        JoinPred* jpredp = &( (*(*joinList)[i])[0] );

        if ( (*joinList)[i] == joinList->GetLast() )
            last = true;

        if ( firstTime == true && relExistsInRL[jpredp->rel1] == false && relExistsInRL[jpredp->rel2] == false) {
            i++;
            continue;
        }

        if (jpredp->rel1 == jpredp->rel2) {

            if (jpredp->colRel1 == jpredp->colRel2) {
                joinList->DeleteBucket(i);
                if (last == true) {
                    i = 0;
                    last = false;
                    firstTime = false;
                }
                continue;

            } else {
                JoinSelf(relTableList , *jpredp , resList);
                relExistsInRL[jpredp->rel1] = true;
                joinList->DeleteBucket(i);
                if (last == true) {
                    i = 0;
                    last = false;
                    firstTime = false;
                }
                continue;
            }

        } else {
            JoinPredicate(relTableList , *jpredp , resList);
            relExistsInRL[jpredp->rel1] = true;
            relExistsInRL[jpredp->rel2] = true;
            joinList->DeleteBucket(i);

            if (last == true) {
                i = 0;
                last = false;
                firstTime = false;
            }
            continue;
        }

        if (last == false)
            i++;

        else {
            i = 0;
            last = false;
            firstTime = false;
        }
    }
    return 1;
}
