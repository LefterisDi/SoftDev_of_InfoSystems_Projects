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
    bool* relExistsInRL;

    List<JoinPred>* joinList = new List<JoinPred>(sizeof(JoinPred) , sizeof(JoinPred));
    List<CompPred>* compList = new List<CompPred>(sizeof(CompPred) , sizeof(CompPred));

    relTableList = ReadRelations(args[0].optType.cp);

    ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList);

    return 0;

    relExistsInRL = new bool[relTableList->GetTotalItems()];
    for (uint32_t i = 0; i < relTableList->GetTotalItems() ; i++)
        relExistsInRL[i] = false;

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

    delete relTableList;
    delete relExistsInRL;
    delete joinList;
    delete compList;

    return 0;
}
