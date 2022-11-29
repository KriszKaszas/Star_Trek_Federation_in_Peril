/**
*@file graphics.c
*/

#include "graphics.h"

SDL_Window static *window;
SDL_Renderer static *renderer;

SDL_Texture static *fed_ship;
SDL_Texture static *en_ship;

/**
*@brief load_sdl_texture
*@details betolti az SDL altal hasznalt texturakat
*@param [] img_name
*@return SDL_Texture
*/
SDL_Texture *load_sdl_texture(char* img_name){
    SDL_Texture *texture = IMG_LoadTexture(renderer, img_name);
    if (texture == NULL) {
        SDL_Log("Cannot open image file: %s", IMG_GetError());
        return NULL;
    }
    return texture;
}

/**
*@brief sdl_init
*@details inicializalja az SDLt
*@param [] title
*@param [] width
*@param [] height
*@param [] pwindow
*@param [] prenderer
*@return void
*/
static void sdl_init(char const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/**
*@brief create_textures
*@details felepiti a texturakat
*@param [] fed
*@param [] enemy
*@return void
*/
void create_textures(char* fed, char* enemy){
    fed_ship = load_sdl_texture(fed);
    en_ship = load_sdl_texture(enemy);
}

/**
*@brief create_window
*@details legeneralja a jatekablakot
*@param [] width
*@param [] height
*@return void
*/
void create_window(int width, int height){
    sdl_init("Star Trek: Federation in Peril", width, height, &window, &renderer);
}

/**
*@brief draw_background
*@details kirajzolja a hatteret
*@param [] sm
*@return void
*/
void draw_background(StarMap *sm){
    for(int i = 0; i < sm->length; i++){
        filledCircleRGBA(renderer, sm->stars[i].x_coor, sm->stars[i].y_coor, sm->stars[i].radius,
                         sm->color.r, sm->color.g, sm->color.b, sm->color.a);
    }
}


/**
*@brief draw_player_ship
*@details kirajzolja a jatekos hajot
*@param [] ps
*@return void
*/
void draw_player_ship(PlayerShip *ps){

    if(fed_ship != NULL){
        SDL_Rect src = {128, 1, 46, 114};
        SDL_Rect dest = {ps->texture_data.texture_center_x, ps->texture_data.texture_center_y, ps->texture_data.width, ps->texture_data.height};
        SDL_RenderCopy(renderer, fed_ship, &src, &dest);
    }
    else{
        filledCircleRGBA(renderer, ps->x_coor, ps->y_coor, 30, 218, 223, 225, 255);
    }
}

/**
*@brief draw_crosshair
*@details kirajzolja a celkeresztet
*@param [] x_coor
*@param [] y_coor
*@return void
*/
void draw_crosshair(int x_coor, int y_coor){
    lineRGBA(renderer, x_coor-20, y_coor, x_coor+20, y_coor, 255, 50, 50, 255);
    lineRGBA(renderer, x_coor, y_coor-20, x_coor, y_coor+20, 255, 50, 50, 255);
}

/**
*@brief draw_enemy_ships
*@details kirajzolja az ellenseges hajokat
*@param [] armada
*@return void
*/
void draw_enemy_ships(EnemyShip *enemy_armada){
    /*for(int i = 0; i < armada->number_of_squadrons; i++){
        EnemySquadronShip *tmp = armada->enemy_armada[i];
        while(tmp != NULL){
            if(en_ship != NULL){
                SDL_Rect src = {166, 1, 74, 80};
                SDL_Rect dest = {tmp->ship.texture_data.texture_center_x, tmp->ship.texture_data.texture_center_y,
                                    tmp->ship.texture_data.width, tmp->ship.texture_data.height};
                SDL_RenderCopy(renderer, en_ship, &src, &dest);
            }
            else{*/


        while(enemy_armada != NULL)
        {
            filledCircleRGBA(renderer, enemy_armada->x_coor,
                            enemy_armada->y_coor, 30, 46, 204, 113, 255);
            enemy_armada=enemy_armada->next_ship;
        }

}


/**
*@brief draw_phaser
*@details kirajzolja a fezer sugarat
*@param [] phaser
*@return void
*/
void draw_phaser(PhaserBeam *phaser){
    thickLineRGBA(renderer, phaser->beg_x, phaser->beg_y, phaser->end_x, phaser->end_y, 12,
                  phaser->beam_composition.falloff_color.a, phaser->beam_composition.falloff_color.g,
                  phaser->beam_composition.falloff_color.b, phaser->beam_composition.falloff_color.a);
    thickLineRGBA(renderer, phaser->beg_x, phaser->beg_y, phaser->end_x, phaser->end_y, 1,
                  phaser->beam_composition.core_color.a, phaser->beam_composition.core_color.g,
                  phaser->beam_composition.core_color.b, phaser->beam_composition.core_color.a);
}

/**
*@brief draw_torpedo
*@details kirajzolja a kilott torpedot
*@param [] torpedoes
*@return void
*/
        /*if(tmp == 0x4B4B4B4B4B4B4B4Bull)
        {
            printf("megvagy geci");
        }*/
void draw_torpedo(TorpedoShot *torpedoes){
    TorpedoShot *tmp = torpedoes;
        //printf("before while: %p\n", tmp);
    while(tmp != NULL)
    {
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 15,
                         tmp->colors.outter_ring.r, tmp->colors.outter_ring.g, tmp->colors.outter_ring.b, tmp->colors.outter_ring.a);
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 10,
                         tmp->colors.inner_ring.r, tmp->colors.inner_ring.g, tmp->colors.inner_ring.b, tmp->colors.inner_ring.a);
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 5,
                         tmp->colors.center.r, tmp->colors.center.g, tmp->colors.center.b, tmp->colors.center.a);
        tmp = tmp->next_torpedo;
        //printf("after while:  %p\n", tmp);

    }
}

/**
*@brief clear_screen
*@details torol mindent a jatekablakbol
*@return void
*/
void clear_screen(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/**
*@brief render_screen
*@details rendereli a jatekablakot
*@return void
*/
void render_screen(){
    SDL_RenderPresent(renderer);
}

/**
*@brief destroy_textures
*@details torli a texturakat
*@return void
*/
void destroy_textures(){
    SDL_DestroyTexture(fed_ship);
    SDL_DestroyTexture(en_ship);
}