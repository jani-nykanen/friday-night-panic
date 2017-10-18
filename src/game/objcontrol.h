/// Object controller (header)
/// (c) 2017 Jani Nykänen

#ifndef __OBJ_CONTROL__
#define __OBJ_CONTROL__

#include "player.h"
#include "obstacle.h"

#include "../engine/bitmap.h"

/// Init object controller
void init_object_control();

/// Push a new obstacle to the stack
/// < o Obstacle
void push_obstacle(OBSCTALE o);

/// Update object controller
/// < tm Time mul.
void update_obj_control(float tm);

/// Draw object controller objects
void draw_objects();

/// Reset object control
void reset_obj_control();

#endif // __OBJ_CONTROL__