/**
*@file random_number_in_interval.c
*/

#include "random_number_in_interval.h"

/**
*@brief random_number_in_range
*@details egy random szammal ter vissza egy meghatarozott intervallumon belul. Csak ez a modul hivhatja
*@param [in] lower az intervallum also hatara.
*@param [in] upper az intervallum felso hatara.
*@return int
*/
int random_number_in_range(int lower, int upper){
     return (rand() % (upper - lower + 1)) + lower;
}
