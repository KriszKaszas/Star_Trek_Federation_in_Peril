#ifndef TORPEDO_HIT_MANAGEMENT_H_INCLUDED
#define TORPEDO_HIT_MANAGEMENT_H_INCLUDED

#include "game_attributes.h"
#include "torpedo.h"

#include <stdbool.h>

#include "debugmalloc.h"

bool is_torpedo_out_of_bounds(TorpedoShot **torpedo, GameAttributes *game_attributes);

void explode_torpedo(TorpedoShot **player_torpedo, TorpedoShot **temp_torpedo);


#endif // TORPEDO_HIT_MANAGEMENT_H_INCLUDED
