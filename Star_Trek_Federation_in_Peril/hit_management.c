/**
*@file hit_management.c
*/

#include "hit_management.h"

///--------------------Ship and torpedo management if a hit is detected - main manager--------------------

static void manage_enemy_ship_and_torpedo_if_hit_detected(EnemyShip **enemy_armada,
                                                          EnemyShip **temp_ship,
                                                          TorpedoShot **player_torpedo,
                                                          TorpedoShot **temp_torpedo,
                                                          GameAttributes *game_attributes);

///-----------------------------------------Torpedo hitbox check------------------------------------------

static bool is_torpedo_in_ship_hitbox(EnemyShip **enemy_armada, TorpedoShot **player_torpedo);

///------------------Ship and torpedo management if a hit is detected - helper functions------------------

static void manage_enemy_ship_health(EnemyShip **enemy_ship, TorpedoShot **player_torpedo);

///-------------Pointer management for linked lists after search and removal - main managers--------------

static void reset_enemy_armada_pointer(EnemyShip **enemy_armada, EnemyShip **temp_ship);

static void reset_player_torpedo_pointer(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo);

static void advance_enemy_ship_list_if_not_NULL(EnemyShip **enemy_ship);

static void advance_torpedo_list_if_not_NULL(TorpedoShot **player_torpedo);

///-------------Pointer management for linked lists after search and removal  - helper functions----------

static void advance_enemy_ship_list(EnemyShip **enemy_ship);

static void advance_torpedo_list(TorpedoShot **player_torpedo);




///-----------------------------------Externally callable hit managers------------------------------------

/**
*@brief remove_torpedo_if_out_of_bounds
*@details Kitorli a torpedot, ha az elhagyta a jatekteret.
*@param [in,out] player_torpedoes A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointere.
*@param [in] game_attributes A jatek attributumait tartalmazo adatszerkezet pointere.
*@return void
*/

void remove_torpedo_if_out_of_bounds(TorpedoShot **player_torpedoes, GameAttributes *game_attributes)
{
    TorpedoShot *temp_torpedo = (*player_torpedoes);
    while((*player_torpedoes) != NULL)
    {
        if(is_torpedo_out_of_bounds(player_torpedoes, game_attributes))
        {
            explode_torpedo(player_torpedoes, &temp_torpedo);
        }
        advance_torpedo_list_if_not_NULL(player_torpedoes);
    }
    reset_player_torpedo_pointer(player_torpedoes, &temp_torpedo);
}

/**
*@brief manage_player_hits
*@details Menedzseli a jatekos altal bevitt torpedo talalatokat.
*@param [in,out] enemy_armada Az osszes ellenseges hajot tartalmazo lancolt lista head pointere.
*@param [in,out] player_torpedoes A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointere.
*@return void
*/

void manage_player_hits(EnemyShip **enemy_armada, TorpedoShot **player_torpedoes, GameAttributes *game_attributes)
{
    EnemyShip *temp_ship = (*enemy_armada);
    TorpedoShot *temp_torpedo = (*player_torpedoes);
    while(((*enemy_armada) != NULL) && ((*player_torpedoes) != NULL))
    {
        while(((*player_torpedoes) != NULL) && ((*enemy_armada) != NULL))
        {
            manage_enemy_ship_and_torpedo_if_hit_detected(enemy_armada, &temp_ship, player_torpedoes, &temp_torpedo, game_attributes);
            advance_torpedo_list_if_not_NULL(player_torpedoes);
        }
        reset_player_torpedo_pointer(player_torpedoes, &temp_torpedo);
        advance_enemy_ship_list_if_not_NULL(enemy_armada);
    }
    reset_enemy_armada_pointer(enemy_armada, &temp_ship);
}


///--------------------Ship and torpedo management if a hit is detected - main manager--------------------


/**
*@brief explode_enemy_ship_and_torpedo_if_hit_detected
*@details torli a talalatot kapott ellenseges hajot es az azt eltalalo torpedot, menedzseli azok pointereit
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void manage_enemy_ship_and_torpedo_if_hit_detected(EnemyShip **enemy_ship,
                                                    EnemyShip **temp_ship,
                                                    TorpedoShot **player_torpedo,
                                                    TorpedoShot **temp_torpedo,
                                                    GameAttributes *game_attributes)
{
    if(is_torpedo_in_ship_hitbox(enemy_ship, player_torpedo))
    {
        manage_enemy_ship_health(enemy_ship, player_torpedo);
        explode_enemy_ship_if_dead(enemy_ship, temp_ship, game_attributes);
        explode_torpedo(player_torpedo, temp_torpedo);
    }
}


///--------------------------------Torpedo hitbox and out of bounds checks--------------------------------


/**
*@brief is_torpedo_in_ship_hitbox
*@details Ellenorzi, hogy az adott torpedo az adott hajo hitboxan belul helyezkedik-e el (van-e talalat).
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return bool Igaz erteket ad, ha a torpedo a hajo hitboxan belul van, egyebkent hamis erteket ad.
*/

