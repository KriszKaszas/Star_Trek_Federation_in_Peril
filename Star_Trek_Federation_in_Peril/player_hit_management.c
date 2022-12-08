/**
*@file player_hit_management.c
*/

#include "player_hit_management.h"

static void explode_player_ship(PlayerShip *player_ship);


/**
*@brief explode_player_ship_if_dead
*@details Felszabaditja a jatekos hajojat, ha annak HP-ja 0 (vagy kevesebb), es kezeli az ahhoz tartozo pointereket.
*@param [in,out] *player_ship A jatekos hajojat tartalmazo adatstruktura pointere.
*@return void
*/

void explode_player_ship_if_dead(PlayerShip *player_ship)
{
    bool is_player_dead = player_ship->health <= 0;
    if(is_player_dead)
    {
        explode_player_ship(player_ship);
    }
}

/**
*@brief explode_player_ship
*@details Felszabaditja a jatekos hajojat es kezeli az ahhoz tartozo pointereket.
*@param [in,out] *player_ship A jatekos hajojat tartalmazo adatstruktura pointere.
*@return void
*/

static void explode_player_ship(PlayerShip *player_ship)
{
    free_player_ship(player_ship);
}

