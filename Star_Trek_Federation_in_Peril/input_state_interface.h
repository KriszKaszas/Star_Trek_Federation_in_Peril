#ifndef INPUT_STATE_INTERFACE_H_INCLUDED
#define INPUT_STATE_INTERFACE_H_INCLUDED

#include <stdbool.h>

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
} InputStateInterface;

#endif // INPUT_STATE_INTERFACE_H_INCLUDED
