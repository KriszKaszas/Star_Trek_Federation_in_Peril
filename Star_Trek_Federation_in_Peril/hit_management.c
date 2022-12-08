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

static void manage_player_ship_and_torpedo_if_hit_detected(PlayerShip **player_ship,
                                                           TorpedoShot **enemy_torpedo,
                                                           TorpedoShot **temp_torpedo,
                                                           GameAssets **game_assets);

///-----------------------------------------Torpedo hitbox check------------------------------------------

static bool is_player_torpedo_in_enemy_ship_hitbox(EnemyShip **enemy_armada, TorpedoShot **player_torpedo);

static bool is_enemy_torpedo_in_player_ship_hitbox(PlayerShip **player_ship, TorpedoShot **enemy_torpedo);

///------------------Ship and torpedo management if a hit is detected - helper functions------------------

static void manage_enemy_ship_health(EnemyShip **enemy_ship, TorpedoShot **player_torpedo);

static void manage_player_ship_health(PlayerShip **player_ship, TorpedoShot **enemy_torpedo);

///-------------Pointer management for linked lists after search and removal - main managers--------------

static void reset_enemy_armada_pointer(EnemyShip **enemy_armada, EnemyShip **temp_ship);

static void reset_torpedo_pointer(TorpedoShot **torpedo, TorpedoShot **temp_torpedo);

static void advance_enemy_ship_list_if_not_NULL(EnemyShip **enemy_ship);

static void advance_torpedo_list_if_not_NULL(TorpedoShot **player_torpedo);

static void nullify_enemy_armada_in_game_assets_if_no_ships_left(EnemyShip **enemy_armada, GameAssets **game_assets);

static void nullify_enemy_armada_in_game_assets(EnemyShip **enemy_armada, GameAssets **game_assets);

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

void remove_torpedo_if_out_of_bounds(TorpedoShot **torpedo, GameAttributes *game_attributes)
{
    TorpedoShot *temp_torpedo = (*torpedo);
    while((*torpedo) != NULL)
    {
        if(is_torpedo_out_of_bounds(torpedo, game_attributes))
        {
            explode_torpedo(torpedo, &temp_torpedo);
        }
        advance_torpedo_list_if_not_NULL(torpedo);
    }
    reset_torpedo_pointer(torpedo, &temp_torpedo);
}

/**
*@brief manage_enemy_hits
*@details Menedzseli a jatekos altal bevitt torpedo talalatokat.
*@param [in,out] enemy_armada Az osszes ellenseges hajot tartalmazo lancolt lista head pointere.
*@param [in,out] player_torpedoes A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointere.
*@return void
*/

void manage_enemy_hits(EnemyShip **enemy_armada, TorpedoShot **player_torpedoes, GameAssets **game_assets, GameAttributes *game_attributes)
{
    EnemyShip *temp_ship = (*enemy_armada);
    TorpedoShot *temp_torpedo = (*player_torpedoes);
    //Vegigiteral az ellenseges hajokon, ha maradt ellenseges hajo es van kilott jatekos torpedo.
    while(((*enemy_armada) != NULL) && ((*player_torpedoes) != NULL))
    {
        //Vegigiteral a jatekos torpedoin, ha van kilott jatekos torpedo es maradt ellenséges hajo.
        while(((*player_torpedoes) != NULL) && ((*enemy_armada) != NULL))
        {
            manage_enemy_ship_and_torpedo_if_hit_detected(enemy_armada, &temp_ship, player_torpedoes, &temp_torpedo, game_attributes);
            advance_torpedo_list_if_not_NULL(player_torpedoes);
        }
        reset_torpedo_pointer(player_torpedoes, &temp_torpedo);
        advance_enemy_ship_list_if_not_NULL(enemy_armada);
    }
    reset_enemy_armada_pointer(enemy_armada, &temp_ship);
    nullify_enemy_armada_in_game_assets_if_no_ships_left(enemy_armada, game_assets);
}

/**
*@brief manage_player_hits
*@details Menedzseli a jatekos altal bevitt torpedo talalatokat.
*@param [in,out] enemy_armada Az osszes ellenseges hajot tartalmazo lancolt lista head pointere.
*@param [in,out] player_torpedoes A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointere.
*@return void
*/

