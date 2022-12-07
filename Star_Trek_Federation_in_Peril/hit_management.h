#ifndef HIT_MANAGEMENT_H_INCLUDED
#define HIT_MANAGEMENT_H_INCLUDED

#include "game_attributes.h"
#include "game_assets.h"
#include "torpedo.h"
#include "enemy_ship.h"
#include "torpedo_hit_management.h"
#include "enemy_hit_management.h"

#include <stdbool.h>

#include "debugmalloc.h"

void remove_torpedo_if_out_of_bounds(TorpedoShot **torpedo, GameAttributes *game_attributes);;

void manage_player_hits(EnemyShip **enemy_armada, TorpedoShot **player_torpedo, GameAttributes *game_attributes);

#endif // HIT_MANAGEMENT_H_INCLUDED
