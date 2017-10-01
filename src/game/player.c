/// Player object (source)
/// (c) 2017 Jani Nykänen

#include "player.h"

#include "stage.h"
#include "status.h"

#include "../vpad.h"

#include "../engine/graphics.h"
#include "../engine/assets.h"

/// Player bitmap
static BITMAP* bmpPlayer;

/// Control player
/// < pl Player to control
/// < tm Time mul.
static void pl_control(PLAYER* pl, float tm)
{
    if(!pl->touchLadder)
    {
        pl->climbing = false;
    }

    VEC2 stick = vpad_get_stick();

    if(pl->climbing)
    {
        pl->target.x = 0.0f;
        pl->target.y = 0.0f;

        if(fabs(stick.y) > 0.1f)
            pl->target.y = stick.y * 0.5f;;
    }
    else
    {
        pl->target.y = 3.0f;
        pl->target.x = 0.0f;
        if(fabs(stick.x) > 0.1f && !pl->crouch)
        {   
            pl->target.x = stick.x * 1.5f;
            pl->dir = stick.x > 0.0f ? FLIP_NONE : FLIP_HORIZONTAL;
        }
    }

    if( (pl->canJump || pl->climbing) && vpad_get_button(0) == PRESSED)
    {
        pl->speed.y = -2.5f;
        pl->climbing = false;
    }
    else if(!pl->canJump && pl->speed.y < 0.0f && vpad_get_button(0) == RELEASED)
    {
        pl->speed.y /= 2.0f;
    }

    pl->crouch = (pl->canJump && stick.y > 0.5f);
}

/// Move player
/// < pl Player object
/// < tm Time mul.
static void pl_move(PLAYER* pl, float tm)
{
    if(pl->target.x > pl->speed.x)
    {
        pl->speed.x += pl->speedMul.x * tm;
        pl->speed.x = pl->speed.x > pl->target.x ? pl->target.x : pl->speed.x;
    }
    else if(pl->target.x < pl->speed.x)
    {
        pl->speed.x -= pl->speedMul.x * tm;
        pl->speed.x = pl->speed.x < pl->target.x ? pl->target.x : pl->speed.x;
    }

    if(pl->target.y > pl->speed.y)
    {
        pl->speed.y += pl->speedMul.y * tm;
        pl->speed.y = pl->speed.y > pl->target.y ? pl->target.y : pl->speed.y;
    }
    else if(pl->target.y < pl->speed.y)
    {
        pl->speed.y -= pl->speedMul.y * tm;
        pl->speed.y = pl->speed.y < pl->target.y ? pl->target.y : pl->speed.y;
    }

    pl->pos.x += pl->speed.x * tm;
    pl->pos.y += pl->speed.y * tm;
}

/// Animate player
/// < pl Player to animate
/// < tm Time mul.
static void pl_animate(PLAYER* pl, float tm)
{
    if(pl->climbing)
    {
        float tspeed = hypot(pl->speed.x,pl->speed.y);

        if(tspeed > 0.05f)
        {
            spr_animate(&pl->spr,2,0,3,6,tm);
        }
        return;
    }

    if(!pl->canJump)
    {
        int frame = 1;
        if(pl->speed.y > 0.25f)
        {
            frame = 2;
        }
        else if(pl->speed.y < -0.25f)
        {
            frame = 0;
        }
        spr_animate(&pl->spr,1,frame,frame,0,tm);

        return;
    }
    else if(pl->crouch)
    {
        spr_animate(&pl->spr,1,3,3,0,tm);
        return;
    }

    if(fabs(pl->speed.x) > 0.01f)
    {
        float speed = 10.0f - fabs(pl->speed.x*3);
        spr_animate(&pl->spr,0,1,6,speed,tm);
    }
    else
    {
        spr_animate(&pl->spr,0,0,0,0,tm);
    }   
}

/// Kill player
/// < pl Player to kill
static void pl_die(PLAYER* pl)
{
    pl->pos.x = 16.0f;
    pl->pos.y = 8.0f + get_lowest_solid_y();

    pl->speed.x = 0.0f;
    pl->speed.y = 0.0f;
    pl->dir = FLIP_NONE;
    pl->climbing = false;
    pl->crouch = false;

    get_global_status()->lives --;
}

