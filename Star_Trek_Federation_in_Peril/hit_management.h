#ifndef HIT_MANAGEMENT_H_INCLUDED
#define HIT_MANAGEMENT_H_INCLUDED

#include "game_assets.h"
#include "game_attributes.h"
#include "torpedo.h"
#include "enemy_ship.h"

#include <stdbool.h>

#include "debugmalloc.h"

void explode_torpedo(TorpedoShot **player_torpedo, TorpedoShot **pt);

#endif // HIT_MANAGEMENT_H_INCLUDED
