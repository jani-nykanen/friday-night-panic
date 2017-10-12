/// Stage (source)
/// (c) 2017 Jani Nykänen

#include "stage.h"

#include "stdlib.h"

#include "../engine/assets.h"
#include "../engine/graphics.h"

#include "objcontrol.h"

/// Background image
static BITMAP* bmpBg;
/// Tiles
static BITMAP* bmpTiles;

/// Current tilemap
static TILEMAP* map;
/// Collision tilemap
static TILEMAP* col;
/// Map id
static int mapID;
/// Water pos
static float waterPos;
/// Shake timer
static float shakeTimer;

/// Draw the current tilemap
/// < tx X translation
/// < ty Y translation
static void draw_tilemap(int tx, int ty)
{
    int x;
    int y;
    int layer;

    int tileid;
    int sx = 0;
    int sy = 0;

    for(layer=0; layer < map->layerCount; layer ++)
    {
        for(y=0; y < map->h; y++)
        {
            for(x=0; x < map->w; x++)
            {
                tileid = map->layers[layer] [y*map->w +x];
                if(tileid > 0 && tileid < 16*6)
                {
                    tileid --;
                    sx = (tileid % 16) * 16;
                    sy = (tileid/16);
                    sy *= 16;

                    if(tileid == 21)
                    {
                        sx += - waterPos;
                        draw_bitmap_region(bmpTiles,sx,sy,16,16,tx + x*16, ty + y*16
                            + (int)(sin(waterPos/8.0f * M_PI)*2 +2),FLIP_NONE);
                    }
                    else
                    {
                        draw_bitmap_region(bmpTiles,sx,sy,16,16,tx + x*16, ty + y*16,FLIP_NONE);
                    }
                }
            }
        }
    }
}

// Get player collision
static void tilemap_collisions(PLAYER* pl, float tm)
{
    int cx = (int)floor(pl->pos.x / 16.0f);
    int cy = (int)floor(pl->pos.y / 16.0f);

    int sx = cx - 3;
    int sy = cy - 3;

    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;

    int ex = cx + 3;
    int ey = cy + 3;

    if(ex >= map->w) ex = map->w-1;
    if(ey >= map->h) ey = map->h-1;

    int x = 0;
    int y = 0;
    Uint8 tid;

    int layer;
    for(layer=0; layer < map->layerCount; layer ++)
    {
        for(y = sy; y <= ey; y++)
        {
            for(x = sx; x <= ex; x++)
            {
                tid = col->layers[0] [map->layers[layer] [y*map->w + x] -1];

                if(tid == 1 || tid == 5 || tid == 7 || tid == 10 || tid == 11 || tid == 12 || tid == 14 || tid == 15)
                {
                    player_get_collision(pl,vec2(8.0f+ x*16.0f,8.0f+ y*16.0f),16.0f,true,true,tm);
                }
            
                if(tid == 3 || tid == 5 || tid == 8 || tid == 9 || tid == 12 || tid == 13 || tid == 14 || tid == 15)
                {
                    player_get_collision(pl,vec2(8.0f+  (x)*16.0f,8.0f+ ( y+1)*16.0f),16.0f,true,false,tm);
                }
            
                if(tid == 2 || tid == 6 || tid == 7 || tid == 8 || tid == 11 || tid == 12 || tid == 13 || tid == 15)
                {
                    player_get_collision(pl,vec2(8.0f + (x+1)*16.0f,8.0f + y*16.0f),16.0f,false,false,tm);
                }
            
                if(tid == 4 || tid == 6 || tid == 9 || tid == 10 || tid == 11 || tid == 13 || tid == 14 || tid == 15)
                {
                    player_get_collision(pl,vec2(8.0f + x*16.0f,8.0f + y*16.0f),16.0f,false,true,tm);
                }

                if(tid == 18)
                {
                    player_get_climb_collision(pl,vec2(8.0f + x*16.0f,8.0f + y*16.0f),16.0f);
                }

                if(tid == 17)
                {
                    player_hurt_collision(pl,vec2(8.0f + x*16.0f + 2.0f,8.0f + y*16.0f + 5.0f),vec2(12.0f,13.0f));
                }
            }
        }
    }
}

