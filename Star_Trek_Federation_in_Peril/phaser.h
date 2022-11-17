#ifndef PHASER_H_INCLUDED
#define PHASER_H_INCLUDED

#include "input_state_interface.h"
#include "debugmalloc.h"

/**
*@brief BeamColor
*@details
*/
typedef struct beamcolor{
    int r; ///<
    int g; ///<
    int b; ///<
    int a; ///<
}BeamColor;

/**
*@brief Beam
*@details
*/
typedef struct beam{
    BeamColor core_color; ///<
    BeamColor falloff_color; ///<
}Beam;

/**
*@brief PhaserBeam
*@details
*/
typedef struct phaserbeam{
    int beg_x; ///<
    int beg_y; ///<
    int end_x; ///<
    int end_y; ///<
    Beam beam_composition; ///<
}PhaserBeam;

#endif // PHASER_H_INCLUDED
