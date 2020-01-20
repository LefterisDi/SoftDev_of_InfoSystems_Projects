/* File: main.cpp */

#include <bitset>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unistd.h>

#include "./opts/getopts.hpp"
#include "./utils/predicates.hpp"
#include "./utils/utils.hpp"
#include "./templates/list.hpp"
#include "./templates/vector.hpp"
#include "./Jobs/Jobs.hpp"
#include "./JobScheduler/JobScheduler.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    struct opt_types args[2];

    /*
     * +------------------------------------+
     * | args[0].optType.cp = <relA file>   |
     * +------------------------------------+
     */
    if (!getopts(argc, argv, (char *)"w:p,q:p", args))
        return -1;

    List<RelationTable> *relTableList;

    List<Query> *batchQueries = NULL;

    relTableList = ReadRelations(args[0].optType.cp);

    while ((batchQueries = ReadQueryBatches(args[0].optType.cp, args[1].optType.cp, *relTableList)) != NULL)
    {
        JobScheduler *js = new JobScheduler(1, batchQueries->GetTotalItems() + 1);
        QueryJobArgs *qja = new QueryJobArgs[batchQueries->GetTotalItems()];
        for (uint32_t i = 0; i < batchQueries->GetTotalItems(); i++)
        {
            qja[i].query = &((*(*batchQueries)[i])[0]);
            qja[i].qNum = i;

            js->addNewJob(&QueryJob, (void *)&qja[i]);
        }
        js->destroyScheduler(1);
        delete js;
        for (uint32_t i = 0; i < batchQueries->GetTotalItems(); i++)
        {
            cout << qja[i].res.c_str() << endl;
        }
        delete batchQueries;
        delete[] qja;
    }

    for (uint32_t l = 0; l < relTableList->GetTotalItems(); l++)
    {

        RelationTable *rtable = &((*(*relTableList)[l])[0]);
        delete rtable->colStats->distinctArray;
        delete rtable->colStats;
        for (uint32_t i = 0; i < rtable->cols; i++)
            delete[] rtable->table[i];
        delete[] rtable->table;
    }
    delete relTableList;

    return 0;
}