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
*@param [in out] game_attributes
*@return GameAssets
*/
GameAssets *init_game_assets(GameAttributes *game_attributes){


    ShipDTT **squadrons = import_ship_dtt("enemy_ship.txt");
    LevelDTT level_dtt;
    level_dtt.number_of_waves = 1;
    level_dtt.number_of_squadrons = 3;
    level_dtt.shiptypes_per_squadron = squadrons;
    level_dtt.ships_per_squadron = (int*) malloc(3* sizeof(int));
    level_dtt.ships_per_squadron[0] = 3;
    level_dtt.ships_per_squadron[1] = 4;
    level_dtt.ships_per_squadron[2] = 5;

    TextureData fed_texture_data = {46, 130};
    TextureData enemy_texture_data = {111, 120};

    GameAssets *game_assets = (GameAssets*) malloc(sizeof(GameAssets));
    game_assets->star_map = starmap_init(game_attributes->width, game_attributes->height);
    game_assets->player_ship = init_player_ship(game_attributes->width, game_attributes->height, fed_texture_data, 100, 1);
    game_assets->enemy_armada = init_enemy_armada(level_dtt, enemy_texture_data, game_attributes);
    game_assets->player_torpedo = NULL;
    game_assets->quantum_torpedo = NULL;
    game_assets->enemy_torpedo = NULL;
    for(int i = 0; i < game_assets->enemy_armada->number_of_squadrons; i++){
        free(level_dtt.shiptypes_per_squadron[i]);
    }
    free(level_dtt.shiptypes_per_squadron);

    return game_assets;
}

/**
*@brief init_game_attributes
*@details inicializalja a jatek attributumait, amelyeket aztan a vezerles hasznal
*@return GameAttributes
*/
GameAttributes *init_game_attributes(){
    GameAttributes *game_attributes = (GameAttributes*) malloc(sizeof(GameAttributes));
    MousePosition mouse = {0,0};
    game_attributes->width = 1900;
    game_attributes->height = 900;
    game_attributes->isi.up = false;
    game_attributes->isi.down = false;
    game_attributes->isi.left = false;
    game_attributes->isi.right = false;
    game_attributes->isi.torpedo = false;
    game_attributes->isi.torpedo_ready = true;
    game_attributes->isi.left_mouse_button = false;
    game_attributes->isi.right_mouse_button = false;
    game_attributes->isi.mouse_position = mouse;
    game_attributes->isi.quit = false;
    game_attributes->isi.phaser_ready = true;
    game_attributes->isi.phaser_firing = false;
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
    if(game_attributes->isi.left_mouse_button && game_attributes->isi.phaser_ready && !game_attributes->isi.phaser_firing){
            fire_phaser(&game_attributes->isi, game_assets->player_ship, player_ship_time);
    }
    if(game_attributes->isi.phaser_firing && player_ship_time - game_assets->player_ship->phaser_timer < 1){
        draw_phaser(game_assets->player_ship->phaser_blast);
    }
    if(player_ship_time - game_assets->player_ship->phaser_timer > 1){
        game_attributes->isi.phaser_ready = true;
        game_attributes->isi.phaser_firing = false;
        game_attributes->isi.left_mouse_button  = false;
    }
    draw_torpedo(game_assets->player_torpedo);
    draw_torpedo(game_assets->enemy_torpedo);
    draw_enemy_ships(game_assets->enemy_armada);
    draw_player_ship(game_assets->player_ship);
    draw_crosshair(game_attributes->isi.mouse_position.mouse_x, game_attributes->isi.mouse_position.mouse_y);
}

