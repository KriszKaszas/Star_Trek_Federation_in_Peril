/**
*@file phaser.c
*/

#include "phaser.h"

/**
*@brief create_beam_attributes
*@details legeneralja a fezer sugar kirajzolasahoz szukseges attributum tipust.
*@param [] core
*@param [] falloff
*@return void
*/
Beam create_beam_attributes(BeamColor core, BeamColor falloff){
    Beam *beam_att = malloc(sizeof(Beam));
    beam_att->core_color = core;
    beam_att->falloff_color = falloff;
}

/**
*@brief phaser_init
*@details inicializalja a sugarat.
*@param [] beam_att
*@param [] x_coor
*@param [] y_coor
*@return void
*/
PhaserBeam *phaser_init(Beam beam_att, int x_coor, int y_coor){
    PhaserBeam *phaserbeam = malloc(sizeof(PhaserBeam));
    phaserbeam->beg_x = x_coor;
    phaserbeam->beg_y = y_coor;
    phaserbeam->end_x = x_coor;
    phaserbeam->end_y = y_coor;
    phaserbeam->beam_composition = beam_att;
    return phaserbeam;
}

/**
*@brief phaser_init
*@details felszabaditja a fezer memoriateruletet.
*@param [] *phaser
*@return void
*/
void free_phaser(PhaserBeam *phaser){
    free(phaser);
    phaser = NULL;
}
