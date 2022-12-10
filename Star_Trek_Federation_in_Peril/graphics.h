#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "star_map.h"
#include "player_ship.h"
#include "enemy_ship.h"
#include "input_state_interface.h"
#include "torpedo.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "debugmalloc.h"

void create_font();

void create_textures(char* fed, char* enemy);

void create_window(int width, int height);

void draw_background(StarMap *sm);

void draw_player_ship(PlayerShip *ps);

void draw_enemy_ships(EnemyShip *enemy_armada);

void draw_torpedo(TorpedoShot *torpedoes);

void draw_end_screen();

void draw_score(int game_score);

void draw_LCARS_bacground();

void clear_screen();

void render_screen();

void destroy_textures();

#endif // GRAPHICS_H_INCLUDED