void manage_player_hits(PlayerShip **player_ship, TorpedoShot **enemy_torpedoes, GameAssets **game_assets)
{
    TorpedoShot *temp_torpedo = (*enemy_torpedoes);
    while(((*player_ship) != NULL) && ((*enemy_torpedoes) != NULL))
    {
        manage_player_ship_and_torpedo_if_hit_detected(player_ship, enemy_torpedoes, &temp_torpedo, game_assets);
        advance_torpedo_list_if_not_NULL(enemy_torpedoes);
    }
    reset_torpedo_pointer(enemy_torpedoes, &temp_torpedo);
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
    if(is_player_torpedo_in_enemy_ship_hitbox(enemy_ship, player_torpedo))
    {
        manage_enemy_ship_health(enemy_ship, player_torpedo);
        explode_enemy_ship_if_dead(enemy_ship, temp_ship, game_attributes);
        explode_torpedo(player_torpedo, temp_torpedo);
    }
}

/**
*@brief explode_enemy_ship_and_torpedo_if_hit_detected
*@details torli a talalatot kapott ellenseges hajot es az azt eltalalo torpedot, menedzseli azok pointereit
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_ship Az ellenseges hajot tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void manage_player_ship_and_torpedo_if_hit_detected(PlayerShip **player_ship,
                                                    TorpedoShot **enemy_torpedo,
                                                    TorpedoShot **temp_torpedo,
                                                    GameAssets **game_assets)
{
    if(is_enemy_torpedo_in_player_ship_hitbox(player_ship, enemy_torpedo))
    {

        manage_player_ship_health(player_ship, enemy_torpedo);
        explode_player_ship_if_dead(player_ship, game_assets);
        explode_torpedo(enemy_torpedo, temp_torpedo);
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

static bool is_player_torpedo_in_enemy_ship_hitbox(EnemyShip **enemy_ship, TorpedoShot **player_torpedo)
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

/**
*@brief is_torpedo_in_ship_hitbox
*@details Ellenorzi, hogy az adott torpedo az adott hajo hitboxan belul helyezkedik-e el (van-e talalat).
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return bool Igaz erteket ad, ha a torpedo a hajo hitboxan belul van, egyebkent hamis erteket ad.
*/

static bool is_enemy_torpedo_in_player_ship_hitbox(PlayerShip **player_ship, TorpedoShot **enemy_torpedo)
{
    bool is_torp_hor_pos_past_ship_hitbox_beg = (*enemy_torpedo)->x_coor > (*player_ship)->hitbox_beg_coor;
    bool is_torp_hor_pos_before_ship_hitbox_end = (*enemy_torpedo)->x_coor < (*player_ship)->hitbox_end_coor;

    bool is_torp_vert_pos_past_ship_centerline = (*enemy_torpedo)->y_coor > (*player_ship)->centerline_y_coor;
    bool is_torp_vert_pos_before_ship_vert_hitbox_end = (*enemy_torpedo)->y_coor < (*player_ship)->centerline_y_coor+50;

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

/**
*@brief decrement_enemy_ship_health
*@details Csokkenti az ellenseges hajo HP-jat az adott fegyver sebzesi erejevel, ha az talalatot kap
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@return void
*/

static void manage_player_ship_health(PlayerShip **player_ship, TorpedoShot **enemy_torpedo)
{
    (*player_ship)->health -= (*enemy_torpedo)->damage;
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

static void reset_torpedo_pointer(TorpedoShot **torpedo, TorpedoShot **temp_torpedo)
{
    (*torpedo) = (*temp_torpedo);
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

static void advance_torpedo_list_if_not_NULL(TorpedoShot **torpedo)
{
    bool is_torpedo_NULL = (*torpedo) != NULL;

    if (is_torpedo_NULL)
    {
        advance_torpedo_list(torpedo);
    }
}

/**
*@brief nullify_enemy_armada_in_game_assets_if_no_ships_left
*@details NULL-ra allitja a game_assets eredeti, az ellenseges hajokra mutato pointeret, amennyiben az osszes hajo megsemmisult.
*@param [in,out] **enemy_armada az ellenseges hajokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] **game_assets A jatek-assetek kulso taroloja.
*@return void
*/

static void nullify_enemy_armada_in_game_assets_if_no_ships_left(EnemyShip **enemy_armada, GameAssets **game_assets)
{
    bool are_all_ships_dead = (*enemy_armada) == NULL;
    if(are_all_ships_dead)
    {
        nullify_enemy_armada_in_game_assets(enemy_armada, game_assets);
    }
}

/**
*@brief advance_torpedo_list_if_not_NULL
*@details NULL-ra allitja a game_assets eredeti, az ellenseges hajokra mutato pointeret.
*@param [in,out] **enemy_armada az ellenseges hajokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] **game_assets A jatek-assetek kulso taroloja.
*@return void
*/

static void nullify_enemy_armada_in_game_assets(EnemyShip **enemy_armada, GameAssets **game_assets)
{
    (*game_assets)->enemy_armada = NULL;
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

static void advance_torpedo_list(TorpedoShot **torpedo)
{
    (*torpedo) = (*torpedo)->next_torpedo;
}