static bool is_torpedo_in_ship_hitbox(EnemyShip **enemy_ship, TorpedoShot **player_torpedo)
{
    bool is_torp_hor_pos_past_ship_hitbox_beg = (*player_torpedo)->x_coor > (*enemy_ship)->hitbox_beg_coor;
    bool is_torp_hor_pos_before_ship_hitbox_end = (*player_torpedo)->x_coor < (*enemy_ship)->hitbox_end_coor;

    bool is_torp_vert_pos_past_ship_centerline = (*player_torpedo)->y_coor < (*enemy_ship)->centerline_y_coor;
    bool is_torp_vert_pos_before_ship_vert_hitbox_end = (*player_torpedo)->y_coor > (*enemy_ship)->centerline_y_coor-30;

    bool is_torp_hor_pos_in_ship_hitbox = is_torp_hor_pos_past_ship_hitbox_beg && is_torp_hor_pos_before_ship_hitbox_end;
    bool is_torp_vert_pos_in_ship_hitbox = is_torp_vert_pos_past_ship_centerline && is_torp_vert_pos_before_ship_vert_hitbox_end;

    bool is_torpedo_in_ship_hitbox = is_torp_hor_pos_in_ship_hitbox && is_torp_vert_pos_in_ship_hitbox;

    return is_torpedo_in_ship_hitbox;
}


///------------------Ship and torpedo management if a hit is detected - helper functions------------------


/**
*@brief decrement_enemy_ship_health
*@details Csokkenti az ellenseges hajo HP-jat az adott fegyver sebzesi erejevel, ha az talalatot kap
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void manage_enemy_ship_health(EnemyShip **enemy_ship, TorpedoShot **player_torpedo)
{
    (*enemy_ship)->health -= (*player_torpedo)->damage;
}


///-------------Pointer management for linked lists after search and removal - main managers--------------


/**
*@brief reset_enemy_armada_pointer
*@details Visszaallitja az enemy_armada lista head pointeret.
*@param [in,out] enemy_armada Az osszes ellenseges hajot tartalmazo lancolt lista pointere.
*@param [in] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void reset_enemy_armada_pointer(EnemyShip **enemy_armada, EnemyShip **temp_ship)
{
    (*enemy_armada) = (*temp_ship);
}

/**
*@brief reset_player_torpedo_pointer
*@details Visszaallitja a player_torpedo lista head pointeret.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void reset_player_torpedo_pointer(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    (*player_torpedo) = (*temp_torpedo);
}

/**
*@brief advance_enemy_ship_list_if_not_NULL
*@details Lepteti az ellenseges hajok listajat, ha az nem NULL.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void advance_enemy_ship_list_if_not_NULL(EnemyShip **enemy_ship)
{
    bool is_enemy_ship_NULL = (*enemy_ship) != NULL;

    if(is_enemy_ship_NULL)
    {
        advance_enemy_ship_list(enemy_ship);
    }
}

/**
*@brief advance_torpedo_list_if_not_NULL
*@details lepteti a jatekos torpedoinak listajat, ha az nem NULL.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void advance_torpedo_list_if_not_NULL(TorpedoShot **player_torpedo)
{
    bool is_torpedo_NULL = (*player_torpedo) != NULL;

    if (is_torpedo_NULL)
    {
        advance_torpedo_list(player_torpedo);
    }
}


///-------------Pointer management for linked lists after search and removal  - helper functions-------------

/**
*@brief advance_enemy_ship_list
*@details Lepteti a az ellenseges hajok listajat.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void advance_enemy_ship_list(EnemyShip **enemy_ship)
{
    (*enemy_ship) = (*enemy_ship)->next_ship;
}

/**
*@brief advance_torpedo_list
*@details Lepteti a jatekos torpedoinak listajat.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void advance_torpedo_list(TorpedoShot **player_torpedo)
{
    (*player_torpedo) = (*player_torpedo)->next_torpedo;
}


