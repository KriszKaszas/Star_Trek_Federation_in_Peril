/**
*@file file_management.c
*/

#include "file_management.h"

/**
*@brief import_ship_dtt
*@details Beolvassa az urhajok alapveto attributumainak listait egy adott forrasfajlbol,
*majd azokbol egy ShipDTT tagokbol allo dinamikus tomb pointerevel ter vissza.
*@param [in] filename A megnyitando file neve.
*@param [out] num_of_rows Az urhajok listajaban talalhato sorok szamaval ter vissza.
*@return enemy_armada ShipDTT-ket tartalmazo lista pointere.
*/
ShipDTT **import_ship_dtt(char *filename, int *num_of_rows)
{
    FILE *ship_data = fopen(filename, "rt");
    if (ship_data == NULL)
    {
        perror("The file cannot be opened");
        return NULL;
    }
    int num_of_dtt = 0;
    ShipDTT *temp_enemy_ship_dtt = (ShipDTT*) malloc(sizeof(ShipDTT));
    ShipDTT **enemy_armada = (ShipDTT**) malloc(num_of_dtt*sizeof(ShipDTT));

    int ctr = 0;
    while(fscanf(ship_data, "%d %d %d", &temp_enemy_ship_dtt->speed, &temp_enemy_ship_dtt->health, &temp_enemy_ship_dtt->score_value) != EOF)
    {
        num_of_dtt++;
        enemy_armada = (ShipDTT**) realloc(enemy_armada, num_of_dtt*sizeof(ShipDTT));
        ShipDTT *enemy_ship_dtt = (ShipDTT*) malloc(sizeof(ShipDTT));
        enemy_ship_dtt->speed = temp_enemy_ship_dtt->speed;
        enemy_ship_dtt->health = temp_enemy_ship_dtt->health;
        enemy_ship_dtt->score_value = temp_enemy_ship_dtt->score_value;
        enemy_armada[ctr] = enemy_ship_dtt;
        ctr++;
    }
    if(num_of_dtt == 0)
    {
        free(enemy_armada);
    }
    free(temp_enemy_ship_dtt);
    fclose(ship_data);
    if(num_of_rows != NULL)
    {
        (*num_of_rows) = ctr;
    }
    return enemy_armada;
}

/**
*@brief read_texture_data
*@details Beolvassa az urhajok texturaihoz szukseges adatokat.
*majd azokbol egy ShipDTT tagokbol allo dinamikus tomb pointerevel ter vissza.
*@param [in] filename A megnyitando file neve.
*@param [out] texture_data Az urhajok megjelenitesehez szukseges textura adatok.
*@param [out] SpriteMapData Az urhajok texturainak beolvasasahoz szukseges meretek.

*@return void
*/

void read_texture_data(char *filename, TextureData *texture_data, SpriteMapData *sprite_map_data)
{
    FILE *ship_sprite_data = fopen(filename, "rt");
    if (ship_sprite_data == NULL)
    {
        perror("The file cannot be opened");
        return;
    }

    fscanf(ship_sprite_data, "%d %d %d %d", &sprite_map_data->x_coor,
                                               &sprite_map_data->y_coor,
                                               &sprite_map_data->width,
                                               &sprite_map_data->height);
    fscanf(ship_sprite_data, "%d %d", &texture_data->width, &texture_data->height);
    fclose(ship_sprite_data);
}
