/**
*@file random_number_in_interval.c
*/

#include "random_number_in_interval.h"

/**
*@brief random_number_in_range
*@details Egy random szammal ter vissza egy meghatarozott intervallumon belul. Csak ez a modul hivhatja.
*@param [in] Lower az intervallum also hatara.
*@param [in] Upper az intervallum felso hatara.
*@return int A generalt random szam.
*/
int random_number_in_range(int lower, int upper){
     return (rand() % (upper - lower + 1)) + lower;
}
