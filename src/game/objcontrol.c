/// Object controller (source)
/// (c) 2017 Jani Nykänen

#include "objcontrol.h"

#include "stage.h"

#include "../engine/assets.h"
#include "../engine/controls.h"

#define NUM_OBJ 32
#define NUM_DOORS 32

/// Player object
static PLAYER pl;

/// Init object controller
void init_object_control()
{
    // Init object data
    init_player();

    // Create objects
    pl = create_player(vec2(4.0f * 16.0f,8.5f * 16.0f));
}

/// Update object controller
void update_obj_control(float tm)
{
    player_update(&pl,tm);
    stage_get_player_collision(&pl,tm);


    // DEBUG
    if(get_key_state((int)SDL_SCANCODE_R) == PRESSED)
    {
        pl.pos.x = 32.0f;
        pl.pos.y = 96.0f;
        pl.speed.y = 0.0f;
    }

    if(get_key_state((int)SDL_SCANCODE_PAGEUP) == PRESSED)
    {
        pl.pos.x += 304.0f;
    }
    else if(get_key_state((int)SDL_SCANCODE_PAGEDOWN) == PRESSED)
    {
        pl.pos.x -= 304.0f;
    }
}

/// Draw object controller objects
void draw_objects()
{
   player_draw(&pl);
}