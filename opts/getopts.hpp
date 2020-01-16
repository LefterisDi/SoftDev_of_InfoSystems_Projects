/* File: getopts.h */

#include <stdbool.h>

#ifndef __GETOPTS__
#define __GETOPTS__

// enum UnionTypes { BOOL , CHAR , CHAR_POINTER , INT , UNSIGNED_INT , FLOAT , DOUBLE };

#define FOREACH_UNION_TYPE(TYPE) \
        TYPE(BOOL)               \
        TYPE(CHAR)               \
        TYPE(CHAR_POINTER)       \
        TYPE(INT)                \
        TYPE(UNSIGNED_INT)       \
        TYPE(FLOAT)              \
        TYPE(DOUBLE)             \

#define GENERATE_ENUM(ENUM_VAL) ENUM_VAL,
#define GENERATE_STRING(STRING) #STRING,

enum UnionTypes {
    FOREACH_UNION_TYPE(GENERATE_ENUM)
};

static const char* UnionTypes_Str[] = {
    FOREACH_UNION_TYPE(GENERATE_STRING)
};

union types {
    bool         b;
    char         c;
    char*        cp;
    int          i;
    unsigned int ui;
    float        f;
    double       d;
};

struct opt_types {
    int         chosenTypeIndex;
    union types optType;
};

bool getopts(int, char* [], char *, struct opt_types *);

#endif //__GETOPTS__
