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
*@details
*/
typedef struct playership{
    int y_coor; ///<
    int x_coor; ///<
    int hitbox_beg_coor; ///<
    int hitbox_end_coor; ///<
    int centerline_y_coor; ///<
    TextureData texture_data; ///<
    SpriteMapData sprite_map_data;
    int speed; ///<
    int health; ///<
    int score_value; ///<
}PlayerShip;

PlayerShip *init_player_ship(GameAttributes *game_attributes,
                             ShipDTT *ship_dtt,
                             TextureData texture_data,
                             SpriteMapData sprite_map_data);

void move_player_ship(PlayerShip *ps, InputStateInterface *isi, int width, int height);

void free_player_ship(PlayerShip *ps);

#endif // PLAYER_SHIP_H_INCLUDED
