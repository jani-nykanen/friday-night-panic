/// Pause scene (header)
/// (c) 2017 Jani Nykänen

#ifndef __PAUSE__
#define __PAUSE__

#include "engine/scene.h"

/// Set pause mode
/// < m Mode (0 for normal, 1 for "Wanna quit?")
void set_pause_mode(int m);

/// Return the pause scene
/// > Scene object
SCENE pause_get_scene();

#endif // __PAUSE__