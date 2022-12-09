/**
*@file enemy_ship.c
*/

#include "enemy_ship.h"

static void pop_only_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship);

static void pop_first_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship);

static void pop_last_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship);

static void pop_in_between_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship);

/**
*@brief create_enemy_ship
*@details az ellenseges hajok generalasaert felelos fuggveny. Hivasonkent csak egy hajot general
*@param [in] ship_dtt az ellenseges hajo attributumait tartalmazo fajlbeolvasasbol szarmazo adatstruktura
*@param [in] y_coor az ellenseges hajo y koordinataja
*@return [out] EnemyShip ellenseges urhajo tipusokkal ter vissza
*/
static void init_enemy_ship(TextureData texture_data, SpriteMapData sprite_map_data, EnemyShip **es, ShipDTT *ship_dtt, int x_coor, int y_coor)
{
    (*es)->x_coor = x_coor;
    (*es)->y_coor = y_coor;
    (*es)->movement_dir = 1;
    (*es)->texture_data = texture_data;
    (*es)->sprite_map_data = sprite_map_data;
    (*es)->texture_data.texture_center_x = (*es)->x_coor - ((*es)->texture_data.width/2);
    (*es)->texture_data.texture_center_y = (*es)->y_coor - ((*es)->texture_data.height/2);
    (*es)->hitbox_beg_coor = (*es)->x_coor - (texture_data.width/2);
    (*es)->hitbox_end_coor = (*es)->x_coor + (texture_data.width/2);
    (*es)->centerline_y_coor = (*es)->y_coor;
    (*es)->speed = ship_dtt->speed;
    (*es)->health = ship_dtt->health;
    (*es)->score_value = ship_dtt->score_value;

}

/**
*@brief init_enemy_armada
*@details az ellenseges hadsereget inicializalja a bemeneti parameterek alapjan.
*<br>VIGYAZAT: a hadsereg es az azt alkoto hajok felszabaditasaert a hivo felel!
*@param [in] level_dtt az ellenseges hadsereg attributumait tartalmazo fajlbeolvasasbol szarmazo adatstruktura
*@param [in] texture_data az egyes hajok textura adatai a kirajzolashoz (kozep koordinatak, textura szelesseg)
*@param [in] game_attributes az osszes jatekattributumot tartlmazo adatstruktura
*@return [out] EnemyArmada az ellenseges hajokkal ter vissza
*/
EnemyShip *init_enemy_armada(TextureData texture_data, SpriteMapData sprite_map_data, ShipDTT **ship_dtt, GameAttributes *game_attributes)
{

    int spacing = game_attributes->width/13;
    int x_start_pos = spacing/2;
    int x_coor = x_start_pos;
    int y_coor = game_attributes->height/12;
    EnemyShip *enemy_armada = NULL;
    for(int i = 0; i < game_attributes->num_of_rows; i++)
    {
        for(int j = 0; j < game_attributes->enemy_ships_per_row; j++)
        {
            EnemyShip *new_ship = (EnemyShip*) malloc(sizeof(EnemyShip));
            init_enemy_ship(texture_data, sprite_map_data, &new_ship, ship_dtt[i], x_coor, y_coor);
            new_ship->next_ship = enemy_armada;
            if(enemy_armada != NULL)
            {
                enemy_armada->prev_ship = new_ship;
            }
            new_ship->prev_ship = NULL;
            enemy_armada = new_ship;
            x_coor += spacing;
        }
        y_coor += 120;
        x_coor = x_start_pos;
    }
    return enemy_armada;
}

void move_enemy_armada(EnemyShip *enemy_armada, GameAttributes *game_attributes)
{
    EnemyShip *temp_ship = enemy_armada;
    int max_x_threshold = (game_attributes->width/26)*25;
    int min_x_threshold = game_attributes->width/26;
    int armada_max_x = find_max_enemy_armada_x_coor(enemy_armada);
    int armada_min_x = find_min_enemy_armada_x_coor(enemy_armada);
    while(temp_ship != NULL)
    {
        bool is_armada_past_max_x_threshold = armada_max_x > max_x_threshold;
        bool is_armada_past_min_x_threshold = armada_min_x < min_x_threshold;
        bool is_dir_right = temp_ship->movement_dir == 1;
        bool is_dir_left = temp_ship->movement_dir == -1;
        bool is_armada_at_max_edge = is_dir_right && is_armada_past_max_x_threshold;
        bool is_armada_at_min_edge = is_dir_left && is_armada_past_min_x_threshold;
        bool is_armada_at_threshold_edge = is_armada_at_max_edge || is_armada_at_min_edge;

        if(is_dir_right)
        {
            move_right(temp_ship);
        }
        if(is_dir_left)
        {
            move_left(temp_ship);
        }
        if(is_armada_at_threshold_edge)
        {
            for(int i = 0; i < 4; i++)
            {
                move_down(temp_ship);
            }
            reverse_movement_dir(temp_ship);
        }
        temp_ship = temp_ship->next_ship;

    }
}

int find_max_enemy_armada_x_coor(EnemyShip *enemy_armada)
{
    EnemyShip *temp_armada = enemy_armada;
    int max_x_coor = 0;
    while(temp_armada != NULL)
    {
        bool is_x_greater_than_max = temp_armada->x_coor >= max_x_coor;
        if(is_x_greater_than_max)
        {
            max_x_coor = temp_armada->x_coor;
        }
        temp_armada = temp_armada->next_ship;
    }
    return max_x_coor;
}

