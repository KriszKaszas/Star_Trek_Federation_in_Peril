#ifndef GAME_ASSETS_H_INCLUDED
#define GAME_ASSETS_H_INCLUDED

#include "star_map.h"
#include "player_ship.h"
#include "enemy_ship.h"
#include "torpedo.h"

/**
*@brief GameAssets
*@details
*/
typedef struct gameassets{
    StarMap *star_map; ///<
    PlayerShip *player_ship; ///<
    EnemyShip *enemy_armada; ///<
    TorpedoShot *player_torpedoes; ///<
    TorpedoShot *enemy_torpedoes; ///<
}GameAssets;

#endif // GAME_ASSETS_H_INCLUDED
