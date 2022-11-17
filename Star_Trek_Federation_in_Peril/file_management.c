/**
*@file file_management.c
*/

#include "file_management.h"

/**
*@brief import_ship_dtt
*@details beimportalja az urhajok alapveto attributumainak listait egy adott forrasfajlbol,
*majd azokbol egy listak dinamikus tombje pointerrel ter vissza.
*@param [] filepath
*@return ShipDTT
*/
ShipDTT *import_ship_dtt(char *filepath){
    FILE *ship = fopen(filepath, "rt");
    ShipDTT **tmp_shiptypes_per_squadron[10];
    ShipDTT *tmp_enemy_ship_dtt = (ShipDTT*) malloc(sizeof(ShipDTT));
    if (ship == NULL) {
        perror("The file cannot be opened");
    }
    int ctr = 0;
    while(fscanf(ship, "%d %d %d", &tmp_enemy_ship_dtt->damage, &tmp_enemy_ship_dtt->health, &tmp_enemy_ship_dtt->speed) != EOF){
        ShipDTT *enemy_ship_dtt = (ShipDTT*) malloc(sizeof(ShipDTT));
        enemy_ship_dtt->damage = tmp_enemy_ship_dtt->damage;
        enemy_ship_dtt->health = tmp_enemy_ship_dtt->health;
        enemy_ship_dtt->speed = tmp_enemy_ship_dtt->speed;
        tmp_shiptypes_per_squadron[ctr] = enemy_ship_dtt;
        ctr++;
    }

    ShipDTT **shiptypes_per_squadron = (ShipDTT**) malloc(ctr * sizeof(ShipDTT));
    for(int i = 0; i < ctr; i++){
        shiptypes_per_squadron[i] = tmp_shiptypes_per_squadron[i];
    }
    free(tmp_enemy_ship_dtt);
    fclose(ship);
    return shiptypes_per_squadron;
}


