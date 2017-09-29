/// Player object (header)
/// (c) 2017 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../engine/vector.h"
#include "../engine/sprite.h"

#include "stdbool.h"

/// Player object
typedef struct
{
    VEC2 pos; /// Position
    VEC2 speed; /// Speed
    VEC2 target; /// Target speed
    VEC2 speedMul; /// Speed multiplier
    VEC2 dim; /// Hitbox dimensions
    bool crouch; /// Is crouching
    bool canJump; /// Can the player jump, naturally
    bool climbing; /// Is the player climbing
    bool touchLadder; /// Is touching ladder
    int dir; /// Direction
    SPRITE spr; /// Player sprite
}
PLAYER;

/// Init global player data (read: bitmaps)
void init_player();

/// Create a player object (mostly set default values)
/// < pos Starting position
/// > A player object
PLAYER create_player(VEC2 pos);

/// Update player
/// < pl Player to update
/// < tm Time multiplier
void player_update(PLAYER* pl, float tm);

/// Get (wall) collision
/// < pl Player to collide
/// < p Collider location
/// < w Collider width
/// < horizontal Is wall horizontal (delta-z = 0)
/// < tm Time mul.
void player_get_collision(PLAYER*pl, VEC2 p, float w, bool horizontal, bool dir, float tm);

/// Climb collision
/// < pl Player
/// < p Collision position
/// < l Side length
void player_get_climb_collision(PLAYER* pl, VEC2 p, float l);

/// Draw player
/// < pl Player to draw
void player_draw(PLAYER* pl);

#endif // __PLAYER__