/**
*@file enemy_ship.c
*/

#include "enemy_ship.h"

/**
*@brief set_squadron_heights
*@details az ellenseges hajok y koordinatait allitja be soronkent
*@param [in] enemy_spawn_size a jatekteren belul annak a teruletnek a magassaga, ahol az ellenseges urhajok spawnolhatnak
*@param [in] num_of_squadrons jeloli, hogy hany sorban spawnolnak az ellenseges hajok
*@param [in] squadron_entry_dirs az ellenseges hajok mozgasi iranya
*@return [out] int az ellenseges hajok magassagaval ter vissza
*/
int static *set_squadron_heights(int enemy_spawn_size, int num_of_squadrons, int *squadron_entry_dirs){

    int division = enemy_spawn_size/num_of_squadrons;
    int height = division/2;

    int *squadron_heights = (int*) malloc(num_of_squadrons * sizeof(int));
    for(int i = 0; i < num_of_squadrons; i++){
        if(squadron_entry_dirs[i] == 2){
            squadron_heights[i] = -500;
            height += division;
        }
        else{
            squadron_heights[i] = height;

            height += division;

        }
    }
    return squadron_heights;
}

/**
*@brief create_enemy_ship
*@details az ellenseges hajok generalasaert felelos fuggveny. Hivasonkent csak egy hajot general
*@param [in] ship_dtt az ellenseges hajo attributumait tartalmazo fajlbeolvasasbol szarmazo adatstruktura
*@param [in] y_coor az ellenseges hajo y koordinataja
*@return [out] EnemyShip ellenseges urhajo tipusokkal ter vissza
*/
EnemyShip create_enemy_ship(ShipDTT *ship_dtt, int y_coor){
    EnemyShip es;
    es.x_coor = -10;
    es.y_coor = y_coor;
    es.speed = ship_dtt->speed;
    es.health = ship_dtt->health;
    es.damage = ship_dtt->damage;
    return es;
}

/**
*@brief init_enemy_armada
*@details az ellenseges hadsereget inicializalja a bemeneti parameterek alapjan.
*<br>VIGYAZAT: a hadsereg es az azt alkoto hajok felszabaditasaert a hivo felel!
*<br>MEGJEGYZES: ez a fuggveny es a segedfuggvenyei a vegleges verziohoz refaktoralason esnek majd at,
*a fuggveny ajelenlegi verzioban mukodik ugyan, de nem azt a feladatot latja el, amit majd a vegleges verzioban kell.
*@param [in] level_dtt az ellenseges hadsereg attributumait tartalmazo fajlbeolvasasbol szarmazo adatstruktura
*@param [in] texture_data az egyes hajok textura adatai a kirajzolashoz (kozep koordinatak, textura szelesseg)
*@param [in] game_attributes az osszes jatekattributumot tartlmazo adatstruktura
*@return [out] EnemyArmada az ellenseges hajokkal ter vissza
*/
EnemyArmada *init_enemy_armada(LevelDTT *level_dtt, TextureData texture_data, GameAttributes *game_attributes){
    EnemyArmada *enemy_armada = (EnemyArmada*) malloc(sizeof(EnemyArmada));
    EnemySquadronShip **enemy_squadrons = (EnemySquadronShip**) malloc(level_dtt->number_of_squadrons * sizeof(EnemySquadronShip));
    int enemy_spawn_size = (game_attributes->height/3)*2;

    bool *entry_fin_per_sq = (bool*) malloc(level_dtt->number_of_squadrons * sizeof(bool));
    int *squadron_dirs = (int*) malloc(level_dtt->number_of_squadrons * sizeof(int));
    for(int i = 0; i < level_dtt->number_of_squadrons; i++){
        enemy_squadrons[i] = NULL;
        entry_fin_per_sq[i] = false;
    }
    squadron_dirs[0] = 2;
    int decide_dir = random_number_in_range(1, 2);
    squadron_dirs[1] = decide_dir == 1 ? 1 : -1;;
    for(int i = 2; i < level_dtt->number_of_squadrons; i++){
        if(squadron_dirs[i-1] == 1){
            squadron_dirs[i] = -1;
        }
        else{
            squadron_dirs[i] = 1;
        }
    }

    int *squadron_height = set_squadron_heights(enemy_spawn_size, level_dtt->number_of_squadrons, squadron_dirs);
    for(int i = 0; i < level_dtt->number_of_squadrons; i++){

        for(int j = 0; j < level_dtt->ships_per_squadron[i]; j++){
            EnemySquadronShip *enemy_squadron_ship = (EnemySquadronShip*) malloc(sizeof(EnemySquadronShip));
            enemy_squadron_ship->ship = create_enemy_ship(level_dtt->shiptypes_per_squadron[i], squadron_height[i]);
            enemy_squadron_ship->ship.texture_data = texture_data;
            enemy_squadron_ship->ship.texture_data.texture_center_x = enemy_squadron_ship->ship.x_coor - (enemy_squadron_ship->ship.texture_data.width/2);
            enemy_squadron_ship->ship.texture_data.texture_center_y = enemy_squadron_ship->ship.y_coor - (enemy_squadron_ship->ship.texture_data.height/2);
            enemy_squadron_ship->next_ship = enemy_squadrons[i];
            enemy_squadron_ship->prev_ship = NULL;
            if(enemy_squadrons[i] != NULL){
                enemy_squadrons[i]->prev_ship = enemy_squadron_ship;
            }
            enemy_squadrons[i] = enemy_squadron_ship;
        }
    }
    enemy_armada->enemy_armada = enemy_squadrons;
    enemy_armada->number_of_squadrons = level_dtt->number_of_squadrons;
    enemy_armada->no_of_ships_per_sq = level_dtt->ships_per_squadron;
    enemy_armada->squadron_dirs = squadron_dirs;
    enemy_armada->squadron_heights = squadron_height;
    enemy_armada->entry_finished_per_squadron = entry_fin_per_sq;
    enemy_armada->ready_to_move = false;
    return enemy_armada;
}

