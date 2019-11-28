/* File: getopts.c */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getopts.hpp"

bool isIntNumeric(char* ch)  /* User input validation */
{
    int cntChars = 0;
    bool dashLocated = false;

    /* We iterate all characters given from command line */
    while(*ch) {

        /* As soon as, we detect any non-digit character, we terminate validation process */
        if (!isdigit(*ch++)) {

            if (cntChars == 0 && *(ch - 1) == '-')
                dashLocated = true;
            else
                return false;
        }
        cntChars++;
    }

    if (dashLocated && cntChars == 1)
        return false;

    return true;             /* No non-digit character was detected */
}

bool isFloatDoubleNumeric(char* ch)     /* User input validation */
{
    int cntChars = 0;
    bool dashLocated = false;
    bool dotLocated = false;

    /* We iterate all characters given from command line */
    while(*ch) {

        /* As soon as, we detect any non-digit character, we terminate validation process */
        if (!isdigit(*ch++)) {
            if (*(ch - 1) == '.' && !dotLocated) {
                dotLocated = true;
                continue;
            } else if (cntChars == 0 && *(ch - 1) == '-')
                dashLocated = true;
            else
                return false;
        }
        cntChars++;
    }

    if (dashLocated && cntChars == 1)
        return false;

    return true;             /* No non-digit character was detected */
}

bool getopts(int argc, char* argv[], char* params, struct opt_types* args)
{
    if (params == NULL)
        return false;

    int len = strlen(params);

    int  paramIndex  = 0;
    bool returnError = false;

    char* save_strtok_ptr;

    /*
     * We use a temporary array for performing tokenization
     * on it, so we can parse all arguments independently
     * with the traverse of the given arguments.
     */
    char tmpParams[strlen(params) + 1];
    strcpy(tmpParams, params);

    for (int m = 0 ; m < len ; m++ , paramIndex++)
    {
        char* currParam;

        currParam = strtok_r((m ? NULL : tmpParams) , ":" , &save_strtok_ptr);
        strtok_r(NULL , "," , &save_strtok_ptr);

        m += strlen(currParam);
        m++; // Skipping ':'

        char typeOfParam = params[m];
        m++; // Skipping ','

        bool paramLocated = false;

        for (int j = 1 ; j < argc ; j++)
        {
            char currTypeOfParam = '\0';

            int k = 0;
            while (k < len) {

                int tmpLen = 0;

                for (int l = k; l < len && params[l] != ':'; l++ , tmpLen++)
                    ;

                /* We search current option's type */
                if (strlen(currParam) < tmpLen || !strncmp(&params[k] , &argv[j][1] , tmpLen)) {
                    currTypeOfParam = params[k+1];
                    break;
                }

                while (params[k++] != ',' && k < len)
                    ;
            }

            if (!strcmp(&argv[j][1] , currParam))
            {
                switch (typeOfParam)
                {
                    case 'c':
                        if ((++j) == argc)
                            break;
                        else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = CHAR;
                            args[paramIndex].optType.c = argv[j][0];
                        }
                        paramLocated = true;
                    break;

                    case 'p':
                        if ((++j) == argc)
                            break;
                        else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = CHAR_POINTER;
                            args[paramIndex].optType.cp = argv[j];
                        }
                        paramLocated = true;
                    break;

                    case 'i':
                        if ((++j) == argc)
                            break;
                        else if (!isIntNumeric(argv[j])) {
                            printf("NON-NUMERIC\n");
                            returnError = true;

                        } else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = INT;
                            args[paramIndex].optType.i = atoi(argv[j]);
                        }
                        paramLocated = true;
                    break;

                    case 'u':
                        if ((++j) == argc)
                            break;
                        else if (!isIntNumeric(argv[j])) {
                            printf("NON-NUMERIC\n");
                            returnError = true;

                        } else if (atoi(argv[j]) < 0) {
                            printf("NON-NEGATIVE\n");
                            returnError = true;

                        } else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = UNSIGNED_INT;
                            args[paramIndex].optType.ui = atoi(argv[j]);
                        }
                        paramLocated = true;
                    break;

                    case 'f':
                        if ((++j) == argc)
                            break;

                        else if (!isFloatDoubleNumeric(argv[j])) {
                            printf("\nNON-FLOAT-NUMERIC\n\n");
                            returnError = true;

                        } else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = FLOAT;
                            args[paramIndex].optType.f = (float)atof(argv[j]);
                        }
                        paramLocated = true;
                    break;

                    case 'd':
                        if ((++j) == argc)
                            break;

                        else if (!isFloatDoubleNumeric(argv[j])) {
                            printf("NON-FLOAT-NUMERIC\n");
                            returnError = true;

                        } else {
                            if (params[m] == 'n')
                                m++;

                            args[paramIndex].chosenTypeIndex = DOUBLE;
                            args[paramIndex].optType.d = atof(argv[j]);
                        }
                        paramLocated = true;
                    break;

                    case 'n':
                        args[paramIndex].chosenTypeIndex = BOOL;
                        args[paramIndex].optType.b = true;
                        paramLocated = true;
                    break;

                    default:
                        printf("UNSUPPORTED ARGUMENT TYPE\n");
                    break;
                } // switch
                break;

            } else {
                /*
                 * We skip current option's argument only if current, under
                 * checking option, isn't a flag option which doesn't have
                 * any argument next to it.
                 */
                if (currTypeOfParam != 'n')
                    j++;
            }
        } // inner for

        if (!paramLocated)
        {
            if (params[m] == 'n') {
                args[paramIndex].chosenTypeIndex = BOOL;
                args[paramIndex].optType.b = true;
                m++; // Skipping ':'

            } else if (typeOfParam != 'n') {
                printf("MISSING ARGUMENT\n");
                return false;

            } else {
                args[paramIndex].chosenTypeIndex = BOOL;
                args[paramIndex].optType.b = false;
            }
        }
    } // outer for

    if (returnError)
        return false;

    return true;
}
