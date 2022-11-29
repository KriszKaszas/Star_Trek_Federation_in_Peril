/**
*@file player_ship.c
*/

#include "player_ship.h"

/**
*@brief init_player_ship
*@details inicializalja a jatekos hajojat
*@param [] width
*@param [] height
*@param [] texture_data
*@param [] health
*@param [] speed
*@return PlayerShip
*/
PlayerShip *init_player_ship(int width, int height, TextureData texture_data, int health, int speed){
    PlayerShip *ps = (PlayerShip*) malloc(sizeof(PlayerShip));
    BeamColor core = {253, 241, 191, 255};
    BeamColor falloff = {61, 119, 69, 220};
    Beam beam_comp = {core, falloff};

    ps->x_coor = width/2;
    ps->y_coor = height/4*3;
    ps->texture_data = texture_data;
    ps->texture_data.texture_center_x = ps->x_coor - (ps->texture_data.width/2);
    ps->texture_data.texture_center_y = ps->y_coor - (ps->texture_data.height/2);
    ps->centerline_x_beg_coor = ps->x_coor - (texture_data.width/2);
    ps->centerline_x_end_coor = ps->x_coor + (texture_data.width/2);
    ps->centerline_y_coor = ps->y_coor;
    ps->health = health;
    ps->speed = speed;
    ps->phaser_timer = 0;
    PhaserBeam *PhaserBeam = phaser_init(beam_comp, ps->x_coor, ps->y_coor);
    ps->phaser_blast = PhaserBeam;
    return ps;
}

/**
*@brief move_player_ship
*@details a jatekos hajojanak mozgasaert felelos szamitasokat vegzi
*@param [] ps
*@param [] isi
*@param [] width
*@param [] height
*@return void
*/
void move_player_ship(PlayerShip *ps, InputStateInterface *isi, int width, int height){
    if(isi->up && ps->y_coor > 0){
        ps->y_coor -= ps->speed;
        ps->texture_data.texture_center_y  -= ps->speed;
        ps->phaser_blast->beg_y -= ps->speed;
    }
    if(isi->down &&  ps->y_coor < height){
        ps->y_coor += ps->speed;
        ps->texture_data.texture_center_y += ps->speed;
        ps->phaser_blast->beg_y += ps->speed;
    }
    if(isi->left && ps->x_coor > 0){
        ps->x_coor -= ps->speed;
        ps->texture_data.texture_center_x -= ps->speed;
        ps->phaser_blast->beg_x -= ps->speed;
    }
    if(isi->right && ps->x_coor  < width){
        ps->x_coor += ps->speed;
        ps->texture_data.texture_center_x += ps->speed;
        ps->phaser_blast->beg_x += ps->speed;
    }
}

/**
*@brief fire_phaser
*@details Kilo egy fezersugarat.
*@param [in out] isi pointer to an InputStateInterface type
*@param [in out] ps pointer to a PlayerShip type
*@param [in] elapsed_interval marker of elapsed ticks since program started
*@return void
*/
void fire_phaser(InputStateInterface *isi, PlayerShip *ps, int elapsed_interval){
    ps->phaser_blast->end_x = isi->mouse_position.mouse_x;
    ps->phaser_blast->end_y = isi->mouse_position.mouse_y;
    ps->phaser_timer = elapsed_interval;
    isi->phaser_firing = true;
}

/**
*@brief move_player_ship
*@details felszabaditja a jatekos hajojat
*@param [] ps
*@return void
*/
void free_player_ship(PlayerShip *ps){
    free(ps->phaser_blast);
    ps->phaser_blast = NULL;
    free(ps);
    ps = NULL;
}