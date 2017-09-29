/// Assets (header)
/// (c) 2017 Jani Nykänen

#ifndef __ASSETS__
#define __ASSETS__

#include "bitmap.h"
#include "tilemap.h"

/// Load assets from an asset list file
/// < path List path
/// > 0 on success, 1 on error
int load_assets(const char* path);

/// Get bitmap by name
/// < name Bitmap name
/// > A bitmap, NULL if not exist
BITMAP* get_bitmap(const char* name);

/// Get tilemap by name
/// < name Tilemap name
/// > A tilemap, NULL if not exist
TILEMAP* get_tilemap(const char* name);

/// Destroy loaded asset files
void destroy_assets();

#endif // __ASSETS__