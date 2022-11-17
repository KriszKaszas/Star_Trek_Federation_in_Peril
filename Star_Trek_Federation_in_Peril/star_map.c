/**
*@file star_map.c
*/

#include "star_map.h"



/**
*@brief advance_starmap_frame
*@details a csillagterkepet eloremozditja egy kockaval. Vegigmegy a csillagokat tartalmazo dinamikus listan, es mindnek
*egyel noveli az y koordinatajat, amennyiben az nem 10-el nagyobb az ablak magassaganal. Amennyiben ennel az erteknel magasabb az adott
*csillag y erteke, ugy az y koordinatat 0-ra, az x koordinatat pedig egy, a kepernyo szelessegeben talalhato random ertekre allitja.
*@param [out] sm egy StarMap tipusu pointer, a jatek StarMap tipusaban tarolt csillagok koordinatait tarolja.
*@param [in] width a kepernyo szelessege. Erre a random szam generalasahoz van szukseg.
*@param [in] height a kepernyo magassaga. Erre a csillag y koordinatajanak ellenorzesehez van szukseg.
*@return void
*/
void advance_starmap_frame(StarMap *sm, int width, int height){
    for(int i = 0; i < sm->length; i++){
        if(sm->stars[i].y_coor < height + 10){
            sm->stars[i].y_coor += 2;
        }
        else{
            sm->stars[i].y_coor = 0;
            sm->stars[i].x_coor = random_number_in_range(0, width);
        }
    }
}

/**
*@brief create_star
*@details Ez a fuggveny egy darab csillagot inicializal. ivalaszt neki egy random meretet, es egy random x es y
koordinatat 0 es a kepernyo szelessege, valamint 0 es a kepernyo magassaga kozott. Az elkeszult csillaggal ter vissza.
*@param [in] width a kepernyo szelessege. Erre a csillag x koordinatajanak generalasahoz van szukseg.
*@param [in] height a kepernyo magassaga. Erre a csillag y koordinatajanak generalasahoz van szukseg.
*@return Star
*/
Star static create_star(int width, int height){
    int x_coor = random_number_in_range(0, width);
    int y_coor = random_number_in_range(0, height);
    int radius = random_number_in_range(1, 4);
    Star st = {y_coor, x_coor, radius};
    return st;
}

/**
*@brief starmap_init
*@details Ez a fuggveny inicializalja a StarMap csillagterkep listajat. letrehoz egy, a csillagok vart szamanak megfelelo hosszusagu dinamikus tombot,
*majd abban elhelyezi a sorban generalt csillagokat. Visszateresi erteke egy csillagterkep.
*@param [in] width a kepernyo szelessege. Erre a csillagok x koordinatajanak generalasahoz van szukseg.
*@param [in] height a kepernyo magassaga. Erre a csillagok y koordinatajanak generalasahoz van szukseg.
*@return StarMap
*/
StarMap *starmap_init(int width, int height){
    StarColor white = {225, 225, 225, 225};
    int size = width/20;
    Star *stars = (Star*) malloc(size*sizeof(Star));
    for(int i = 0; i < size; i++){
        stars[i] = create_star(width, height);
    }
    StarMap *sm = (StarMap*) malloc(sizeof(StarMap));
    sm->length = size;
    sm->stars = stars;
    sm->color = white;
    return sm;
}

/**
*@brief free_starmap
*@details Ez a fuggveny a parameterkent kapott csillagterkep csillagainak listajat, majd magat a csillagterkepet szabaditja fel.
*@param [in] sm a felszabaditando csillagterkep pointer.
*@return void
*/
void free_starmap(StarMap *sm){
    free(sm->stars);
    sm->stars = NULL;
    free(sm);
    sm = NULL;
}

