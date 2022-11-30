/**
*@file hit_management.c
*/

#include "hit_management.h"


/**
*@brief is_torpedo_in_ship_hitbox
*@details ellenorzi, hogy az adott torpedo az adott hajo hitboxan belul helyezkedik-e el (van-e talalat)
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

bool is_torpedo_in_ship_hitbox(EnemyShip **enemy_armada, TorpedoShot **player_torpedo)
{
    bool is_torp_hor_pos_past_ship_hitbox_beg = (*player_torpedo)->x_coor > (*enemy_armada)->hitbox_beg_coor;
    bool is_torp_hor_pos_before_ship_hitbox_end = (*player_torpedo)->x_coor < (*enemy_armada)->hitbox_end_coor;

    bool is_torp_vert_pos_past_ship_centerline = (*player_torpedo)->y_coor < (*enemy_armada)->centerline_y_coor;
    bool is_torp_vert_pos_before_ship_vert_hitbox_end = (*player_torpedo)->y_coor > (*enemy_armada)->centerline_y_coor-30;

    bool is_torp_hor_pos_in_ship_hitbox = is_torp_hor_pos_past_ship_hitbox_beg && is_torp_hor_pos_before_ship_hitbox_end;
    bool is_torp_vert_pos_in_ship_hitbox = is_torp_vert_pos_past_ship_centerline && is_torp_vert_pos_before_ship_vert_hitbox_end;

    bool is_torpedo_in_ship_hitbox = is_torp_hor_pos_in_ship_hitbox && is_torp_vert_pos_in_ship_hitbox;

    return is_torpedo_in_ship_hitbox;
}


/**
*@brief manage_player_hits
*@details menedzseli a jatekos altal bevitt torpedo talalatokat (csokkenti az ellenseges HP-t)
*@param [] torpedo
*@param [] game_assets
*@return void
*/

void manage_player_hits(EnemyShip **enemy_armada, TorpedoShot **player_torpedo, GameAssets game_assets)
{
    EnemyShip *temp_ship = (*enemy_armada);
    TorpedoShot *temp_torpedo = (*player_torpedo);
    ///TODO fix this, move it to its own function, this is awful
    remove_torpedo_if_out_of_bounds(player_torpedo, &temp_torpedo, &game_assets);
    while(((*enemy_armada) != NULL) && ((*player_torpedo) != NULL))
    {
        while((*player_torpedo) != NULL)
        {
            explode_enemy_ship_and_torpedo_if_hit_detected(enemy_armada, &temp_ship, player_torpedo, &temp_torpedo);
            advance_torpedo_list_if_not_NULL(player_torpedo);
        }
        reset_player_torpedo_pointer(player_torpedo, &temp_torpedo);
        advance_enemy_ship_list(enemy_armada);
    }
    reset_enemy_armada_pointer(enemy_armada, &temp_ship);
}

