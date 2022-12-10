#ifndef PLAYER_HIT_MANAGEMENT_H_INCLUDED
#define PLAYER_HIT_MANAGEMENT_H_INCLUDED

#include "game_assets.h"
#include "player_ship.h"

#include "debugmalloc.h"

void explode_player_ship_if_dead(PlayerShip **player_ship,
                                 GameAssets **game_assets,
                                 GameAttributes *game_attributes);

#endif // PLAYER_HIT_MANAGEMENT_H_INCLUDED
