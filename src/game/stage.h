/// Stage (header)
/// (c) 2017 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "player.h"
#include "obstacle.h"

#include "../engine/assets.h"

/// Initialize stage
void init_stage();

/// Update stage
/// < tm Time multiplier
void stage_update(float tm);

/// Get player collision
/// < pl Player
/// < tm Time mul.
void stage_get_player_collision(PLAYER* pl, float tm);

/// Get obstacle collision
/// < o Obstacle
void stage_get_obs_collision(OBSCTALE* o);

/// Draw stage
void stage_draw();

/// Shake the stage
/// < t Shake time
void stage_shake(float t);

/// Return the lowest (actually highest) solid y position
/// > The lowest position in screen coords
float get_lowest_solid_y();

/// Return the current map
/// > A pointer to the map
TILEMAP* get_current_map();

/// Get map id
/// > Map id
int get_map_id();

/// Set the first map
void set_starting_map();

#endif // __STAGE__