/**
*@brief explode_enemy_ship_and_torpedo_if_hit_detected
*@details torli a talalatot kapott ellenseges hajot es az azt eltalalo torpedot, menedzseli azok pointereit
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void explode_enemy_ship_and_torpedo_if_hit_detected(EnemyShip **enemy_armada,
                                                    EnemyShip **temp_ship,
                                                    TorpedoShot **player_torpedo,
                                                    TorpedoShot **temp_torpedo)
{
    if(is_torpedo_in_ship_hitbox(enemy_armada, player_torpedo))
    {
        explode_enemy_ship(enemy_armada, temp_ship);
        explode_torpedo(player_torpedo, temp_torpedo);
    }
}

/**
*@brief reset_enemy_armada_pointer
*@details visszaallitja az enemy_armada lista head pointeret.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void reset_enemy_armada_pointer(EnemyShip **enemy_armada, EnemyShip **temp_ship)
{
    (*enemy_armada) = (*temp_ship);
}

/**
*@brief reset_player_torpedo_pointer
*@details visszaallitja a player_torpedo lista head pointeret.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void reset_player_torpedo_pointer(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    (*player_torpedo) = (*temp_torpedo);
}

/**
*@brief advance_enemy_ship_list_if_not_NULL
*@details lepteti az ellenseges hajok listajat, ha az nem NULL.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void advance_enemy_ship_list_if_not_NULL(EnemyShip **enemy_ship)
{
    bool is_enemy_ship_NULL = (*enemy_ship) != NULL;

    if(is_enemy_ship_NULL)
    {
        advance_enemy_ship_list(enemy_ship);
    }
}

/**
*@brief advance_enemy_ship_list
*@details lepteti a ellenseges hajok listajat.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void advance_enemy_ship_list(EnemyShip **enemy_ship)
{
    (*enemy_ship) = (*enemy_ship)->next_ship;
}


/**
*@brief advance_torpedo_list_if_not_NULL
*@details lepteti a jatekos torpedoinak listajat, ha az nem NULL.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void advance_torpedo_list_if_not_NULL(TorpedoShot **player_torpedo)
{
    bool is_torpedo_NULL = (*player_torpedo) != NULL;

    if (is_torpedo_NULL)
    {
        advance_torpedo_list(player_torpedo);
    }
}

/**
*@brief advance_torpedo_list
*@details lepteti a jatekos torpedoinak listajat.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void advance_torpedo_list(TorpedoShot **player_torpedo)
{
    (*player_torpedo) = (*player_torpedo)->next_torpedo;
}

/**
*@brief explode_enemy_ship
*@details felszabaditja a felrobbant ellenséges hajót es kezeli az ahhoz tartozo pointereket.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void explode_enemy_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
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
}

/**
*@brief explode_torpedo
*@details felszabaditja a jatekos felrobbant torpedojat es kezeli az ahhoz tartozo pointereket.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void explode_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    bool is_first_torpedo = (*player_torpedo)->prev_torpedo == NULL;
    bool is_last_torpedo = (*player_torpedo)->next_torpedo == NULL;
    bool is_only_torpedo = is_first_torpedo && is_last_torpedo;
    bool is_in_between_torpedo = !is_first_torpedo && !is_last_torpedo;

    if(is_only_torpedo)
    {
        remove_only_torpedo(player_torpedo, temp_torpedo);
    }
    if(is_first_torpedo)
    {
        remove_first_torpedo(player_torpedo, temp_torpedo);
    }
    if(is_in_between_torpedo || is_last_torpedo)
    {
        remove_in_between_or_last_torpedo(player_torpedo, temp_torpedo);
    }
}

/**
*@brief remove_only_ship
*@details felszabaditja a felrobbant ellenséges hajot ha, az a hajo lista egyeduli eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_only_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*enemy_ship) = NULL;
    (*temp_ship) = NULL;
}

/**
*@brief remove_first_ship
*@details felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista head eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_first_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*temp_ship) = (*enemy_ship);
}

/**
*@brief remove_in_between_or_last_ship
*@details felszabaditja a felrobbant ellenséges hajot ha az a hajo lista köztes vagy tail eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_in_between_or_last_ship(EnemyShip **enemy_ship, EnemyShip **temp_ship)
{
    pop_enemy_ship(enemy_ship);
    (*enemy_ship) = (*temp_ship);
}

/**
*@brief remove_only_torpedo
*@details felszabaditja a jatekos felrobbant torpedojat ha az a torpedo lista egyeduli eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_only_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = NULL;
    (*temp_torpedo) = NULL;
}

/**
*@brief remove_first_torpedo
*@details felszabaditja a jatekos felrobbant torpedojat ha az a torpedo lista head eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_first_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*temp_torpedo) = (*player_torpedo);
}

/**
*@brief remove_in_between_or_last_torpedo
*@details felszabaditja a jatekos felrobbant torpedojat ha az a torpedo lista köztes vagy tail eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_in_between_or_last_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = (*temp_torpedo);
}

/**
*@brief is_torpedo_out_of_bounds
*@details erzekeli, ha a torpedo kilepett a jatekterbol
*@param [] torpedo
*@param [] game_assets
*@return bool
*/

bool is_torpedo_out_of_bounds(TorpedoShot **torpedo, GameAttributes *game_attributes)
{
    bool is_torpedo_over_max_game_area = (*torpedo)->y_coor > game_attributes->height + 10;
    bool is_torpedo_under_min_game_area = (*torpedo)->y_coor < -10;
    bool is_torpedo_out_of_bounds = is_torpedo_over_max_game_area || is_torpedo_under_min_game_area;

    return is_torpedo_out_of_bounds;
}

/**
*@brief remove_torpedo_if_out_of_bounds
*@details kitorli a torpedot, ha az elhagyta a jatekteret
*@param [] torpedo
*@param [] game_assets
*@return void
*/

void remove_torpedo_if_out_of_bounds(TorpedoShot **torpedo, TorpedoShot **temp_torpedo, GameAttributes *game_attributes)
{
    if(is_torpedo_out_of_bounds(torpedo, game_attributes))
    {
        explode_torpedo(torpedo, temp_torpedo);
    }
}

/**
*@brief detect_player_hit
*@details ellenorzi, hogy barmelyik ellenseges hajo talalatot kapott-e.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/
void detect_enemy_hit(EnemyShip **enemy_armada, TorpedoShot **torpedo)
{

}

/**
*@brief detect_enemy_hit
*@details detektalja, ha az ellenseg eltalalta a jatekos hajojat
*@param [] game_assets
*@return bool
*/
bool static detect_player_hit(GameAssets *game_assets)
{

}


/**
*@brief manage_enemy_hits
*@details menedzseli az ellenseges talalatokat, amelyeket a jatekos hajoja kapott
*@param [] torpedo
*@param [] game_assets
*@return void
*/
void static manage_enemy_hits(TorpedoShot *torpedo, GameAssets *game_assets)
{

}

/**
*@brief manage_hits
*@details aggregalo fuggveny a hit management logikahoz
*@param [] game_assets
*@param [] game_attributes
*@return void
*//*
void manage_hits(GameAssets *game_assets, GameAttributes *game_attributes){
    detect_player_hit(game_assets, game_attributes);
}
*/