int find_min_enemy_armada_x_coor(EnemyShip *enemy_armada)
{
    EnemyShip *temp_armada = enemy_armada;
    int min_x_coor = 1000;
    while(temp_armada != NULL)
    {
        bool is_x_less_than_min = temp_armada->x_coor <= min_x_coor;
        if(is_x_less_than_min)
        {
            min_x_coor = temp_armada->x_coor;
        }
        temp_armada = temp_armada->next_ship;
    }
    return min_x_coor;
}


void move_right(EnemyShip *enemy_ship)
{
    enemy_ship->x_coor += enemy_ship->speed;
    enemy_ship->texture_data.texture_center_x += enemy_ship->speed;
    enemy_ship->hitbox_beg_coor += enemy_ship->speed;
    enemy_ship->hitbox_end_coor += enemy_ship->speed;
}

void move_left(EnemyShip *enemy_ship)
{
    enemy_ship->x_coor -= enemy_ship->speed;
    enemy_ship->texture_data.texture_center_x -= enemy_ship->speed;
    enemy_ship->hitbox_beg_coor -= enemy_ship->speed;
    enemy_ship->hitbox_end_coor -= enemy_ship->speed;
}

void move_down(EnemyShip *enemy_ship)
{
    enemy_ship->y_coor += enemy_ship->speed;
    enemy_ship->texture_data.texture_center_y += enemy_ship->speed;
    enemy_ship->centerline_y_coor += enemy_ship->speed;
}

void reverse_movement_dir(EnemyShip *enemy_ship)
{
    enemy_ship->movement_dir *= -1;
}

/**
*@brief pop_enemy_ship
*@details Egy hajo torleset vegzi.
*@param [in,out] enemy_ship
*@return void
*/
void pop_enemy_ship(EnemyShip **enemy_ship)
{
    EnemyShip *temp_ship = (*enemy_ship);
    bool is_ship_null = temp_ship == NULL;

    if(is_ship_null)
    {
        return;
    }

    bool is_first_ship = (*enemy_ship)->prev_ship == NULL;
    bool is_last_ship = (*enemy_ship)->next_ship == NULL;
    bool is_only_ship = is_first_ship && is_last_ship;
    bool is_in_between_ship = !is_first_ship && !is_last_ship;

    if(is_only_ship)
    {
        pop_only_ship(enemy_ship, temp_ship);
        return;
    }
    if(is_first_ship)
    {
        pop_first_ship(enemy_ship, temp_ship);
        return;
    }
    if(is_last_ship)
    {
        pop_last_ship(enemy_ship, temp_ship);
        return;
    }
    if(is_in_between_ship)
    {
        pop_in_between_ship(enemy_ship, temp_ship);
        return;
    }
}

/**
*@brief pop_only_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista egyeduli eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] temp_ship Az ellenseges hajot tartalmazo lancolt lista aktualis elemenek ideiglenes taroloja.
*@return void
*/

static void pop_only_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship)
{
    (*enemy_ship) = NULL;
    free(temp_ship);
}

/**
*@brief remove_first_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista head eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] temp_ship Az ellenseges hajot tartalmazo lancolt lista aktualis elemenek ideiglenes taroloja.
*@return void
*/

static void pop_first_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship)
{
    (*enemy_ship) = temp_ship->next_ship;
    (*enemy_ship)->prev_ship = NULL;
    free(temp_ship);
}

/**
*@brief pop_last_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista tail eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] temp_ship Az ellenseges hajot tartalmazo lancolt lista aktualis elemenek ideiglenes taroloja.
*@return void
*/

static void pop_last_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship)
{
    (*enemy_ship)->prev_ship->next_ship = NULL;
    (*enemy_ship) = (*enemy_ship)->prev_ship;
    free(temp_ship);
}

/**
*@brief pop_in_between_ship
*@details Felszabaditja a felrobbant ellenséges hajot, ha az a hajo lista koztes eleme.
*@param [in,out] enemy_ship Az ellenseges hajotkat tartalmazo lancolt lista aktualis elemenek pointere.
*@param [in] temp_ship Az ellenseges hajot tartalmazo lancolt lista aktualis elemenek ideiglenes taroloja.
*@return void
*/

static void pop_in_between_ship(EnemyShip **enemy_ship, EnemyShip *temp_ship)
{
        (*enemy_ship)->prev_ship->next_ship = (*enemy_ship)->next_ship;
        (*enemy_ship)->next_ship->prev_ship = (*enemy_ship)->prev_ship;
        (*enemy_ship) = (*enemy_ship)->next_ship;
        free(temp_ship);
}

/**
*@brief free_enemy_armada
*@details Az ellenseges hadsereg altal elfoglalt memoriaterulet felszabaditasaert felel
*@param [] enemy_armada
*@return void
*/
void free_enemy_armada(EnemyShip *enemy_armada)
{
    EnemyShip *temp_ship = enemy_armada;
    while(temp_ship != NULL){
        temp_ship = enemy_armada->next_ship;
        pop_enemy_ship(&enemy_armada);
        enemy_armada = temp_ship;
    }
    enemy_armada = NULL;
    temp_ship = NULL;
}