/// Parse obstacles from the map
static void parse_obstacles()
{
    const int ids[] =
    {
        0,1,6,3,4,2,7,8
    };

    int  ex = map->w-1;
    int ey = map->h-1;

    int x = 0;
    int y = 0;
    Uint8 tid;

    OBSCTALE o;

    int layer;
    for(layer=0; layer < map->layerCount; layer ++)
    {
        for(y = 0; y <= ey; y++)
        {
            for(x = 0; x <= ex; x++)
            {
                tid = map->layers[layer] [y*map->w +x];

                if(tid >= 16*6 && tid < 16*6 + 9)
                {
                    o = create_obstacle(8+ x*16,8+ y*16,ids[tid-16*6-1]);
                    push_obstacle(o);
                }
                else if(tid == 16*6 + 11)
                {
                    o = create_obstacle(8 + x*16,8 + y*16,5);
                    push_obstacle(o);
                }
            }
        }
    }
}

/// Initialize stage
void init_stage()
{
    bmpBg = get_bitmap("bg");
    bmpTiles = get_bitmap("tiles");

    mapID = 1;
    waterPos = 0.0f;
    shakeTimer = 0.0f;

    map = get_tilemap("1");
    col = get_tilemap("collisions");

    parse_obstacles();
}

/// Update stage
void stage_update(float tm)
{
    waterPos -= 0.125f * tm;
    if(waterPos <= -16.0f)
        waterPos += 16.0f;

    if(shakeTimer > 0.0f)
        shakeTimer -= 1.0f * tm;
}

/// Shake the stage
void stage_shake(float t)
{
    shakeTimer = t;
}

/// Get player collision
void stage_get_player_collision(PLAYER* pl, float tm)
{

    tilemap_collisions(pl,tm);

    bool swapMap = false;

    // Swap map
    if(pl->pos.x > 8+304)
    {
        mapID ++;
        swapMap = true;

        pl->pos.x -= 304;
    }
    else if(mapID > 0 && pl->pos.x < 8)
    {
        mapID --;
        swapMap = true;

        pl->pos.x += 304;
    }

    if(swapMap)
    {
        char numString[2];
        sprintf(numString, "%d", mapID);
        map = get_tilemap(numString);
        clear_obstacles();
        parse_obstacles();
    }
}

/// Get obstacle collision
void stage_get_obs_collision(OBSCTALE* o)
{
    int cx = (int)floor(o->x / 16.0f);
    int cy = (int)floor(o->y / 16.0f);

    int sx = cx - 3;
    int sy = cy - 3;

    if(sx < 0) sx = 0;
    if(sy < 0) sy = 0;

    int ex = cx + 3;
    int ey = cy + 3;

    if(ex >= map->w) ex = map->w-1;
    if(ey >= map->h) ey = map->h-1;

    int x = 0;
    int y = 0;
    int layer = 0;
    Uint8 tid;

    for(layer=0; layer < map->layerCount; layer ++)
    {
        for(y=0; y < map->h; y++)
        {
            for(x=0; x < map->w; x++)
            {
                tid = map->layers[layer] [y*map->w +x];

                if(tid == 16*6+9)
                {
                    obs_swap_dir_horizontal(o,8+ x*16,8+ y*16,16,16);
                }
                else if(tid == 16*6+10)
                {
                    obs_swap_dir_vertical(o,8+ x*16,8+ y*16,16,16);
                }

            }
        }
    }
}

/// Draw stage
void stage_draw()
{
    if(shakeTimer > 0.0f)
    {
        set_translation(rand() % 5 - 2, rand() % 5 - 2);
    }

    draw_bitmap(bmpBg,8,8,FLIP_NONE);
    draw_tilemap(8,8);
}

/// Return the lowest (actually highest) solid y position
float get_lowest_solid_y()
{
    int x = 0;
    int y = 0;
    Uint8 tid;

    int layer;
    for(layer=0; layer < map->layerCount; layer ++)
    {
        for(y = 3; y <= map->h; y++)
        {
            tid = col->layers[0] [map->layers[layer] [y*map->w + x] -1];
                
            if(tid == 1 || tid == 5 || tid == 7 || tid == 10 || tid == 11 || tid == 12 || tid == 14 || tid == 15)
            {
                 return y*16.0f;
            }
        }
    }

    return 0.0f;
}

/// Get current map
TILEMAP* get_current_map()
{
    return map;
}

/// Get map id
int get_map_id()
{
    return mapID;
}