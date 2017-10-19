/// Tilemap (header)
/// (c) 2017 Jani Nykänen

#ifndef __TILEMAP__
#define __TILEMAP__

#include "SDL2/SDL.h"

/// Tilemap structure
typedef struct
{
    int w; /// Width
    int h; /// Height
    Uint8* layers[16]; /// Layers
    Uint8 name[32]; /// Map name
    int layerCount; /// Layer count
}
TILEMAP;

/// Load a tilemap from a file (tmx format)
/// < path Tilemap path
/// > A new tilemap
TILEMAP* load_tilemap(const char* path);

/// Remove tilemap from the memory
/// < t Tilemap to destroy
void destroy_tilemap(TILEMAP* t);

#endif // __TILEMAP__