/// Obstacle (header)
/// (c) 2017 Jani Nykänen

#ifndef __OBSTACLE__
#define __OBSTACLE__

#include "../engine/sprite.h"

/// Obstacle object
typedef struct
{
    float x; /// X coordinate
    float y; /// Y coordinate
    SPRITE spr; /// Sprite
    int id; /// Obstacle id
}
OBSCTALE;

/// Init global obstacle variables
void init_obstacles();

/// Remove obstacles from the screen
void clear_obstacles();

/// Create a new obstacle
/// < x X coordinate
/// < y Y coordinate
/// < id Obstacle id
/// > A new plant
OBSCTALE create_obstacle(float x, float y, int id);

/// Update an obstacle
/// < o Obstacle to update
/// > tm Time multiplier
void obs_update(OBSCTALE* o, float tm);

/// Draw an obstacle
/// < o Obstacle to draw
void obs_draw(OBSCTALE* o);

#endif // __OBSTACLE__