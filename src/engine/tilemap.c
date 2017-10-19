/// Tilemap (header)
/// (c) 2017 Jani Nykänen

#include "tilemap.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"

#ifdef LIBTMX_ENABLED

#include "tmx.h"

/// Map name
static char mapName[32];

/// Get property
static void get_prop(tmx_property* p, void* depth)
{
    if(p != NULL)
    {
        if(p->type == PT_STRING && strcmp(p->name,"name") == 0)
        {
            // Clear name
            int i = 0;
            for(; i < 32; i++)
                mapName[i] = '\0';

            strcpy(mapName,p->value.string);
        }
    }
    
}

/// Load a tilemap from a file
TILEMAP* load_tilemap(const char* path)
{
    // Allocate memory
    TILEMAP* t = (TILEMAP*)malloc(sizeof(TILEMAP));
    if(t == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }

    // Open map file
    tmx_map* map = tmx_load(path);
    if(map == NULL) 
    {
        printf("Failed to open a tilemap in %s!\n",path);
        return NULL;
    }

    // Store dimensions
    t->w = map->width;
    t->h = map->height;

    tmx_tile* tile;
    int l = 0;
    int i = 0;
    while (map->ly_head) 
    {
        t->layers[l] = (Uint8*)malloc(sizeof(Uint8) * t->w * t->h);
        if(t->layers[l] == NULL)
        {
            tmx_map_free(map);
            printf("Memory allocation error!\n");
            return NULL;
        }

        // Clear layer
        for(i=0; i < t->w*t->h; i++)
        {
            t->layers[l] [i] = 0;
        }

        // Copy layer data
        for(i=0; i < t->w*t->h; i++)
        {
            tile = map->tiles[map->ly_head->content.gids[i]];
            if(tile != NULL)
            {
                t->layers[l] [i] = tile->id +1;
            }
        }

        map->ly_head = map->ly_head->next;

        l++;
    }   
    t->layerCount = l;
    
    tmx_property_foreach(map->properties, get_prop, (void*)0);

    strcpy((char*)t->name,mapName);

    // Free map as it's no longer needed
    tmx_map_free(map);

    return t;
}

#else


/// Load a tilemap from a binary file
TILEMAP* load_tilemap(const char* path)
{
    // Allocate memory
    TILEMAP* t = (TILEMAP*)malloc(sizeof(TILEMAP));
    if(t == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }
    // Load file
    FILE* f = fopen(path,"rb");
    if(f == NULL)
    {
        printf("Failed to open a file in %s!\n",path);
        free(t);
        return NULL;
    }

    // Read magic byte
    char b;
    fread(&b,1,1,f);
    // Ignore it for now


    // Read layer count
    Uint8 c;
    fread(&c,1,1,f);
    t->layerCount = (int)c;

    // Read name length
    Uint8 nl;
    fread((char*) &nl,1,1,f);

    // Read name
    fread(t->name,1,nl,f);
    t->name[nl] = '\0';

    // Read dimensions
    fread(&t->w,sizeof(int),1,f);
    fread(&t->h,sizeof(int),1,f);

    // Allocate memory for layers and read data to them
    int i = 0;
    for(; i < t->layerCount; i++)
    {
        t->layers[i] = (Uint8*)malloc(sizeof(Uint8) * t->w * t->h);
        if(t->layers[i] == NULL)
        {
            free(t);
            printf("Memory allocation error!\n");
            return NULL;
        }
        fread(t->layers[i],1,t->w*t->h,f);
    }
    
    fclose(f);

    return t;
}


#endif

/// Remove tilemap from the memory
void destroy_tilemap(TILEMAP* t)
{
    if(t == NULL) return;

    int i = 0;
    for(; t->layers[i] != NULL; i++)
    {
        free(t->layers[i]);
    }

    free(t);
}