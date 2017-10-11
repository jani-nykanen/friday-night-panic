/// Obstacle (source)
/// (c) 2017 Jani Nykänen

#include "obstacle.h"

#include "../engine/graphics.h"
#include "../engine/assets.h"

/// Enemy bitmap
static BITMAP* bmpEnemies;

/// Init global obstacle variables
void init_obstacles()
{
    bmpEnemies = get_bitmap("enemies");
}

/// Create a new obstacle
OBSCTALE create_obstacle(float x, float y, int id)
{
    OBSCTALE o;

    o.x = x;
    o.y = y;
    o.id = id;

    o.spr = create_sprite(24,24);
    o.spr.row = id;

    return o;
}

/// Update an obstacle
void obs_update(OBSCTALE* o, float tm)
{
    // Animate
    switch(o->spr.row)
    {
        // Plant 
        case 6:
        {
            spr_animate(&o->spr,o->spr.row,0,1,8,tm);
            break;
        }

        // Sun & bats & electricity & cats
        case 0:
        case 1:
        case 2:
        case 4:
        case 7:
        case 8:
        {
            spr_animate(&o->spr,o->spr.row,0,3,o->spr.row < 7 ? 8 : 4,tm);
            break;
        }

        default:
            break;
    }
}

/// Draw an obstacle
void obs_draw(OBSCTALE* o)
{
    spr_draw(&o->spr,bmpEnemies,o->x+4,o->y+4, FLIP_NONE);
}
