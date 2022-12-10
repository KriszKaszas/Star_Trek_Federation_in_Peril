#ifndef TORPEDO_H_INCLUDED
#define TORPEDO_H_INCLUDED

#include "game_attributes.h"
#include <stdio.h>

#include <stdbool.h>

#include "debugmalloc.h"

/**
*@brief ColorData
*@details
*/
typedef struct colordata
{
    int r; ///<
    int g; ///<
    int b; ///<
    int a; ///<
}ColorData;

/**
*@brief TorpedoColors
*@details
*/
typedef struct torpedocolors
{
    ColorData outter_ring; ///<
    ColorData inner_ring; ///<
    ColorData center; ///<
}TorpedoColors;

/**
*@brief TorpedoShot
*@details
*/
typedef struct torpedoshot
{
    int x_coor; ///<
    int y_coor; ///<
    int damage; ///<
    int speed; ///<
    int dir; ///<
    TorpedoColors colors; ///<
    struct torpedoshot *next_torpedo; ///<
    struct torpedoshot *prev_torpedo; ///<
}TorpedoShot;

TorpedoShot *add_torpedo_shot(TorpedoShot *torpedoes, int damage, int speed, int x_coor, int y_coor, bool is_enemy_torpedo);

void move_torpedoes(TorpedoShot **torpedo);

void pop_torpedo_shot(TorpedoShot **torpedo);

void free_torpedoes(TorpedoShot *torpedoes);

#endif // TORPEDO_H_INCLUDED
