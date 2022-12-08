/**
*@file game_engine.c
*/

#include "game_engine.h"

/**
*@brief input_timer
*@details az inputok beolvasasanak idoziteseert felel
*@param [in] ms
*@param [in] param
*@return Uint32
*/
Uint32  input_timer(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

/**
*@brief init_game_assets
*@details inicializalja az osszes jatekhoz szukseges assetet.
*<br>VIGYAZAT: a game_assets pointereinek NULL-ra allitasaert a hivo felel a free fuggvenyek hivasa utan!
*@param [in out] game_attributes
*@return GameAssets
*/
GameAssets *init_game_assets(GameAttributes *game_attributes){

    ///MOCK DATA TYPES
    ///TODO: read from file
    ShipDTT ship_dtt = {1, 10, 1};
    TextureData fed_texture_data = {46, 130};
    TextureData enemy_texture_data = {60, 60};
    ///
    GameAssets *game_assets = (GameAssets*) malloc(sizeof(GameAssets));
    game_assets->star_map = starmap_init(game_attributes->width, game_attributes->height);
    game_assets->player_ship = init_player_ship(game_attributes->width, game_attributes->height, fed_texture_data, 10, 1);
    game_assets->enemy_armada = init_enemy_armada(enemy_texture_data, &ship_dtt, game_attributes);
    game_assets->player_torpedoes = NULL;
    game_assets->enemy_torpedoes = NULL;

    return game_assets;
}

/**
*@brief init_game_attributes
*@details inicializalja a jatek attributumait, amelyeket aztan a vezerles hasznal
*@return GameAttributes
*/
GameAttributes *init_game_attributes(){
    GameAttributes *game_attributes = (GameAttributes*) malloc(sizeof(GameAttributes));
    game_attributes->enemy_armada_size = 50;
    game_attributes->width = 1900;
    game_attributes->height = 900;
    game_attributes->isi.up = false;
    game_attributes->isi.down = false;
    game_attributes->isi.left = false;
    game_attributes->isi.right = false;
    game_attributes->isi.torpedo = false;
    game_attributes->isi.torpedo_ready = true;
    game_attributes->isi.quit = false;
    create_window(game_attributes->width, game_attributes->height);
    create_textures("player_ship_sprites.png", "enemy_ship_sprites.png");
    SDL_ShowCursor(SDL_DISABLE);
    game_attributes->id = SDL_AddTimer(3, input_timer, NULL);
    return game_attributes;
}

/**
*@brief default_keymap_init
*@details az iranyitashoz hasznalt alapertelmezett billentyuket inicializalja egy KeyMap tipusba
*@return KeyMap
*/
KeyMap *default_keymap_init(){
    KeyMap *key_map = malloc(sizeof(KeyMap));
    key_map->upkey = "W";
    key_map->downkey = "S";
    key_map->leftkey = "A";
    key_map->rightkey = "D";
    key_map->torpedokey = "Space";
    return key_map;
}

/**
*@brief clear_graphics
*@details kirajzoltatas elott mindent torol a kepernyorol.
*@param [] game_assets
*@return void
*/
void clear_graphics(GameAssets *game_assets){
    clear_screen();
}

/**
*@brief draw_graphics
*@details az osszes asset kirajzolasaert felel
*@param [in] player_ship_time
*@param [in] game_assets
*@param [in] game_attributes
*@return void
*/
void draw_graphics(int player_ship_time, GameAssets *game_assets, GameAttributes *game_attributes)
{
    draw_background(game_assets->star_map);
    draw_torpedo(game_assets->player_torpedoes);
    draw_torpedo(game_assets->enemy_torpedoes);
    draw_enemy_ships(game_assets->enemy_armada);
    draw_player_ship(game_assets->player_ship);
}

/**
*@brief calculate_game_assets
*@details a jatek assetek mozgasat es mukodeset vezerlo szamitasok aggregalo fuggvenye
*@param [in] game_assets
*@param [in] game_attributes
*@param [in] enemy_ship_time
*@return void
*/
void calculate_game_assets(GameAssets **game_assets, GameAttributes *game_attributes, int enemy_ship_time){
    int static time = 0;
    int static shot_time = 0;
    if((*game_assets)->player_ship != NULL)
    {
        move_player_ship((*game_assets)->player_ship, &game_attributes->isi, game_attributes->width, game_attributes->height);
    }
    advance_starmap_frame((*game_assets)->star_map, game_attributes->width, game_attributes->height);
    if((*game_assets)->enemy_armada != NULL && enemy_ship_time*2 > time)
    {
        move_enemy_armada((*game_assets)->enemy_armada, game_attributes);
        time = enemy_ship_time*2;
    }
    if((*game_assets)->enemy_armada != NULL && enemy_ship_time > shot_time)
    {
        fire_enemy_torpedoes(game_attributes->enemy_armada_size, (*game_assets));
        shot_time += 500;
    }
    if((*game_assets)->enemy_torpedoes != NULL)
    {
        move_torpedoes(&(*game_assets)->enemy_torpedoes);
        remove_torpedo_if_out_of_bounds(&(*game_assets)->enemy_torpedoes, game_attributes);
        manage_player_hits(&(*game_assets)->player_ship, &(*game_assets)->enemy_torpedoes, game_assets);
    }
    if((*game_assets)->player_ship != NULL && game_attributes->isi.torpedo){
        fire_player_torpedo((*game_assets), game_attributes);
    }
    if((*game_assets)->player_torpedoes != NULL){
        move_torpedoes(&(*game_assets)->player_torpedoes);
        remove_torpedo_if_out_of_bounds(&(*game_assets)->player_torpedoes, game_attributes);
        manage_enemy_hits(&(*game_assets)->enemy_armada, &(*game_assets)->player_torpedoes, game_assets, game_attributes);
    }
}

/**
*@brief free_assets
*@details felszabaditja a jatek asseteket a jatek bezarasa elott.
*@param [in] game_assets
*@return void
*/
void free_assets(GameAssets **game_assets){
    free_starmap((*game_assets)->star_map);
    (*game_assets)->star_map = NULL;
    free_player_ship((*game_assets)->player_ship);
    (*game_assets)->player_ship = NULL;
    free_enemy_armada((*game_assets)->enemy_armada);
    (*game_assets)->enemy_armada = NULL;
    free_torpedoes((*game_assets)->player_torpedoes);
    (*game_assets)->player_torpedoes = NULL;
    free_torpedoes((*game_assets)->enemy_torpedoes);
    (*game_assets)->enemy_torpedoes = NULL;
}

/**
*@brief free_components
*@details az osszes jatekkomponens felszabaditasaert felel.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/
void free_components(GameAssets *game_assets, GameAttributes *game_attributes){
    free(game_assets);
    free(game_attributes);
    destroy_textures();
}

/**
*@brief keep_player_time
*@details a jatekos hajo idozitesehez szukseges szamitast vegzi el
*@return int
*/
int keep_player_time(){
    return SDL_GetTicks()/50;
}

/**
*@brief keep_enemy_time
*@details az ellenseges hajok idozitesehez szukseges szamitast vegzi el
*@return int
*/
int keep_enemy_time(){
    return SDL_GetTicks()/5;
}

/**
*@brief game_loop
*@details a jatek fo vezerlesi logikaja
*@param [] game_assets
*@param [] key_map
*@param [] game_attributes
*@return void
*/
void game_loop(GameAssets **game_assets, KeyMap *key_map, GameAttributes *game_attributes){
    int player_ship_time = 0;
    int enemy_ship_time = 0;
    while(!game_attributes->isi.quit){
        player_ship_time = keep_player_time();
        enemy_ship_time = keep_enemy_time();
        user_input(&game_attributes->isi, key_map, game_attributes->id);
        clear_graphics((*game_assets));
        calculate_game_assets(game_assets, game_attributes, enemy_ship_time);
        draw_graphics(player_ship_time, (*game_assets), game_attributes);
        render_screen();
    }
}

/**
*@brief game
*@details aggregalja az osszes jatek mukodesehez szukseges logikai fuggvenyt. Amikor a game_loop kilep a ciklusabol
*felszabadit mindent es kilep az SDL2bol.
*@return void
*/
void runtime(){

    GameAttributes *game_attributes = init_game_attributes();
    GameAssets *game_assets = init_game_assets(game_attributes);
    KeyMap *key_map = default_keymap_init();
    game_loop(&game_assets, key_map, game_attributes);
    free_assets(&game_assets);
    printf("\nstarmap pointer: %p\n", game_assets->star_map);
    printf("\nplayer ship pointer: %p\n", game_assets->player_ship);
    printf("\nenemy armada pointer: %p\n", game_assets->enemy_armada);
    printf("\nplayer torpedoes pointer: %p\n", game_assets->player_torpedoes);
    printf("\nenemy torpedoes pointer: %p\n", game_assets->enemy_torpedoes);
    free_components(game_assets, game_attributes);
    free(key_map);
    SDL_Quit();
}