/**
*@brief squadron_size
*@details egy ellenseges hajo-sort tartalmazo lancolt lista alapjan visszater az adott sorban levo hajok szamaval
*@param [in] squadron egy adott sor ellenseges hajot tarolo lancolt lista head pointere
*@return [out] int visszater egy sor hajo meretevel
*/
int squadron_size(EnemySquadronShip *squadron){
    EnemySquadronShip *tmp = squadron;
    int ctr = 1;
    while(tmp->next_ship != NULL){
        ctr++;
        tmp = tmp->next_ship;
    }
    return ctr;
}

/**
*@brief position_enemy_armada
*@details az ellenseges hajokat a kepernyon kivul elhelyezi, elokeszitve azokat jatek kezdeti animaciojara
*@param [in out] squadron egy hajo-sor egy elemere mutato lancolt lista elem pointer
*@param [in] game_attributes az osszes jatekattributumot tartlmazo adatstruktura
*@param [in] dir a hajo x iranyu mozgasi iranya
*@return [out] void
*/
void position_enemy_armada(EnemySquadronShip *squadron, GameAttributes *game_attributes, int dir){
    EnemySquadronShip *tmp = squadron;
    switch (dir){
    case 1:
        tmp->ship.x_coor = -20;
        tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
        tmp = tmp->next_ship;
        while(tmp != 0){
            tmp->ship.x_coor = tmp->prev_ship->ship.x_coor - game_attributes->width/squadron_size(squadron);
            tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
            tmp = tmp->next_ship;
        }
        break;
    case -1:
        tmp->ship.x_coor = game_attributes->width+20;
        tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
        tmp = tmp->next_ship;
        while(tmp != 0){
            tmp->ship.x_coor = tmp->prev_ship->ship.x_coor + game_attributes->width/squadron_size(squadron);
            tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
            tmp = tmp->next_ship;
        }
        break;
    case 2:
        tmp->ship.x_coor = game_attributes->width/squadron_size(squadron)/2;
        tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
        tmp = tmp->next_ship;
        while(tmp != 0){
            tmp->ship.x_coor = tmp->prev_ship->ship.x_coor + game_attributes->width/squadron_size(squadron);
            tmp->ship.texture_data.texture_center_x = tmp->ship.x_coor - (tmp->ship.texture_data.width/2);
            tmp = tmp->next_ship;
        }
        break;
    }
}

