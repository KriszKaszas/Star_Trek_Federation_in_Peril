/**
*@file enemy_hit_management.c
*/

#include "enemy_hit_management.h"


static void explode_enemy_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship, GameAttributes *game_attributes);

static void remove_only_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship);

static void remove_first_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship);

static void remove_in_between_or_last_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship);

static void manage_score_on_enemy_death(EnemyShip **enemy_ship, GameAttributes *game_attributes);

static void decrement_enemy_armada_size(GameAttributes *game_attributes);

/**
*@brief explode_enemy_ship_if_dead
*@details Felszabaditja a felrobbant ellenséges hajót ha annak HP-ja 0 (vagy kevesebb), es kezeli az ahhoz tartozo pointereket.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

void explode_enemy_ship_if_dead(EnemyShip **enemy_ship, EnemyShip **temp_ship, GameAttributes *game_attributes)
{
    bool is_enemy_dead = (*enemy_ship)->health <= 0;
    if(is_enemy_dead)
    {
        manage_score_on_enemy_death(enemy_ship, game_attributes);
        explode_enemy_ship(enemy_ship, temp_ship, game_attributes);
    }
}


/**
*@brief explode_enemy_ship
*@details Felszabaditja a felrobbant ellenséges hajót es kezeli az ahhoz tartozo pointereket.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void explode_enemy_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship, GameAttributes *game_attributes)
{
    bool is_ship_null = (*enemy_ship) == NULL;

    if(is_ship_null)
    {
        return;
    }

    bool is_first_ship = (*enemy_ship)->prev_ship == NULL;
    bool is_last_ship = (*enemy_ship)->next_ship == NULL;
    bool is_only_ship = is_first_ship && is_last_ship;
    bool is_in_between_ship = !is_first_ship && !is_last_ship;

    if(is_only_ship)
    {
        remove_only_ship(enemy_ship, temp_ship);
    }
    if(is_first_ship)
    {
        remove_first_ship(enemy_ship, temp_ship);
    }
    if(is_in_between_ship || is_last_ship)
    {
        remove_in_between_or_last_ship(enemy_ship, temp_ship);
    }
    decrement_enemy_armada_size(game_attributes);
}

/**
*@brief remove_only_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista egyeduli eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_only_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*enemy_ship) = NULL;
    (*temp_ship) = NULL;
}

/**
*@brief remove_first_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista head eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_first_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*temp_ship) = (*enemy_ship);
}

/**
*@brief remove_in_between_or_last_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista köztes vagy tail eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_in_between_or_last_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*enemy_ship) = (*temp_ship);
}





static void manage_score_on_enemy_death(EnemyShip **enemy_ship, GameAttributes *game_attributes)
{
    game_attributes->game_score += (*enemy_ship)->score_value;
}

/**
*@brief decrement_enemy_armada_size
*@details Csokkenti a game attributes taroloban az ellenseges hajoflotta szamat jelolo valtozot
*@param [in,out] game_attributes A jatek attributumainak a taroloja.
*@return void
*/

static void decrement_enemy_armada_size(GameAttributes *game_attributes)
{
    game_attributes->enemy_armada_size--;
}
