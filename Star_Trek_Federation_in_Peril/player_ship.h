#ifndef PLAYER_SHIP_H_INCLUDED
#define PLAYER_SHIP_H_INCLUDED

#include "input_state_interface.h"
#include "texture_data.h"
#include "player_ship.h"
#include "phaser.h"

#include <stdbool.h>

#include "debugmalloc.h"

/**
*@brief PlayerShip
*@details
*/
typedef struct playership{
    int y_coor; ///<
    int x_coor; ///<
    int centerline_x_beg_coor; ///<
    int centerline_x_end_coor; ///<
    int centerline_y_coor; ///<
    TextureData texture_data; ///<
    PhaserBeam *phaser_blast; ///<
    int phaser_timer; ///<
    int health; ///<
    int speed; ///<
}PlayerShip;

PlayerShip *init_player_ship(int width, int height, TextureData texture_data, int health, int speed);

void move_player_ship(PlayerShip *ps, InputStateInterface *isi, int width, int height);

void free_player_ship(PlayerShip *ps);

#endif // PLAYER_SHIP_H_INCLUDED
