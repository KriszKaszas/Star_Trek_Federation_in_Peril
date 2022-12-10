/**
*@file player_ship.h
*/

#ifndef PLAYER_SHIP_H_INCLUDED
#define PLAYER_SHIP_H_INCLUDED

#include "game_attributes.h"
#include "input_state_interface.h"
#include "data_transfer_types.h"
#include "texture_data.h"
#include "player_ship.h"

#include <stdbool.h>
#include <stdio.h>

#include "debugmalloc.h"

/**
*@brief PlayerShip
*@details A jatekos hajojat tarolo adatstruktura.
*/
typedef struct playership{
    int y_coor; ///< A hajo y koordinataja
    int x_coor; ///< A hajo x koordinataja
    int hitbox_beg_coor; ///< A hitbox bal szele
    int hitbox_end_coor; ///< A hitbox jobb szele
    int centerline_y_coor; ///< A hitbox also hatara (ahonnan mar erzekeli a sprite a talalatot)
    TextureData texture_data; ///< A hajo textura adatai (meretek)
    SpriteMapData sprite_map_data; ///< A hajo spritemap meretei (SDL2 igenyli)
    int speed; ///< A hajo mozgasi sebessege
    int health; ///< A hajo HP-ja
    int score_value; ///< A hajo elpusztulasanal elveszitett pont
}PlayerShip;

PlayerShip *init_player_ship(GameAttributes *game_attributes,
                             ShipDTT *ship_dtt,
                             TextureData texture_data,
                             SpriteMapData sprite_map_data);

void move_player_ship(PlayerShip *ps,
                      InputStateInterface *isi,
                      int width,
                      int height);

void free_player_ship(PlayerShip *ps);

#endif // PLAYER_SHIP_H_INCLUDED