/**
*@brief enemy_armada_entry_animation
*@details a jatek kezdo animaciojat vezerli.
*@param [in out] squadron egy hajo-sor egy elemere mutato lancolt lista elem pointer
*@param [in] game_attributes az osszes jatekattributumot tartlmazo adatstruktura
*@param [in] dir a hajo x iranyu mozgasi iranya
*@return [out] bool true ertekkel ter vissza, ha a kezdo animacio befejezodott.
*/
bool enemy_armada_entry_animation(EnemySquadronShip *squadron, GameAttributes *game_attributes, int dir){
    EnemySquadronShip *tmp = squadron;
    bool is_squadron_entry_finished = false;
    switch (dir){
    case 1:
        if(squadron->ship.x_coor < game_attributes->width-(game_attributes->width/squadron_size(squadron))/2+1){
            while(tmp != 0){
                tmp->ship.x_coor += tmp->ship.speed*3;
                tmp->ship.texture_data.texture_center_x += tmp->ship.speed*3;
                tmp = tmp->next_ship;
            }
        }
        else{
            is_squadron_entry_finished = true;
        }
        break;
    case -1:
        if(squadron->ship.x_coor > game_attributes->width/squadron_size(squadron)/2-1){
            while(tmp!= 0){
                tmp->ship.x_coor -= tmp->ship.speed*3;
                tmp->ship.texture_data.texture_center_x -= tmp->ship.speed*3;
                tmp = tmp->next_ship;
             }
        }
        else{
            is_squadron_entry_finished = true;
        }
        break;
    case 2:
        if(squadron->ship.y_coor < game_attributes->height/8){
            while(tmp != 0){
                tmp->ship.y_coor +=  tmp->ship.speed;
                tmp->ship.texture_data.texture_center_y += tmp->ship.speed;
                tmp = tmp->next_ship;
             }
        }
        else{
            is_squadron_entry_finished = true;
        }
        break;
    }
    return is_squadron_entry_finished;
}

/**
*@brief modify_enemy_dir
*@details az ellenseges hajok mozgasi iranyat valtoztatja az ellenkezojere
*@param [in out] armada az ellenseges hadsereget tartalmazo adatstruktura
*@return void
*/
void modify_enemy_dir(EnemyArmada *armada){
    for(int i = 0; i < armada->number_of_squadrons; i++){
        if(armada->squadron_dirs[i] == 2){
            if(i != armada->number_of_squadrons){
                armada->squadron_dirs[i] = armada->squadron_dirs[i+1]*(-1);
            }
            else if(i == armada->number_of_squadrons){
                armada->squadron_dirs[i] = armada->squadron_dirs[i-1]*(-1);
            }
        }
    }
}

/**
*@brief calculate_enemy_movement_freedom
*@details megadja, hogy az ellenseges hajok mennyit mozoghatnak x iranyba a jatek csata animacioja kozben
*@param [in] game_attributes az osszes jatekattributumot tartlmazo adatstruktura
*@param [in] squadron egy hajo-sort tartalmazo lancolt lista head pointer
*@return int visszater az ellenseges urhajok mozgasi szabadsagaval
*/

int static calculate_enemy_movement_freedom(GameAttributes *game_attributes, EnemySquadronShip *squadron){
    int squadron_size = 0;
    EnemySquadronShip *tmp = squadron;
    while(tmp != NULL){
        squadron_size++;
        tmp = tmp->next_ship;
    }
    return game_attributes->width/squadron_size/3;
}

/**
*@brief manage_enemy_dirs
*@details visszaforditja az ellenseges hajokat a jatek csata animacioja kozben, amennyiben azok elertek a mozgasi szabadsaguk veget
*jelzo x koordinatat
*@param [] armada
*@param [] game_attributes
*@return void
*/
void manage_enemy_dirs(EnemyArmada *armada, GameAttributes *game_attributes){
    int enemy_movement_freedom;
    for(int i = 0; i < armada->number_of_squadrons; i++){
        enemy_movement_freedom = calculate_enemy_movement_freedom(game_attributes, armada->enemy_armada[i]);
        if(armada->enemy_armada[i]->ship.x_coor > game_attributes->width/2){
            if(armada->squadron_dirs[i] > 0 &&
               armada->enemy_armada[i]->ship.x_coor == game_attributes->width-enemy_movement_freedom){
                   armada->squadron_dirs[i] *= -1;
            }
            else if(armada->squadron_dirs[i] < 0 &&
                    armada->enemy_armada[i]->ship.x_coor == game_attributes->width - (enemy_movement_freedom * 2)){
                        armada->squadron_dirs[i] *= -1;
                    }
        }
        else if(armada->enemy_armada[i]->ship.x_coor < game_attributes->width/2){
                if(armada->squadron_dirs[i] > 0 &&
                   armada->enemy_armada[i]->ship.x_coor == enemy_movement_freedom * 2){
                        armada->squadron_dirs[i] *= -1;
                }
                else if(armada->squadron_dirs[i] < 0 &&
                        armada->enemy_armada[i]->ship.x_coor == enemy_movement_freedom){
                        armada->squadron_dirs[i] *= -1;
                        }
        }
    }
}

