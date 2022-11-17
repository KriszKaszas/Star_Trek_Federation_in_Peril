#ifndef TEXTURE_DATA_H_INCLUDED
#define TEXTURE_DATA_H_INCLUDED

#include "debugmalloc.h"

/**
*@brief SpriteMapData
*@details
*/
typedef struct spritemapdata{
    int x_coor; ///<
    int y_coor; ///<
    int width; ///<
    int hight; ///<
}SpriteMapData;


/**
*@brief TextureData
*@details
*/
typedef struct texturedata{
    int width; ///<
    int height; ///<
    int texture_center_x; ///<
    int texture_center_y; ///<
}TextureData;

#endif // TEXTURE_DATA_H_INCLUDED
