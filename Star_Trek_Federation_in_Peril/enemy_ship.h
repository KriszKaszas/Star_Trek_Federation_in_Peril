#ifndef ENEMY_SHIP_H_INCLUDED
#define ENEMY_SHIP_H_INCLUDED

#include "game_attributes.h"
#include "data_transfer_types.h"
#include "random_number_in_interval.h"
#include "texture_data.h"

#include <stdlib.h>
#include <stdbool.h>

#include "debugmalloc.h"

/**
*@brief EnemyShip
*@details
*/
typedef struct enemyship{
    int y_coor; ///<
    int x_coor; ///<
    TextureData texture_data; ///<
    int speed; ///<
    int health; ///<
    int damage; ///<
}EnemyShip;

/**
*@brief EnemySquadronShip
*@details
*/
typedef struct enemysquadronship{
    EnemyShip ship; ///<
    struct enemysquadronship *next_ship; ///<
    struct enemysquadronship *prev_ship; ///<
}EnemySquadronShip;

/**
*@brief EnemyArmada
*@details
*/
typedef struct enemyarmada{
    int number_of_squadrons; ///<
    int *no_of_ships_per_sq; ///<
    EnemySquadronShip **enemy_armada; ///<
    int *squadron_dirs; ///<
    int *squadron_heights; ///<
    bool *entry_finished_per_squadron; ///<
    bool ready_to_move; ///<
}EnemyArmada;

#endif // ENEMY_SHIP_H_INCLUDED
