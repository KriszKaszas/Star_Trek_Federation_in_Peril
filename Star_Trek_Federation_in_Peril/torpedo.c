/**
*@file torpedo.c
*/

#include "torpedo.h"

/**
*@brief init_torpedo_colors
*@details inicializalja a torpedok szineit ado TorpedoColors structot
*@param [] is_enemy_torpedo
*@param [] is_quantum_torpedo
*@return TorpedoColors
*/
TorpedoColors init_torpedo_colors(bool is_enemy_torpedo, bool is_quantum_torpedo){
    TorpedoColors colors;
    if(is_enemy_torpedo){
        ColorData outter_ring = {94, 195, 79, 100};
        colors.outter_ring = outter_ring;
        ColorData inner_ring = {62, 250, 85, 155};
        colors.inner_ring = inner_ring;
        ColorData center = {138, 254, 202, 255};
        colors.center = center;
    }
    else if(is_quantum_torpedo){
        ColorData outter_ring = {94, 195, 79, 100};
        colors.outter_ring = outter_ring;
        ColorData inner_ring = {65, 123, 237, 155};
        colors.inner_ring = inner_ring;
        ColorData center = {255, 255, 255, 255};
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
TorpedoShot *add_torpedo_shot(TorpedoShot *torpedoes, int damage, int speed, int x_coor, int y_coor, bool is_enemy_torpedo, bool is_quantum_torpedo){

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
    torpedo->colors = init_torpedo_colors(is_enemy_torpedo, is_quantum_torpedo);
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
void move_torpedoes(TorpedoShot **torpedo, GameAttributes *game_attributes){
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
    TorpedoShot *tmp = (*torpedo);
    if(tmp != NULL)
    {
        if((*torpedo)->prev_torpedo == NULL && (*torpedo)->next_torpedo == NULL)
        {
            (*torpedo) = NULL;
            free(tmp);
        }
        else if((*torpedo)->prev_torpedo == NULL)
        {
            (*torpedo) = tmp->next_torpedo;
            (*torpedo)->prev_torpedo = NULL;
            free(tmp);
        }
        else if((*torpedo)->next_torpedo == NULL)
        {
            (*torpedo)->prev_torpedo->next_torpedo = NULL;
            (*torpedo) = (*torpedo)->prev_torpedo;
            free(tmp);
        }
        else
        {
            (*torpedo)->prev_torpedo->next_torpedo = (*torpedo)->next_torpedo;
            (*torpedo)->next_torpedo->prev_torpedo = (*torpedo)->prev_torpedo;
            (*torpedo) = (*torpedo)->next_torpedo;
            free(tmp);
        }
    }
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
