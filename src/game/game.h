/// Game scene (header)
/// (c) 2017 Jani Nykänen

#ifndef __GAME_SCENE__
#define __GAME_SCENE__

#include "../engine/scene.h"

/// Enable game over
/// < mode Game over! mode (2 for victory)
void enable_game_over(int mode);

/// Reset game
void game_reset();

/// Get game scene
/// > Game scene
SCENE get_game_scene();

#endif // __GAME_SCENE__