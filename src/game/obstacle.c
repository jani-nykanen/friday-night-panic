/// Obstacle (source)
/// (c) 2017 Jani Nykänen

#include "obstacle.h"

#include "../engine/graphics.h"
#include "../engine/assets.h"

/// Enemy bitmap
static BITMAP* bmpEnemies;

/// Animate
/// < o Obstacle
/// < tm Time mul.
static void obs_animate(OBSCTALE* o, float tm)
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

/// Jumping behavior
/// < o Obstacle
/// < tm Time mul.
static void jump_behavior(OBSCTALE* o, float tm)
{
    o->timer -= 1.0f * tm;
    if(o->timer <= 0.0f)
    {
        int ix = (int)floor(o->x / 16);
        o->timer = ix % 2 == 0 ? 120.0f : 135.0f;

        o->speed.y = ix % 2 == 0 ? -2.5f : -2.75f;
    }

    o->speed.y += 0.0625f * tm;
    if(o->y > o->startY)
    {
        o->speed.y = 0.0f;
        o->y = o->startY;
    }

    if(fabs(o->speed.y) < 0.2f)
        o->spr.frame = 0;
    else
        o->spr.frame = o->speed.y < 0.0f ? 1 : 2;
}

/// Electricity
/// < o Obstacle
/// < tm Time mul.
void electricity_behavior(OBSCTALE* o, float tm)
{
    o->timer += 1.0f * tm;
    o->show = o->timer <= 120.0f;

    if(o->timer >= 240.0)
    {
        o->timer -= 240.0f;
    }
}

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
    o.startY = y;
    o.id = id;

    o.speed.x = 0.0f;
    o.speed.y = 0.0f;

    o.timer = 0.0f;
    if(id == 3 || id == 8)
    {
        o.timer = 60.0f;
    }

    o.spr = create_sprite(24,24);
    o.spr.row = id;

    o.show = true;

    return o;
}

/// Update an obstacle
void obs_update(OBSCTALE* o, float tm)
{
    // Animate
    obs_animate(o,tm);

    // Special behavior
    switch(o->spr.row)
    {
    case 3:
        jump_behavior(o,tm);
        break;
    
    case 7:
    case 8:
        electricity_behavior(o,tm);
        break;

    default:
        break;
    }

    // Move
    o->x += o->speed.x * tm;
    o->y += o->speed.y * tm;
}

/// Draw an obstacle
void obs_draw(OBSCTALE* o)
{
    if(o->show)
        spr_draw(&o->spr,bmpEnemies,o->x+4,o->y+4 +1, FLIP_NONE);
}
