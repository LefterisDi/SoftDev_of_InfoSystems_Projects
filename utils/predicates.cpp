/* File: predicates.cpp */

#include <iostream>

#include "../relationStructs.h"

using namespace std;


ResStruct* FindInResList(List<ResStruct>* resList , uint64_t elemID)
{
    if (resList == NULL)
        return NULL;

    Bucket<ResStruct>* bucket = resList->GetFirst();
    ResStruct* found = NULL;

    while (bucket != NULL) {
        for (int j = 0 ; j < bucket->GetBucketItems() ; j++) {
            cout << "TABLE IDDDDDDDD-> " << (*bucket)[j].tableID << endl;
            if (elemID == (*bucket)[j].tableID) {
                found = &(*bucket)[j];
                return found;
            }
        }

        bucket = bucket->GetNextBucket();
    }
    return found;
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

            switch (mask) {
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

    for (uint32_t l = 0; l < listOfArrays->tableList->GetTotalItems() ; l++){
        ResStruct* res = &( (*(*(listOfArrays->tableList))[l])[0] );

        switch (mask) {
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

int ComparisonPredicate(RelationTable* relTable , CompPred& cpred , List<FullResList>* resList)
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

        existingRel = new ResStruct;
        existingRel->tableID   = cpred.rel1;
        existingRel->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < relTable[cpred.rel1].rows ; i++)
            existingRel->rowIDlist->ListInsert(i);

        // frl->tableList->ListInsert(*existingRel);
        // resList.ListInsert(*frl);
    }

    uint32_t totalBuckets = existingRel->rowIDlist->GetTotalItems();

    switch (cpred.comp) {
        case '>':
            for (uint64_t i = 0; i < totalBuckets ;i++){
                uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
                // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
                if ( (relTable[cpred.rel1].table[cpred.colRel1][rowID] > cpred.num) == false ){
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
                if ( (relTable[cpred.rel1].table[cpred.colRel1][rowID] < cpred.num) == false ){
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
                if ( (relTable[cpred.rel1].table[cpred.colRel1][rowID] == cpred.num) == false ){
                    existingRel->rowIDlist->DeleteBucket(i);
                    i--;
                    totalBuckets--;
                }
            }
        break;
    }

    for (uint64_t i = 0; i < existingRel->rowIDlist->GetTotalItems() ;i++) {
        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
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

int JoinSelf(RelationTable* relTable , JoinPred& jpred ,  List<FullResList>* resList)
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

        existingRel = new ResStruct;
        existingRel->tableID   = jpred.rel1;
        existingRel->rowIDlist = new List<uint64_t>(sizeof(uint64_t) , sizeof(uint64_t));

        for (uint64_t i = 0 ; i < relTable[jpred.rel1].rows ; i++)
            existingRel->rowIDlist->ListInsert(i);

        // frl->tableList->ListInsert(*existingRel);
        // resList.ListInsert(*frl);
    }

    uint32_t totalBuckets = existingRel->rowIDlist->GetTotalItems();

    for (uint64_t i = 0; i < totalBuckets ;i++) {

        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        // cout << relTable[cpred.rel1].table[cpred.colRel1][rowID] << endl;
        if ( (relTable[jpred.rel1].table[jpred.colRel1][rowID] == relTable[jpred.rel2].table[jpred.colRel2][rowID]) == false ) {
            existingRel->rowIDlist->DeleteBucket(i);
            i--;
            totalBuckets--;
        }
    }

    for (uint64_t i = 0; i < existingRel->rowIDlist->GetTotalItems() ;i++) {
        uint64_t rowID = ( *((*existingRel->rowIDlist)[i]) )[0];
        cout << relTable[jpred.rel1].table[jpred.colRel1][rowID] << endl;
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

int DoAllCompPreds(RelationTable* relTable , List<CompPred>* compList , List<FullResList>* resList , bool* relExistsInRL)
{
    for (uint32_t i = 0 ; i < compList->GetTotalItems() ; i++) {

        CompPred* cpredp = &( (*(*compList)[i])[0] );
        ComparisonPredicate(relTable , *cpredp , resList);
        relExistsInRL[cpredp->rel1] = true;
    }
    return 1;
}

int DoAllJoinPreds(RelationTable* relTable , List<JoinPred>* joinList , List<FullResList>* resList ,  bool* relExistsInRL)
{
    uint32_t i     = 0;
    bool last      = false;
    bool firstTime = true;

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
                continue;

            } else {
                JoinSelf(relTable , *jpredp , resList);
                relExistsInRL[jpredp->rel1] = true;
                joinList->DeleteBucket(i);
                continue;
            }

        } else {
            JoinPredicate(relTable , *jpredp , resList);
            relExistsInRL[jpredp->rel1] = true;
            relExistsInRL[jpredp->rel2] = true;
            joinList->DeleteBucket(i);
            continue;
        }

        if (last == false)
            i++;
        else {
            i         = 0;
            last      = false;
            firstTime = false;
        }
    }
    return 1;
}
