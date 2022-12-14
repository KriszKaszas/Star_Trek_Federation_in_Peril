/**
*@file player_ship.c
*/

#include "player_ship.h"

/**
*@brief init_player_ship
*@details Inicializalja a jatekos hajojat.
*@param [in] *game_attributes A jatek attributumainak taroloja.
*@param [in] *ship_dtt A hajo adatainak ideiglenes taroloja.
*@param [in] texture_data A textura megjelenitesehez szukseges adatok.
*@param [in] sprite_map_data A spritemap beolvasasahoz szukseges adatok.
*@return *ps A jatekos hajojanak pointere.
*/

PlayerShip *init_player_ship(GameAttributes *game_attributes,
                             ShipDTT *ship_dtt,
                             TextureData texture_data,
                             SpriteMapData sprite_map_data)
 {

    PlayerShip *ps = (PlayerShip*) malloc(sizeof(PlayerShip));

    ps->x_coor = game_attributes->width/2;
    ps->y_coor = game_attributes->height/4*3;
    ps->texture_data = texture_data;
    ps->sprite_map_data = sprite_map_data;
    ps->texture_data.texture_center_x = ps->x_coor - (ps->texture_data.width/2);
    ps->texture_data.texture_center_y = ps->y_coor - (ps->texture_data.height/2);
    ps->hitbox_beg_coor = ps->x_coor - (texture_data.width/2);
    ps->hitbox_end_coor = ps->x_coor + (texture_data.width/2);
    ps->centerline_y_coor = ps->y_coor;
    ps->speed = ship_dtt->speed;
    ps->health = ship_dtt->health;
    ps->score_value = ship_dtt->score_value;
    return ps;
}

/**
*@brief move_player_ship
*@details A jatekos hajojanak mozgasaert felelos szamitasokat vegzi.
*@param [in,out] *ps A jatekos hajojanak pointere.
*@param [in] *isi A jatek belso allapotainak taroloja.
*@param [in] width Jatekablak szelessege.
*@param [in] height Jatekablak magassaga.
*@return void
*/

void move_player_ship(PlayerShip *ps, InputStateInterface *isi, int width, int height)
{
    if(isi->up && ps->y_coor > 0){
        ps->y_coor -= ps->speed;
        ps->texture_data.texture_center_y  -= ps->speed;
        ps->centerline_y_coor -= ps->speed;
    }
    if(isi->down &&  ps->y_coor < height){
        ps->y_coor += ps->speed;
        ps->texture_data.texture_center_y += ps->speed;
        ps->centerline_y_coor += ps->speed;
    }
    if(isi->left && ps->x_coor > 0){
        ps->x_coor -= ps->speed;
        ps->texture_data.texture_center_x -= ps->speed;
        ps->hitbox_beg_coor -= ps->speed;
        ps->hitbox_end_coor -= ps->speed;
    }
    if(isi->right && ps->x_coor  < width){
        ps->x_coor += ps->speed;
        ps->texture_data.texture_center_x += ps->speed;
        ps->hitbox_beg_coor += ps->speed;
        ps->hitbox_end_coor += ps->speed;
    }
}

/**
*@brief move_player_ship
*@details Felszabaditja a jatekos hajojat.
*@param [in] *ps A jatekos hajojanak pointere.
*@return void
*/

void free_player_ship(PlayerShip *ps){
    free(ps);
    ps = NULL;
}
