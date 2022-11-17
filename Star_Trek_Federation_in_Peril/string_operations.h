#ifndef STRING_OPERATIONS_H_INCLUDED
#define STRING_OPERATIONS_H_INCLUDED

#include <string.h>

#include "debugmalloc.h"

typedef struct DinStr {
    int size;
    char *str;
} DinStr;


#endif // STRING_OPERATIONS_H_INCLUDED
