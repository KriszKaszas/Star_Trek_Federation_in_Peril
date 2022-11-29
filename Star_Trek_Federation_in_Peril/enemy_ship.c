/**
*@file enemy_ship.c
*/

#include "enemy_ship.h"


/**
*@brief create_enemy_ship
*@details az ellenseges hajok generalasaert felelos fuggveny. Hivasonkent csak egy hajot general
*@param [in] ship_dtt az ellenseges hajo attributumait tartalmazo fajlbeolvasasbol szarmazo adatstruktura
*@param [in] y_coor az ellenseges hajo y koordinataja
*@return [out] EnemyShip ellenseges urhajo tipusokkal ter vissza
*/
static void init_enemy_ship(TextureData texture_data, EnemyShip **es, ShipDTT *ship_dtt, int x_coor, int y_coor)
{
    (*es)->x_coor = x_coor;
    (*es)->y_coor = y_coor;
    (*es)->texture_data = texture_data;
    (*es)->hitbox_beg_coor = (*es)->x_coor - (texture_data.width/2);
    (*es)->hitbox_end_coor = (*es)->x_coor + (texture_data.width/2);
    (*es)->centerline_y_coor = (*es)->y_coor;
    (*es)->speed = ship_dtt->speed;
    (*es)->health = ship_dtt->health;
    (*es)->damage = ship_dtt->damage;
    //printf("%d\n", (*es)->hitbox_beg_coor);
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
EnemyShip *init_enemy_armada(TextureData texture_data, ShipDTT *ship_dtt, GameAttributes *game_attributes)
{
    int x_coor = game_attributes->width/20;
    int y_coor = game_attributes->height/50;
    EnemyShip *enemy_armada = NULL;
    for(int i = 0; i < 50; i++)
    {
        if(i % 10 ==0)
        {
            y_coor += 100;
            x_coor = game_attributes->width/50;
        }
        EnemyShip *new_ship = (EnemyShip*) malloc(sizeof(EnemyShip));
        init_enemy_ship(texture_data, &new_ship, &ship_dtt, x_coor, y_coor);
        new_ship->next_ship = enemy_armada;
        if(enemy_armada != NULL)
        {
            enemy_armada->prev_ship = new_ship;
        }
        new_ship->prev_ship = NULL;
        enemy_armada = new_ship;
        x_coor += 100;

    }
    return enemy_armada;
}


/**
*@brief modify_enemy_dir
*@details az ellenseges hajok mozgasi iranyat valtoztatja az ellenkezojere
*@param [in out] armada az ellenseges hadsereget tartalmazo adatstruktura
*@return void
*//*
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
/*
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
*//*
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
*//*
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
*//*
void move_enemy_armada(EnemyArmada *armada, GameAttributes *game_attributes){
    for(int i = 0; i < armada->number_of_squadrons; i++){
        move_enemy_squadron(armada->enemy_armada[i], game_attributes, armada->squadron_dirs[i]);
    }
    manage_enemy_dirs(armada, game_attributes);
}
*/

/**
*@brief pop_enemy_ship
*@details egy hajo torleset vegzi.
*@param [] torpedo
*@return void
*/
void pop_enemy_ship(EnemyShip **enemy_ship)
{
    EnemyShip *tmp = (*enemy_ship);
    if((*enemy_ship)->prev_ship == NULL && (*enemy_ship)->next_ship == NULL)
    {
        (*enemy_ship) = NULL;
        free(tmp);
    }
    else if((*enemy_ship)->prev_ship == NULL)
    {

        (*enemy_ship) = tmp->next_ship;
        (*enemy_ship)->prev_ship = NULL;
        free(tmp);
    }
    else if((*enemy_ship)->next_ship == NULL)
    {
        (*enemy_ship)->prev_ship->next_ship = NULL;
        (*enemy_ship) = (*enemy_ship)->prev_ship;
        free(tmp);
    }
    else
    {
        (*enemy_ship)->prev_ship->next_ship = (*enemy_ship)->next_ship;
        (*enemy_ship)->next_ship->prev_ship = (*enemy_ship)->prev_ship;
        (*enemy_ship) = (*enemy_ship)->next_ship;
        free((*enemy_ship));
    }
}

/**
*@brief free_enemy_ship
*@details felszabadit egy ellenseges hajot
*@param [] enemy_armada
*@return void
*/
void free_enemy_ship(EnemyShip *enemy_ship)
{
    free(enemy_ship);
}

/**
*@brief free_enemy_armada
*@details az ellenseges hadsereg altal elfoglalt memoriaterulet felszabaditasaert felel
*@param [] enemy_armada
*@return void
*/
void free_enemy_armada(EnemyShip *enemy_armada)
{
    EnemyShip *tmp = enemy_armada;
    while(tmp != NULL){
        tmp = enemy_armada->next_ship;
        pop_enemy_ship(&enemy_armada);
        enemy_armada = tmp;
    }
    enemy_armada = NULL;
    tmp = NULL;
}

