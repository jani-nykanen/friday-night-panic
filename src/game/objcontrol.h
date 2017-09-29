/// Object controller (header)
/// (c) 2017 Jani Nykänen

#ifndef __OBJ_CONTROL__
#define __OBJ_CONTROL__

#include "player.h"

#include "../engine/bitmap.h"

/// Init object controller
void init_object_control();

/// Update object controller
/// < tm Time mul.
void update_obj_control(float tm);

/// Draw object controller objects
void draw_objects();

#endif // __OBJ_CONTROL__