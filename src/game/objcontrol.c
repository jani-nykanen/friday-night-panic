/// Object controller (source)
/// (c) 2017 Jani Nykänen

#include "objcontrol.h"

#include "stage.h"

#include "../engine/assets.h"
#include "../engine/controls.h"

#define NUM_OBJ 64

/// Player object
static PLAYER pl;

/// Obstacle count
static Uint16 obsCount;

/// Obstacles
static OBSCTALE obstacles[NUM_OBJ];

/// Push a new obstacle to the stack
void push_obstacle(OBSCTALE o)
{
    obstacles[obsCount] = o;
    obsCount ++;
}

/// Clear obstacles
void clear_obstacles()
{
    obsCount = 0;
}

/// Init object controller
void init_object_control()
{
    // Init object data
    init_player();
    init_obstacles();

    // Create objects
    pl = create_player(vec2(4.0f * 16.0f,8.5f * 16.0f));

    // Set default values
    obsCount = 0;
}

/// Update object controller
void update_obj_control(float tm)
{
    // Update player
    player_update(&pl,tm);
    stage_get_player_collision(&pl,tm);

    // Update obstacles
    int i = 0;
    for(; i < obsCount; i++)
    {
        obs_update(&obstacles[i],tm);
        obs_on_player_collision(&obstacles[i],&pl);
        stage_get_obs_collision(&obstacles[i]);
    }
    
    // DEBUG
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
     // Draw obstacles
     int i = 0;
     for(; i < obsCount; i++)
     {
         obs_draw(&obstacles[i]);
     }

    // Draw player
    player_draw(&pl);
}