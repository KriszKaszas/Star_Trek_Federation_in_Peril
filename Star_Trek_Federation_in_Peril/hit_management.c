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
*@details menedzseli a jatekos altal bevitt torpedo talalatokat (csokkenti a HP-t)
*@param [] torpedo
*@param [] game_assets
*@return void
*/

void manage_player_hits(EnemyShip **enemy_armada, TorpedoShot **player_torpedo, GameAssets game_assets)
{
    EnemyShip *es = (*enemy_armada);
    TorpedoShot *pt = (*player_torpedo);

    remove_torpedo_if_out_of_bounds(player_torpedo, &pt, &game_assets);
    while((*enemy_armada) != NULL)
    {
        while((*player_torpedo) != NULL)
        {
            if(is_torpedo_in_ship_hitbox(enemy_armada, player_torpedo))
            {
                explode_torpedo(player_torpedo, &pt);
                break;
            }
            (*player_torpedo) = (*player_torpedo)->next_torpedo;
        }
        (*player_torpedo) = pt;
        (*enemy_armada) = (*enemy_armada)->next_ship;
    }
    (*enemy_armada) = es;
}


/**
*@brief explode_torpedo
*@details felszabaditja a felrobbant torpedot es kezeli az ahhoz tartozo pointereket.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void explode_torpedo(TorpedoShot **player_torpedo, TorpedoShot **pt)
{
    bool is_first_torpedo = (*player_torpedo)->prev_torpedo == NULL;
    bool is_last_torpedo = (*player_torpedo)->next_torpedo == NULL;
    bool is_only_torpedo = is_first_torpedo && is_last_torpedo;
    bool is_in_between_torpedo = !is_first_torpedo && !is_last_torpedo;

    if(is_only_torpedo)
    {
        remove_only_torpedo(player_torpedo, pt);
    }
    if(is_first_torpedo)
    {
        remove_first_torpedo(player_torpedo, pt);
    }
    if(is_in_between_torpedo || is_last_torpedo)
    {
        remove_in_between_or_last_torpedo(player_torpedo, pt);S
    }
}

/*
    if((*player_torpedo)->prev_torpedo == NULL)
    {
        if((*player_torpedo)->next_torpedo == NULL)
        {
            pop_torpedo_shot(player_torpedo);
            (*player_torpedo) = NULL;
            (*pt) = NULL;
        }
        else
        {
            pop_torpedo_shot(player_torpedo);
            (*pt) = (*player_torpedo);
        }
    }
    else
    {
        pop_torpedo_shot(player_torpedo);
        (*player_torpedo) = (*pt);
    }
*/

/**
*@brief remove_only_torpedo
*@details felszabaditja a felrobbant torpedot ha az a torpedo lista egyeduli eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_only_torpedo(TorpedoShot **player_torpedo, TorpedoShot **pt)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = NULL;
    (*pt) = NULL;
}

/**
*@brief remove_first_torpedo
*@details felszabaditja a felrobbant torpedot ha az a torpedo lista head eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_first_torpedo(TorpedoShot **player_torpedo, TorpedoShot **pt)
{
    pop_torpedo_shot(player_torpedo);
    (*pt) = (*player_torpedo);
}

/**
*@brief remove_in_between_or_last_torpedo
*@details felszabaditja a felrobbant torpedot ha az a torpedo lista köztes vagy tail eleme.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/

void remove_in_between_or_last_torpedo(TorpedoShot **player_torpedo, TorpedoShot **pt)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = (*pt);
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
S
void remove_torpedo_if_out_of_bounds(TorpedoShot **torpedo, TorpedoShot **pt, GameAttributes *game_attributes)
{
    if(is_torpedo_out_of_bounds(torpedo, game_attributes))
    {
        explode_torpedo(torpedo, pt);
    }
}

//is_torpedo_out_of_bounds(torpedo, &game_attributes)
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