/// Init global player data (read: bitmaps)
void init_player()
{
    bmpPlayer = get_bitmap("player");
}

/// Create a player object (mostly set default values)
PLAYER create_player(VEC2 pos)
{
    PLAYER pl;
    pl.pos = pos;
    pl.speed = vec2(0.0f,0.0f);
    pl.target = pl.speed;
    pl.crouch = false;
    pl.speedMul.x = 0.025f;
    pl.speedMul.y = 0.08f;
    pl.canJump = false;
    pl.climbing = false;
    pl.spr = create_sprite(32,32);
    pl.dir = 0;
    pl.dim = vec2(4.0f,18.0f);
    pl.touchLadder = false;

    return pl;
}

/// Update player
void player_update(PLAYER* pl, float tm)
{
    pl_control(pl,tm);
    pl_animate(pl,tm);
    pl_move(pl,tm);

    pl->canJump = false;
    pl->touchLadder = false;

    // In the case the player drops outside the stage
    player_hurt_collision(pl,vec2(0.0f,168.0f),vec2(320.0f,64.0f));
}

/// Get (wall) collision
void player_get_collision(PLAYER*pl, VEC2 p, float w, bool horizontal, bool dir, float tm)
{
    float x = pl->pos.x;
    float y = pl->pos.y;

    float pw = pl->dim.x;
    float ph = pl->dim.y;

    if(horizontal)
    {
        if(x+pw > p.x && x-pw < p.x+w)
        {
            if(dir && pl->speed.y > 0.0f && y > p.y-1*tm && y < p.y+4*tm)
            {
                pl->speed.y = 0.0f;
                pl->pos.y = p.y;
                pl->canJump = true;
                pl->climbing = false;
            }
            else if(!dir && pl->speed.y < 0.0f && y-ph < p.y+1*tm && y-ph > p.y-4*tm)
            {
                pl->speed.y = 0.0f;
                pl->pos.y = p.y+ph;
            }
        }           
    }
    else
    {
        if(y-ph < p.y+w && y > p.y+0.5f) // 0.5f required, precision reasons or something
        {
            bool col = false;

            if(dir && pl->speed.x > 0.0f && x+pw > p.x-0.5f*tm && x < p.x+8.0f * tm)
            {
                pl->pos.x = p.x-pw;
                col = true;
            }
            else if(!dir && pl->speed.x < 0.0f && x-pw < p.x+0.5f*tm && x > p.x-8.0f * tm)
            {
                pl->pos.x = p.x+pw;
                col = true;
            }

            if(col)
            {
                if(fabs(pl->speed.x) > 0.15f)
                {
                    pl->speed.x /= -1.25f;
                }
                else
                {
                    pl->speed.x = 0.0f;
                }
            }
        }
    }
}

/// Climb collision
void player_get_climb_collision(PLAYER* pl, VEC2 p, float l)
{
    float x = pl->pos.x;
    float y = pl->pos.y;

    float pw = pl->dim.x * 0.75f;;
    float ph = pl->dim.y;

    if(x+pw >= p.x && x-pw <= p.x+l && y >= p.y && y-ph <= p.y+l)
    {
        pl->touchLadder = true;

        /// @TODO: Delta here
        if(!pl->climbing && vpad_get_stick().y < -0.25f && vpad_get_delta().y < -0.5f)
        {
            pl->climbing = true;
            pl->speed.x = 0.0f;
            pl->speed.y = 0.0f;
            pl->pos.x = p.x + l/2.0f;
            pl->spr.frame = 0;
            pl->spr.row = 2;
        }
    }
}

/// Hurt collision (kills the player)
void player_hurt_collision(PLAYER* pl, VEC2 p, VEC2 dim)
{
    float x = pl->pos.x;
    float y = pl->pos.y;

    float pw = pl->dim.x;
    float ph = pl->dim.y;

    if(x+pw >= p.x && x-pw <= p.x+dim.x && y >= p.y && y-ph <= p.y+dim.y)
    {
        pl_die(pl);
    }
}

/// Draw player
void player_draw(PLAYER* pl)
{
    int x = (int)round(pl->pos.x);
    int y = (int)round(pl->pos.y) +1;

    spr_draw(&pl->spr,bmpPlayer,x-16,y-28,pl->dir);
}