/**
*@file hit_management.c
*/

#include "hit_management.h"

/**
*@brief detect_player_hit
*@details ellenorzi, hogy barmelyik ellenseges hajo talalatot kapott-e.
*@param [] game_assets
*@param [] game_attributes
*@return void
*/
void detect_player_hit(GameAssets *game_assets, GameAttributes *game_attributes){
    if(game_assets->player_torpedo != NULL){
       EnemySquadronShip **enemy_ships = game_assets->enemy_armada->enemy_armada;
        for(int i = 0; i < game_assets->enemy_armada->number_of_squadrons; i++){
            EnemySquadronShip *enemy_ship = enemy_ships[i];
            while(enemy_ship != NULL){
                TorpedoShot *torpedo = game_assets->player_torpedo;
                while(torpedo != NULL){
                    if(torpedo->x_coor < enemy_ship->ship.texture_data.texture_center_x + (enemy_ship->ship.texture_data.width/2)
                       && torpedo->x_coor > enemy_ship->ship.texture_data.texture_center_x - (enemy_ship->ship.texture_data.width/2)
                       && torpedo->y_coor < enemy_ship->ship.texture_data.texture_center_y + (enemy_ship->ship.texture_data.height/2)
                       && torpedo->y_coor > enemy_ship->ship.texture_data.texture_center_y - (enemy_ship->ship.texture_data.height/2)){
                            printf("BOOM\n");
                              //remove_torpedo_shot(&torpedo);
                    }
                    torpedo = torpedo->next_torpedo;
                }
            enemy_ship = enemy_ship->next_ship;
            }
        }
    }
}

/**
*@brief detect_enemy_hit
*@details detektalja, ha az ellenseg eltalalta a jatekos hajojat
*@param [] game_assets
*@return bool
*/
bool static detect_enemy_hit(GameAssets *game_assets){

}

/**
*@brief manage_player_hits
*@details menedzseli a jatekos altal bevitt torpedo talalatokat (csokkenti a HP-t)
*@param [] torpedo
*@param [] game_assets
*@return void
*/
void static manage_player_hits(TorpedoShot *torpedo, GameAssets *game_assets){

}

/**
*@brief manage_enemy_hits
*@details menedzseli az ellenseges talalatokat, amelyeket a jatekos hajoja kapott
*@param [] torpedo
*@param [] game_assets
*@return void
*/
void static manage_enemy_hits(TorpedoShot *torpedo, GameAssets *game_assets){

}

/**
*@brief manage_hits
*@details aggregalo fuggveny a hit management logikahoz
*@param [] game_assets
*@param [] game_attributes
*@return void
*/
void manage_hits(GameAssets *game_assets, GameAttributes *game_attributes){
    detect_player_hit(game_assets, game_attributes);
}

