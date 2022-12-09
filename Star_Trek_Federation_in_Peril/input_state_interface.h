#ifndef INPUT_STATE_INTERFACE_H_INCLUDED
#define INPUT_STATE_INTERFACE_H_INCLUDED

#include <stdbool.h>

/**
*@brief InputStateInterface
*@details
*/
typedef struct inputstateinterface{
    bool quit; ///<
    bool restart;
    bool game_over;
    bool up; ///<
    bool down; ///<
    bool left; ///<
    bool right; ///<
    bool y;
    bool n;
    bool torpedo; ///<
    bool torpedo_ready; ///<
} InputStateInterface;

#endif // INPUT_STATE_INTERFACE_H_INCLUDED
