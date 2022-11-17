#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include "input_state_interface.h"
#include "keymap.h"
#include "graphics.h"
#include "ui_input.h"
#include "game_assets.h"
#include "game_attributes.h"
#include "star_map.h"
#include "player_ship.h"
#include "enemy_ship.h"
#include "hit_management.h"
#include "data_transfer_types.h"
#include "file_management.h"
#include "random_number_in_interval.h"
#include "texture_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "debugmalloc.h"

void runtime();

#endif // GAME_ENGINE_H_INCLUDED