/**
*@brief calculate_game_assets
*@details a jatek assetek mozgasat es mukodeset vezerlo szamitasok aggregalo fuggvenye
*@param [in] game_assets
*@param [in] game_attributes
*@param [in] enemy_ship_time
*@return void
*/
void calculate_game_assets(GameAssets *game_assets, GameAttributes *game_attributes, int enemy_ship_time){
    int static time = 0;
    int static shot_time = 0;
    move_player_ship(game_assets->player_ship, &game_attributes->isi, game_attributes->width, game_attributes->height);
    advance_starmap_frame(game_assets->star_map, game_attributes->width, game_attributes->height);
    if(enemy_ship_time*2 > time){
        if(!game_assets->enemy_armada->ready_to_move){
            for(int i = 0; i < game_assets->enemy_armada->number_of_squadrons; i++){
                game_assets->enemy_armada->entry_finished_per_squadron[i] = enemy_armada_entry_animation(game_assets->enemy_armada->enemy_armada[i],
                                                                                                         game_attributes, game_assets->enemy_armada->squadron_dirs[i]);

            }
            int squardon_in_place_ctr = 0;
            for(int i = 0; i < game_assets->enemy_armada->number_of_squadrons; i++){
                if(game_assets->enemy_armada->entry_finished_per_squadron[i]){
                    squardon_in_place_ctr++;
                }
            }
            if(squardon_in_place_ctr == game_assets->enemy_armada->number_of_squadrons){
                game_assets->enemy_armada->ready_to_move = true;
                modify_enemy_dir(game_assets->enemy_armada);
            }
        }
        else if(game_assets->enemy_armada->ready_to_move){
            move_enemy_armada(game_assets->enemy_armada, game_attributes);
        }

        time = enemy_ship_time*2;
    }
    if(game_attributes->isi.torpedo){
        game_assets->player_torpedo = add_torpedo_shot(game_assets->player_torpedo, 5, 2,
                         game_assets->player_ship->x_coor, game_assets->player_ship->y_coor, false, false);
        game_attributes->isi.torpedo = false;
    }
    if(game_assets->player_torpedo != NULL){
        move_torpedoes(&game_assets->player_torpedo, game_attributes);
    }
    if(game_assets->enemy_armada->ready_to_move && enemy_ship_time > shot_time){

        for(int i = random_number_in_range(0, game_assets->enemy_armada->number_of_squadrons-1); i < random_number_in_range(1, game_assets->enemy_armada->number_of_squadrons); i++){
            EnemySquadronShip *tmp = game_assets->enemy_armada->enemy_armada[i];
            int ctr = 0;
            int ctr_threshold = random_number_in_range(0, game_assets->enemy_armada->no_of_ships_per_sq[i]);
            while(tmp != NULL){
               if(ctr == ctr_threshold){
                    game_assets->enemy_torpedo = add_torpedo_shot(game_assets->enemy_torpedo,
                                                              tmp->ship.damage, 2, tmp->ship.x_coor, tmp->ship.y_coor, true, false);

                }
                tmp = tmp->next_ship;
                ctr++;
            }
            ctr_threshold = random_number_in_range(0, game_assets->enemy_armada->no_of_ships_per_sq[i]);
        }
        shot_time += 50;
    }
     if(game_assets->enemy_torpedo != NULL){
        move_torpedoes(&game_assets->enemy_torpedo, game_attributes);
    }
}

/**
*@brief free_assets
*@details felszabaditja a jatek asseteket a jatek bezarasa elott.
*@param [in] game_assets
*@return void
*/
void free_assets(GameAssets *game_assets){
    free_starmap(game_assets->star_map);
    free_player_ship(game_assets->player_ship);
    free_enemy_armada(game_assets->enemy_armada);
    free_torpedoes(game_assets->player_torpedo);
    free_torpedoes(game_assets->enemy_torpedo);
    free_torpedoes(game_assets->quantum_torpedo);
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
void game_loop(GameAssets *game_assets, KeyMap *key_map, GameAttributes *game_attributes){
    int player_ship_time = 0;
    int enemy_ship_time = 0;
    for(int i = 0; i < game_assets->enemy_armada->number_of_squadrons; i++){
        position_enemy_armada(game_assets->enemy_armada->enemy_armada[i], game_attributes, game_assets->enemy_armada->squadron_dirs[i]);
    }
    while(!game_attributes->isi.quit){
        if(game_assets->player_torpedo != NULL || game_assets->enemy_torpedo != NULL || game_assets->quantum_torpedo != NULL){
            manage_hits(game_assets, game_attributes);
        }
        player_ship_time = keep_player_time();
        enemy_ship_time = keep_enemy_time();
        user_input(&game_attributes->isi, key_map, game_attributes->id);
        clear_graphics(game_assets);
        calculate_game_assets(game_assets, game_attributes, enemy_ship_time);
        draw_graphics(player_ship_time, game_assets, game_attributes);
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
    game_loop(game_assets, key_map, game_attributes);

    free_assets(game_assets);
    free_components(game_assets, game_attributes);
    free(key_map);
    SDL_Quit();
}
