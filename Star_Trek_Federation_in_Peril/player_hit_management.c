/**
*@file player_hit_management.c
*/

#include "player_hit_management.h"

static void explode_player_ship(PlayerShip **player_ship, GameAssets **game_assets);

static void manage_score_on_player_death(PlayerShip **player_ship, GameAttributes *game_attributes);

/**
*@brief explode_player_ship_if_dead
*@details Felszabaditja a jatekos hajojat, ha annak HP-ja 0 (vagy kevesebb), es kezeli az ahhoz tartozo pointereket.
*@param [in,out] **player_ship A jatekos hajojat tartalmazo adatstruktura pointere.
*@param [in,out] **game_assets A jatekhoz szukseges assetek taroloja.
*@param [in,out] *game_attributes A jatek attributumainak taroloja.
*@return void
*/

void explode_player_ship_if_dead(PlayerShip **player_ship, GameAssets **game_assets, GameAttributes *game_attributes)
{
    bool is_player_dead = (*player_ship)->health <= 0;
    if(is_player_dead)
    {
        manage_score_on_player_death(player_ship, game_attributes);
        explode_player_ship(player_ship, game_assets);
    }
}

/**
*@brief explode_player_ship
*@details Felszabaditja a jatekos hajojat es kezeli az ahhoz tartozo pointereket.
*@param [in,out] **player_ship A jatekos hajojat tartalmazo adatstruktura pointere.
*@param [in,out] **game_assets A jatekhoz szukseges assetek taroloja.
*@return void
*/

static void explode_player_ship(PlayerShip **player_ship, GameAssets **game_assets)
{
    free_player_ship((*player_ship));
    (*player_ship) = NULL;
    (*game_assets)->player_ship = NULL;
}

/**
*@brief explode_player_ship
*@details Felszabaditja a jatekos hajojat es kezeli az ahhoz tartozo pointereket.
*@param [in,out] **player_ship A jatekos hajojat tartalmazo adatstruktura pointere.
*@param [in] *game_assets A jatekhoz szukseges assetek taroloja.
*@return void
*/

static void manage_score_on_player_death(PlayerShip **player_ship, GameAttributes *game_attributes)
{
    game_attributes->game_score += (*player_ship)->score_value;
}
