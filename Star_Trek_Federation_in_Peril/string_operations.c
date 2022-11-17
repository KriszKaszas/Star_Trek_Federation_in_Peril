/**
*@file string_operations.c
*/

#include "string_operations.h"

/**
*@brief dinstr_alloc
*@details dinamikus sztringet allokal
*@param [] str
*@param [] size
*@return bool
*/

bool dinstr_alloc(DinStr *str, int size){
    str->size = size;
    str->str = (char*) malloc(size * sizeof(char));
    return str->str != NULL;
}
