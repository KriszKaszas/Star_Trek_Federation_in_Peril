/**
*@file torpedo.c
*/

#include "torpedo.h"

static void pop_only_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo);

static void pop_first_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo);

static void pop_last_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo);

static void pop_in_between_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo);

/**
*@brief init_torpedo_colors
*@details inicializalja a torpedok szineit ado TorpedoColors structot
*@param [] is_enemy_torpedo
*@param [] is_quantum_torpedo
*@return TorpedoColors
*/
TorpedoColors init_torpedo_colors(bool is_enemy_torpedo){
    TorpedoColors colors;
    if(is_enemy_torpedo){
        ColorData outter_ring = {94, 195, 79, 100};
        colors.outter_ring = outter_ring;
        ColorData inner_ring = {62, 250, 85, 155};
        colors.inner_ring = inner_ring;
        ColorData center = {138, 254, 202, 255};
        colors.center = center;
    }
    else{
        ColorData outter_ring = {246, 22, 0, 100};
        colors.outter_ring = outter_ring;
        ColorData inner_ring = {246, 22, 0, 155};
        colors.inner_ring = inner_ring;
        ColorData center = {254, 255, 69, 255};
        colors.center = center;
    }
    return colors;
}

/**
*@brief add_torpedo_shot
*@details hozzaad a kilott torpedok listajahoz egy ujabb elemet
*@param [] torpedoes
*@param [] damage
*@param [] speed
*@param [] x_coor
*@param [] y_coor
*@param [] is_enemy_torpedo
*@param [] is_quantum_torpedo
*@return TorpedoShot
*/
TorpedoShot *add_torpedo_shot(TorpedoShot *torpedoes, int damage, int speed, int x_coor, int y_coor, bool is_enemy_torpedo){

    TorpedoShot *torpedo = (TorpedoShot*) malloc(sizeof(TorpedoShot));
    torpedo->x_coor = x_coor;
    torpedo->y_coor = y_coor;
    torpedo->damage = damage;
    torpedo->speed = speed;
    if(!is_enemy_torpedo){
        torpedo->dir = -1;
    }
    else{
        torpedo->dir = 1;
    }
    torpedo->colors = init_torpedo_colors(is_enemy_torpedo);
    torpedo->next_torpedo = torpedoes;
    if(torpedoes != NULL){
        torpedoes->prev_torpedo = torpedo;
    }
    torpedo->prev_torpedo = NULL;
    return torpedo;
}

/**
*@brief move_torpedoes
*@details a torpedok mozgasahoz szukseges szamitasokat vegzi
*@param [] torpedo
*@param [] game_attributes
*@return void
*/
void move_torpedoes(TorpedoShot **torpedo){
    TorpedoShot *tmp = (*torpedo);
    while(tmp != NULL){
         tmp->y_coor += tmp->dir;
        tmp = tmp->next_torpedo;
    }
}

/**
*@brief pop_torpedo_shot
*@details amennyiben a torpedo eltalal valamit, vagy kimegy a jatekterbol, torli azt.
*@param [] torpedo
*@return void
*/
void pop_torpedo_shot(TorpedoShot **torpedo)
{
    TorpedoShot *temp_torpedo = (*torpedo);
    bool is_torpedo_null = (*torpedo) == NULL;

    if(is_torpedo_null)
    {
        return;
    }

    bool is_first_torpedo = (*torpedo)->prev_torpedo == NULL;
    bool is_last_torpedo = (*torpedo)->next_torpedo == NULL;
    bool is_only_torpedo = is_first_torpedo && is_last_torpedo;
    bool is_in_between_torpedo = !is_first_torpedo && !is_last_torpedo;

    if(is_only_torpedo)
    {
        pop_only_torpedo(torpedo, temp_torpedo);
        return;
    }
    if(is_first_torpedo)
    {
        pop_first_torpedo(torpedo, temp_torpedo);
        return;
    }
    if(is_last_torpedo)
    {
        pop_last_torpedo(torpedo, temp_torpedo);
        return;
    }
    if(is_in_between_torpedo)
    {
        pop_in_between_torpedo(torpedo, temp_torpedo);
        return;
    }
}

/**
*@brief pop_only_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista egyeduli eleme.
*@param [in,out] **player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] *temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt aktualis elemere mutato pointer ideiglenes taroloja.
*@return void
*/

static void pop_only_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo)
{
    (*torpedo) = NULL;
    free(temp_torpedo);
}

/**
*@brief pop_first_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista egyeduli eleme.
*@param [in,out] **player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] *temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt aktualis elemere mutato pointer ideiglenes taroloja.
*@return void
*/

static void pop_first_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo)
{
    (*torpedo) = temp_torpedo->next_torpedo;
    (*torpedo)->prev_torpedo = NULL;
    free(temp_torpedo);
}

/**
*@brief pop_last_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista tail eleme.
*@param [in,out] **player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] *temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt aktualis elemere mutato pointer ideiglenes taroloja.
*@return void
*/

static void pop_last_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo)
{
    (*torpedo)->prev_torpedo->next_torpedo = NULL;
    (*torpedo) = (*torpedo)->prev_torpedo;
    free(temp_torpedo);
}

/**
*@brief pop_in_between_torpedo
*@details Felszabaditja a jatekos felrobbant torpedojat, ha az a torpedo lista koztes eleme.
*@param [in,out] **player_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] *temp_torpedo A jatekos altal kilott torpedokat tartalmazo lancolt aktualis elemere mutato pointer ideiglenes taroloja.
*@return void
*/

static void pop_in_between_torpedo(TorpedoShot **torpedo, TorpedoShot *temp_torpedo)
{
    (*torpedo)->prev_torpedo->next_torpedo = (*torpedo)->next_torpedo;
    (*torpedo)->next_torpedo->prev_torpedo = (*torpedo)->prev_torpedo;
    (*torpedo) = (*torpedo)->next_torpedo;
    free(temp_torpedo);
}

/**
*@brief free_torpedoes
*@details felszabaditja a torpedok listajat
*@param [] torpedoes
*@return void
*/
void free_torpedoes(TorpedoShot *torpedoes){
    TorpedoShot *tmp = torpedoes;
    while(tmp != NULL){
        tmp = torpedoes->next_torpedo;
        pop_torpedo_shot(&torpedoes);
        torpedoes = tmp;
    }
    torpedoes = NULL;
    tmp = NULL;
}
