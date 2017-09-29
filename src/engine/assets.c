/// Assets (source)
/// (c) 2017 Jani Nykänen

#include "assets.h"

#include "SDL2/SDL.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#include "list.h"

/// Asset type enum
enum
{
    T_BITMAP = 0,
    T_TILEMAP = 1,
};

/// Asset type
typedef struct
{
    void* data; /// Asset data
    char name[64]; /// Asset name
    int type; /// Asset type
}
ASSET;

/// Word parse count index
static unsigned int wordIndex;
/// Path
static char* path;
/// Name
static char* name;
/// Asset type
static int assType;
/// Assets array
static ASSET assets[1024];
/// Asset count
static unsigned int assCount;

/// Load assets from list
/// > 0 on success, 1 on error
static int load_from_list()
{
    int i = 0;

    wordIndex = 0;
    assCount = 0;
    unsigned int wordCount = get_list_word_count();
    for(i=0; i <= wordCount; i++)
    {
        if(wordIndex == 0)
        {
            if(strcmp(get_list_word(i).data,"bitmap") == 0)
            {
                assType = T_BITMAP;
                wordIndex = 2;
            }
            else if(strcmp(get_list_word(i).data,"tilemap") == 0)
            {
                assType = T_TILEMAP;
                wordIndex = 2;
            }
            
        }
        else
        {
            if(wordIndex == 2)
            {
                name = get_list_word(i).data;
            }
            else if(wordIndex == 1)
            {
                path = get_list_word(i).data;
                
                bool success = false;

                void* p = NULL;

                // Load file
                switch(assType)
                {
                    case T_BITMAP:
                    {
                        p = (void*)load_bitmap(path);
                        if(p == NULL)
                        {
                            return 1;
                        }
                        success = true;
                        break;
                    }

                    case T_TILEMAP:
                    {
                        p = (void*)load_tilemap(path);
                        if(p == NULL)
                        {
                            return 1;
                        }
                        success = true;
                        break;
                    }

                    default:
                        break;
                }
                if(success)
                {
                    assets[assCount].data = p;
                    assets[assCount].type = assType;
                    strcpy(assets[assCount].name,name);
                }

                assCount ++;
            }

            wordIndex --;
        }
    }

    return 0;
}

/// Load assets
int load_assets(const char* path)
{

    // Parse file
    if(load_list(path) != 0 || load_from_list() != 0)
    {
        return 1;
    }

    return 0;
}

/// Get bitmap by name
BITMAP* get_bitmap(const char* name)
{
    int i = 0;
    for(; i < assCount; i++)
    {
        if(assets[i].type == T_BITMAP && strcmp(assets[i].name,name) == 0)
        {
            return (BITMAP*)assets[i].data;
        }
    }

    return NULL;
}


/// Get tilemap by name
TILEMAP* get_tilemap(const char* name)
{
    int i = 0;
    for(; i < assCount; i++)
    {
        if(assets[i].type == T_TILEMAP && strcmp(assets[i].name,name) == 0)
        {
            return (TILEMAP*)assets[i].data;
        }
    }

    return NULL;
}

/// Destroy loaded asset files
void destroy_assets()
{
    int i = 0;
    for(; i < assCount; i++)
    {
        int t = assets[i].type;
        if(t == T_BITMAP)
        {
            BITMAP* b = (BITMAP*)assets[i].data;
            destroy_bitmap(b);
        }
        else if(t == T_TILEMAP)
        {
            TILEMAP* t = (TILEMAP*)assets[i].data;
            destroy_tilemap(t);
        }
    }
}