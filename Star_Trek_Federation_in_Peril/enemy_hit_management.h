#ifndef ENEMY_HIT_MANAGEMENT_H_INCLUDED
#define ENEMY_HIT_MANAGEMENT_H_INCLUDED

#include "game_attributes.h"
#include "game_assets.h"
#include "torpedo.h"

void explode_enemy_ship_if_dead(EnemyShip **enemy_ship, EnemyShip **temp_ship, GameAttributes *game_attributes);

#endif // ENEMY_HIT_MANAGEMENT_H_INCLUDED
