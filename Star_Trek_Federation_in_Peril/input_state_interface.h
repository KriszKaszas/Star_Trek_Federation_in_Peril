#ifndef INPUT_STATE_INTERFACE_H_INCLUDED
#define INPUT_STATE_INTERFACE_H_INCLUDED

#include <stdbool.h>

/**
*@brief MousePosition
*@details
*/
typedef struct mouseposition{
    int mouse_x; ///<
    int mouse_y; ///<
}MousePosition;

/**
*@brief InputStateInterface
*@details
*/
typedef struct inputstateinterface{
    bool quit; ///<
    bool up; ///<
    bool down; ///<
    bool left; ///<
    bool right; ///<
    bool torpedo; ///<
    bool torpedo_ready; ///<
    bool left_mouse_button; ///<
    bool right_mouse_button; ///<
    MousePosition mouse_position; ///<
    bool phaser_ready; ///<
    bool phaser_firing; ///<
} InputStateInterface;

#endif // INPUT_STATE_INTERFACE_H_INCLUDED
