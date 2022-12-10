/**
*@file enemy_ship.h
*/

#ifndef ENEMY_SHIP_H_INCLUDED
#define ENEMY_SHIP_H_INCLUDED

#include "game_attributes.h"
#include "data_transfer_types.h"
#include "random_number_in_interval.h"
#include "texture_data.h"
#include "torpedo.h"

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "debugmalloc.h"

/**
*@brief EnemyShip
*@details Az ellenseges hajokat tarolo lancolt lista listaeleme
*/
typedef struct enemyship{
    int y_coor; ///< Az ellenseges hajo y koordinataja
    int x_coor; ///< Az ellenseges hajo x koordinataja
    int movement_dir; ///< Az ellenseges hajo mozgasi iranya
    int hitbox_beg_coor; ///< A hitbox bal szele
    int hitbox_end_coor; ///< A hitbox jobb szele
    int centerline_y_coor; ///< A hitbox also hatara (ahonnan mar erzekeli a sprite a talalatot)
    TextureData texture_data; ///< Az ellenseges hajo textura adatai (meretek)
    SpriteMapData sprite_map_data; ///< Az ellenseges hajo spritemap meretei (SDL2 igenyli)
    int speed; ///< Az ellenseges hajo mozgasi sebessege
    int health; ///< Az ellenseges hajo HP-ja
    int score_value; ///< Az ellenseges hajo lelovesevel megszerezheto pont
    struct enemyship *next_ship; ///< Az ellenseges hajokat tartalmazo lancolt lista kovetkezo eleme
    struct enemyship *prev_ship; ///< Az ellenseges hajokat tartalmazo lancolt lista elozo eleme
}EnemyShip;

EnemyShip *init_enemy_armada(TextureData texture_data,
                             SpriteMapData sprite_map_data,
                             ShipDTT **ship_dtt,
                             GameAttributes *game_attributes);

void move_enemy_armada(EnemyShip *enemy_armada, GameAttributes *game_attributes);

int find_max_enemy_armada_y_coor(EnemyShip *enemy_armada);

void pop_enemy_ship(EnemyShip **enemy_ship);

void free_enemy_armada(EnemyShip *enemy_armada);

#endif // ENEMY_SHIP_H_INCLUDED
