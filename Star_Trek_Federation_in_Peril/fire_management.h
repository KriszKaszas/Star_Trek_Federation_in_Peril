#ifndef FIRE_MANAGEMENT_H_INCLUDED
#define FIRE_MANAGEMENT_H_INCLUDED

#include "game_attributes.h"
#include "game_assets.h"
#include "enemy_ship.h"

#include "debugmalloc.h"

void fire_player_torpedo(GameAssets *game_assets, GameAttributes *game_attributes);

void fire_enemy_torpedoes(int armada_size, GameAssets *game_assets);

#endif // FIRE_MANAGEMENT_H_INCLUDED
