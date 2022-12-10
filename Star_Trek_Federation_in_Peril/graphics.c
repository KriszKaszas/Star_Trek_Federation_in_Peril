/**
*@file graphics.c
*/

#include "graphics.h"

static SDL_Window *window;
static SDL_Renderer *renderer;

static SDL_Texture *fed_ship;
static SDL_Texture *en_ship;

static TTF_Font *LCARS_font;
static TTF_Font *LCARS_font_large;

static SDL_Surface *LCARS_text;
static SDL_Surface *LCARS_text_large;

static SDL_Texture *LCARS_text_texture;
static SDL_Texture *LCARS_text_texture_large;

static SDL_Rect location = {70, 820, 0, 0};
static SDL_Rect location_large = {500, 200, 0, 0};

static SDL_Color white = {255, 255, 255};



/**
*@brief load_sdl_texture
*@details Betolti az SDL2 altal hasznalt texturakat.
*@param [in] img_name A betoltendo kepfajl neve.
*@return fontokat A betoltott textura.
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
*@brief open_font
*@details Betolti az SDL2 altal hasznalt fontokat.
*@param [in] size A betoltendo font merete.
*@return font A betoltott font.
*/

TTF_Font *open_font(int size)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("OkudaBold-qA72.ttf", size);
    if (!font) {
        SDL_Log("Cannot open font! %s\n", TTF_GetError());
        exit(1);
    }
    return font;
}

/**
*@brief sdl_init
*@details Inicializalja az SDL2-t.
*@param [in] title
*@param [in] width
*@param [in] height
*@param [in] pwindow
*@param [in] prenderer
*@return void
*/

