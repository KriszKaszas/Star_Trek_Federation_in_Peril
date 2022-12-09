#ifndef GAME_ATTRIBUTES_H_INCLUDED
#define GAME_ATTRIBUTES_H_INCLUDED

#include "input_state_interface.h"
#include <SDL.h>

/**
*@brief GameAttributes
*@details
*/
typedef struct gameattributes{
    int width; ///<
    int height; ///<
    int enemy_armada_size; ///<
    int num_of_rows;
    int enemy_ships_per_row;
    int game_score;
    InputStateInterface isi; ///<
    SDL_TimerID id; ///<
}GameAttributes;

#endif // GAME_ATTRIBUTES_H_INCLUDED
