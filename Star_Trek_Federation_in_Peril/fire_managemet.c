/**
*@file fire_management.c
*/

#include "fire_management.h"

static void fire_ship_torpedo_if_selected(int *ship_number,
                                   int *selector,
                                   EnemyShip *temp_ship,
                                   GameAssets *game_assets);

static void increment_ship_number_if_ships_in_same_column(int *ship_number);

static void increment_ship_number(int *ship_number);



/**
*@brief fire_player_torpedo
*@details A jatekos torpedok kiloveseert felel.
*@param [out] *game_assets A jatekhoz szukseges assetek taroloja.
*@param [out] *game_attributes A jatek attributumainak taroloja.
*@return void
*/

void fire_player_torpedo(GameAssets *game_assets, GameAttributes *game_attributes)
{
    game_assets->player_torpedoes = add_torpedo_shot(game_assets->player_torpedoes, 5, 2,
                                                    game_assets->player_ship->x_coor, game_assets->player_ship->y_coor, false);
    game_attributes->isi.torpedo = false;
}

/**
*@brief fire_enemy_torpedoes
*@details Az ellenseges torpedok kiloveseert felel.
*@param [in] armada_size Az ellenseges hadseregmerete.
*@param [out] *game_assets A jatekhoz szukseges assetek taroloja.
*@return void
*/

void fire_enemy_torpedoes(int armada_size, GameAssets *game_assets)
{
    EnemyShip *temp_ship = game_assets->enemy_armada;
    int selector = random_number_in_range(3, 9);
    int ship_number = 0;
    while(temp_ship != NULL)
    {
        fire_ship_torpedo_if_selected(&ship_number, &selector, temp_ship, game_assets);
        increment_ship_number_if_ships_in_same_column(&ship_number);
        increment_ship_number(&ship_number);
        temp_ship = temp_ship->next_ship;
    }
}

/**
*@brief fire_ship_torpedo_if_selected
*@details A kivalasztott hajo loveseert felel.
*@param [in,out] *ship_number A kivalasztott hajo szama.
*@param [in,out] *selector A kivalasztashoz hasznalt szam.
*@param [in,out] *temp_ship Az adott hajora mutato pointer.
*@param [in,out] *game_assets A jatekhoz szukseges assetek taroloja.
*@return void
*/

static void fire_ship_torpedo_if_selected(int *ship_number,
                                   int *selector,
                                   EnemyShip *temp_ship,
                                   GameAssets *game_assets)
{
    if((*ship_number) % (*selector) == 0)
    {
        game_assets->enemy_torpedoes = add_torpedo_shot(game_assets->enemy_torpedoes, 5, 2, temp_ship->x_coor, temp_ship->y_coor, true);
    }
}

/**
*@brief increment_ship_number_if_ships_in_same_column
*@details Egyel emeli a hajo szamat, ha az oszthato 5-tel.
*@param [in,out] *ship_number A kivalasztott hajo szama.
*@return void
*/

static void increment_ship_number_if_ships_in_same_column(int *ship_number)
{
    bool is_ship_num_divisible_by_5 = (*ship_number) % 5 == 0;
    if(is_ship_num_divisible_by_5)
    {
        increment_ship_number(ship_number);
    }
}
/**
*@brief increment_ship_number
*@details Egyel emeli a hajo szamat.
*@param [out] *ship_number A kivalasztott hajo szama.
*@return void
*/
static void increment_ship_number(int *ship_number)
{
    (*ship_number)++;
}
