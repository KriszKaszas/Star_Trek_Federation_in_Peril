#ifndef FILE_MANAGEMENT_H_INCLUDED
#define FILE_MANAGEMENT_H_INCLUDED

#include "data_transfer_types.h"
#include "texture_data.h"
#include "keymap.h"

#include <stdio.h>

#include "debugmalloc.h"

ShipDTT **import_ship_dtt(char *filename, int *num_of_rows);

void read_texture_data(char *filename,
                       TextureData *texture_data,
                       SpriteMapData *sprite_map_data);

#endif // FILE_MANAGEMENT_H_INCLUDED