/**
*@brief move_enemy_squadron
*@details az ellenseges hajok egy soranak mozgatasaert felel
*@param [] squadron
*@param [] game_attributes
*@param [] dir
*@return void
*/
void static move_enemy_squadron(EnemySquadronShip *squadron, GameAttributes *game_attributes, int dir){
    EnemySquadronShip *tmp = squadron;
    while(tmp != NULL){
        tmp->ship.x_coor += tmp->ship.speed*dir;
        tmp->ship.texture_data.texture_center_x += tmp->ship.speed*dir;
        tmp = tmp->next_ship;
    }
}

/**
*@brief move_enemy_armada
*@details az egesz ellenseges hadsereg mozgasaert felel
*@param [] armada
*@param [] game_attributes
*@return void
*/
void move_enemy_armada(EnemyArmada *armada, GameAttributes *game_attributes){
    for(int i = 0; i < armada->number_of_squadrons; i++){
        move_enemy_squadron(armada->enemy_armada[i], game_attributes, armada->squadron_dirs[i]);
    }
    manage_enemy_dirs(armada, game_attributes);
}

/**
*@brief free_enemy_squadron_ship
*@details az ellenseges hajok altal elfoglalt memoriaterulet egyenkenti felszabaditasaert felel
*@param [] ess
*@return void
*/
void free_enemy_squadron_ship(EnemySquadronShip **ess){
    EnemySquadronShip *ship = (*ess);
    if((*ess)->prev_ship == NULL && (*ess)->next_ship == NULL){
        (*ess) = NULL;
        free(ship);
    }
    else if((*ess)->prev_ship == NULL){
        if((*ess)->next_ship != NULL){
            (*ess) = ship->next_ship;
        }
        free(ship);
        (*ess)->prev_ship = NULL;
    }
    else if((*ess)->next_ship == NULL){
        if((*ess)->prev_ship != NULL){
            (*ess)->prev_ship->next_ship = NULL;
        }
        free(ship);
    }
    else{
        (*ess)->prev_ship->next_ship = (*ess)->next_ship;
        (*ess)->next_ship->prev_ship = (*ess)->prev_ship;
        free(ship);
    }
}

/**
*@brief free_enemy_squadron
*@details az ellenseges hajok altal soronkent elfoglalt memoriaterulet felszabaditasaert felel
*@param [] squadron
*@return void
*/
void free_enemy_squadron(EnemySquadronShip *squadron){
    EnemySquadronShip *ship = squadron;
    while(ship != NULL){
        ship = squadron->next_ship;
        free_enemy_squadron_ship(&squadron);
        squadron = ship;
    }
}

/**
*@brief free_enemy_armada
*@details az ellenseges hadsereg altal elfoglalt memoriaterulet felszabaditasaert felel
*@param [] armada
*@return void
*/
void free_enemy_armada(EnemyArmada *armada){
    for(int i = 0; i < armada->number_of_squadrons; i++){
        free_enemy_squadron(armada->enemy_armada[i]);
        armada->enemy_armada[i] = NULL;
    }
    free(armada->squadron_heights);
    armada->squadron_heights = NULL;
    free(armada->squadron_dirs);
    armada->squadron_dirs = NULL;
    free(armada->entry_finished_per_squadron);
    armada->entry_finished_per_squadron = NULL;
    free(armada->enemy_armada);
    armada->enemy_armada = NULL;
    free(armada->no_of_ships_per_sq);
    armada->no_of_ships_per_sq = NULL;
    free(armada);
    armada = NULL;
}
