#include "torpedo_hit_management.h"

static void remove_only_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo);

static void remove_first_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo);

static void remove_in_between_or_last_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo);

/**
*@brief is_torpedo_out_of_bounds
*@details Erzekeli, ha a torpedo kilepett a jatekterbol.
*@param [in] torpedo A torpedokat tartalmazo lancolt lista adott eleme.
*@param [in] game_attributes A jatek attributumait tartalmazo adatszerkezet pointere.
*@return bool igaz erteket ad, ha az adott torpedo kilepett a jatekterbol, egyebkent hamis erteket ad.
*/

bool is_torpedo_out_of_bounds(TorpedoShot **torpedo, GameAttributes *game_attributes)
{
    bool is_torpedo_over_max_game_area = (*torpedo)->y_coor > game_attributes->height + 10;
    bool is_torpedo_under_min_game_area = (*torpedo)->y_coor < -10;
    bool is_torpedo_out_of_bounds = is_torpedo_over_max_game_area || is_torpedo_under_min_game_area;

    return is_torpedo_out_of_bounds;
}

/**
*@brief explode_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat es kezeli az ahhoz tartozo pointereket.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
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
*@brief remove_only_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista egyeduli eleme.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_only_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = NULL;
    (*temp_torpedo) = NULL;
}

/**
*@brief remove_first_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojatn ha az a torpedo lista head eleme.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_first_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*temp_torpedo) = (*player_torpedo);
}

/**
*@brief remove_in_between_or_last_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista köztes vagy tail eleme.
*@param [in,out] player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in,out] temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista head pointerenek ideiglenes taroloja.
*@return void
*/

static void remove_in_between_or_last_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo)
{
    pop_torpedo_shot(player_torpedo);
    (*player_torpedo) = (*temp_torpedo);
}