void sdl_init(char const *title, int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Cannot start SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL) {
        SDL_Log("Cannot create window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    if (renderer == NULL) {
        SDL_Log("Cannot create renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/**
*@brief create_font
*@details Beolvassa a szukseges meretu fontokat.
*@return void
*/

void create_font()
{
    LCARS_font = open_font(64);
    LCARS_font_large = open_font(150);
}

/**
*@brief create_textures
*@details felepiti a texturakat
*@param [in] fed A jatekos texturajat tartalmazo fajl neve.
*@param [in] enemy A ellenseg texturajat tartalmazo fajl neve.
*@return void
*/

void create_textures(char* fed, char* enemy)
{
    fed_ship = load_sdl_texture(fed);
    en_ship = load_sdl_texture(enemy);
}

/**
*@brief create_window
*@details Legeneralja a jatekablakot.
*@param [in] width
*@param [in] height
*@return void
*/

void create_window(int width, int height)
{
    sdl_init("Star Trek: Federation in Peril", width, height, &window, &renderer);
}

/**
*@brief draw_background
*@details kirajzolja a hatteret
*@param [in] *sm A csillagterkep pointere.
*@return void
*/

void draw_background(StarMap *sm)
{
    for(int i = 0; i < sm->length; i++){
        filledCircleRGBA(renderer, sm->stars[i].x_coor, sm->stars[i].y_coor, sm->stars[i].radius,
                         sm->color.r, sm->color.g, sm->color.b, sm->color.a);
    }
}


/**
*@brief draw_player_ship
*@details Kirajzolja a jatekos hajot.
*@param [in] *ps A jatekos hajojanak pointere.
*@return void
*/
void draw_player_ship(PlayerShip *ps)
{
    if(ps == NULL)
    {
        return;
    }
    if(fed_ship != NULL){
        SDL_Rect src = {ps->sprite_map_data.x_coor, ps->sprite_map_data.y_coor, ps->sprite_map_data.width, ps->sprite_map_data.height};
        SDL_Rect dest = {ps->texture_data.texture_center_x, ps->texture_data.texture_center_y, ps->texture_data.width, ps->texture_data.height};
        SDL_RenderCopy(renderer, fed_ship, &src, &dest);
    }
    else{
        filledCircleRGBA(renderer, ps->x_coor, ps->y_coor, 30, 218, 223, 225, 255);
        thickLineRGBA(renderer, ps->hitbox_beg_coor, ps->centerline_y_coor, ps->hitbox_end_coor, ps->centerline_y_coor, 5, 255, 0, 0, 255);
    }
}

/**
*@brief draw_enemy_ships
*@details Kirajzolja az ellenseges hajokat.
*@param [in] *armada Az ellenseges hajok listajanak head pointere.
*@return void
*/

void draw_enemy_ships(EnemyShip *enemy_armada)
{
    if(enemy_armada == NULL)
    {
        return;
    }
    while(enemy_armada != NULL)
    {
        if(en_ship != NULL)
        {
            SDL_Rect src = {enemy_armada->sprite_map_data.x_coor, enemy_armada->sprite_map_data.y_coor,
                                enemy_armada->sprite_map_data.width, enemy_armada->sprite_map_data.height};
            SDL_Rect dest = {enemy_armada->texture_data.texture_center_x, enemy_armada->texture_data.texture_center_y,
                                enemy_armada->texture_data.width, enemy_armada->texture_data.height};
            SDL_RenderCopy(renderer, en_ship, &src, &dest);
        }
        else
        {
            filledCircleRGBA(renderer, enemy_armada->x_coor,
                            enemy_armada->y_coor, 30, 46, 204, 113, 255);
            thickLineRGBA(renderer, enemy_armada->hitbox_beg_coor, enemy_armada->centerline_y_coor, enemy_armada->hitbox_end_coor, enemy_armada->centerline_y_coor, 5, 255, 0, 0, 255);
        }
        enemy_armada=enemy_armada->next_ship;
    }

}

/**
*@brief draw_torpedo
*@details Kirajzolja a kilott torpedot.
*@param [in] *torpedoes A torpedok listajanak head pointere.
*@return void
*/

void draw_torpedo(TorpedoShot *torpedoes)
{
    TorpedoShot *tmp = torpedoes;
    while(tmp != NULL)
    {
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 15,
                           tmp->colors.outter_ring.r, tmp->colors.outter_ring.g, tmp->colors.outter_ring.b, tmp->colors.outter_ring.a);
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 10,
                         tmp->colors.inner_ring.r, tmp->colors.inner_ring.g, tmp->colors.inner_ring.b, tmp->colors.inner_ring.a);
        filledCircleRGBA(renderer, tmp->x_coor, tmp->y_coor, 5,
                         tmp->colors.center.r, tmp->colors.center.g, tmp->colors.center.b, tmp->colors.center.a);
        tmp = tmp->next_torpedo;

    }
}

/**
*@brief draw_end_screen
*@details Kirajzolja a jatek vege kepernyot.
*@return void
*/

void draw_end_screen()
{
    char end_message[41] = "GAME OVER    PLAY AGAIN?  PRESS Y OR N";
    LCARS_text_large = TTF_RenderText_Blended_Wrapped(LCARS_font_large, end_message, white, 700);
    LCARS_text_texture_large = SDL_CreateTextureFromSurface(renderer, LCARS_text_large);
    location_large.w = LCARS_text_large->w;
    location_large.h = LCARS_text_large->h;
    SDL_RenderCopy(renderer, LCARS_text_texture_large, NULL, &location_large);
}

/**
*@brief draw_score
*@details Kirajzolja a jatekos pontjait kepernyot.
*@param [in] game_score a jatekos pontjai.
*@return void
*/

void draw_score(int game_score)
{
    char LCARS_score_str[27] = "SCORE ";
    char score_str[20];
    itoa(game_score, score_str, 10);
    strcat(LCARS_score_str, score_str);

    LCARS_text = TTF_RenderUTF8_Blended(LCARS_font, LCARS_score_str, white);

    LCARS_text_texture = SDL_CreateTextureFromSurface(renderer, LCARS_text);
    location.w = LCARS_text->w;
    location.h = LCARS_text->h;
    SDL_RenderCopy(renderer, LCARS_text_texture, NULL, &location);
}

/**
*@brief draw_LCARS_bacground
*@details Kirajzolja a jatekos pontjainak hatteret.
*@return void
*/

void draw_LCARS_bacground()
{
    boxRGBA(renderer, 40, 830, 1860, 880, 216, 165, 112, 255);
    filledCircleRGBA(renderer, 1860, 855, 25, 216, 165, 112, 255);
    filledCircleRGBA(renderer, 40, 855, 25, 216, 165, 112, 255);
    boxRGBA(renderer, 50, 830, 300, 880, 0, 0, 0, 255);
}

/**
*@brief clear_screen
*@details Torol mindent a jatekablakbol.
*@return void
*/

void clear_screen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/**
*@brief render_screen
*@details Rendereli a jatekablakot.
*@return void
*/
void render_screen()
{
    SDL_RenderPresent(renderer);
}

/**
*@brief destroy_textures
*@details Torli a texturakat.
*@return void
*/
void destroy_textures()
{
    SDL_DestroyTexture(fed_ship);
    SDL_DestroyTexture(en_ship);
    SDL_FreeSurface(LCARS_text);
    SDL_DestroyTexture(LCARS_text_texture);
    TTF_CloseFont(LCARS_font);
    TTF_CloseFont(LCARS_font_large);
}
