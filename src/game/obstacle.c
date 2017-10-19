/// Obstacle (source)
/// (c) 2017 Jani Nykänen

#include "obstacle.h"

#include "stage.h"

#include "../engine/graphics.h"
#include "../engine/assets.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Enemy bitmap
static BITMAP* bmpEnemies;

/// Jump sound
static SOUND* sndJump2;
/// Thwomp sound
static SOUND* sndThwomp;

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
            spr_animate(&o->spr,o->spr.row,0,3,o->spr.row < 7 ? (o->spr.row != 0 ? 8 : 6) : 4,tm);
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
        play_sound(sndJump2,0.5f);
        int ix = (int)floor(o->x / 16);
        o->timer += (ix % 2 == 0) ? 120.0f : 135.0f;

        o->speed.y = ix % 2 == 0 ? -2.5f : -2.75f;
    }

    o->speed.y += 0.0625f * tm;
    if(o->y > o->startY && o->speed.y >= 0.0f)
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
static void electricity_behavior(OBSCTALE* o, float tm)
{
    o->playZap = false;
    o->timer += 1.0f * tm;
    bool s = o->show;
    o->show = o->timer <= 120.0f;
    if(s != o->show)
        o->playZap = true;

    if(o->timer >= 240.0)
    {
        o->timer -= 240.0f;
    }
}

/// Apple
/// < o Obstacle
/// < tm Time mul.
static void apple_behavior(OBSCTALE* o, float tm)
{
    if(o->dropping == 0)
    {
        o->speed.y = 0;
    }
    else if(o->dropping == 1)
    {
        o->speed.y += 0.1f * tm;
        if(o->speed.y > 3.0f)
            o->speed.y = 3.0f;
    }
    else if(o->dropping == 2)
    {
        if(o->speed.y > -0.5f)
        {
            o->speed.y -= 0.05f;
            if(o->speed.y < -0.5f)
                o->speed.y = -0.5f;
        }
        if(o->y < o->startY)
        {
            o->y = o->startY;
            o->dropping = 0;
        }
    }
}

/// Init global obstacle variables
void init_obstacles()
{
    bmpEnemies = get_bitmap("enemies");

    sndJump2 = get_sound("jump2");
    sndThwomp = get_sound("thwomp");
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

    if(id == 0 || id == 2)
    {
        float speed = id == 0 ? 0.5f : 0.4f;
        o.speed.x = (int)floor(o.x/16) % 2 == 0 ? -speed : speed;
    }

    if(id == 1)
    {
        o.speed.y = (int)floor(o.y/16) % 2 == 0 ? -0.4f : 0.4f;
    }

    o.timer = 0.0f;
    if(id == 3 || id == 8)
    {
        o.timer = 60.0f;
    }
    o.playZap = false;

    o.spr = create_sprite(24,24);
    o.spr.row = id;

    o.dropping = 0;
    o.show = true;

    return o;
}

/// Get player collision
void obs_on_player_collision(OBSCTALE* o, PLAYER* pl)
{
    if(!o->show) return;

    if(o->spr.row == 5 && o->dropping == 0)
    {
        if(fabs(o->x+8 - pl->pos.x) < 24)
        {
            o->dropping = 1;
        }
    }

    player_hurt_collision(pl,vec2(o->x+2,o->y+4),vec2(12,8));
}

/// Swap direction collision, horizontal
void obs_swap_dir_horizontal(OBSCTALE* o, float x, float y, float w, float h)
{
    if(!o->show) return;

    if(o->x <= x+w && o->x+16 >= x && o->y+16 >= y && o->y <= y+h)
    {
        if( (x+w/2 < o->x+8 && o->speed.x < 0.0f) || (x+w/2 > o->x+8 && o->speed.x > 0.0f) )
        {
            o->speed.x *= -1;
        }
    }
    
}


/// Swap direction collision, vertical
void obs_swap_dir_vertical(OBSCTALE* o, float x, float y, float w, float h)
{
    if(!o->show) return;

    if(o->x <= x+w-2 && o->x+16 >= x+2 && o->y+16 >= y && o->y <= y+h)
    {
        if( (y+h/2 < o->y+8 && o->speed.y < 0.0f) || (y+h/2 > o->y+8 && o->speed.y > 0.0f) )
        {
            if(o->spr.row == 5 && o->dropping == 1)
            {
                o->dropping = 2;
                o->speed.y = 0;
                stage_shake(60.0f);
                play_sound(sndThwomp,0.60f);
            }
            else if(o->spr.row != 5)
            {
                o->speed.y *= -1;
            }
        }
    }
    
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
    
    case 5:
        apple_behavior(o,tm);
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
        spr_draw(&o->spr,bmpEnemies,o->x-4,o->y-4 +1,o->speed.x > 0.0f ? FLIP_HORIZONTAL : FLIP_NONE);

}
