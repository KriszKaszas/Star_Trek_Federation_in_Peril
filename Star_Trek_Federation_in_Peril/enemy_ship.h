#ifndef ENEMY_SHIP_H_INCLUDED
#define ENEMY_SHIP_H_INCLUDED

#include "game_attributes.h"
#include "data_transfer_types.h"
#include "random_number_in_interval.h"
#include "texture_data.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "debugmalloc.h"

/**
*@brief EnemyShip
*@details
*/
typedef struct enemyship{
    int y_coor; ///<
    int x_coor; ///<
    int hitbox_beg_coor; ///<
    int hitbox_end_coor; ///<
    int centerline_y_coor; ///<
    TextureData texture_data; ///<
    int speed; ///<
    int health; ///<
    int damage; ///<
    int score_value;
    struct enemyship *next_ship; ///<
    struct enemyship *prev_ship; ///<
}EnemyShip;


#endif // ENEMY_SHIP_H_INCLUDED
