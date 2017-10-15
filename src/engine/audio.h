/// Audio (header)
/// (c) 2017 Jani Nykänen

#ifndef __AUDIO__
#define __AUDIO__

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include "stdbool.h"

/// Sound effect type
typedef struct
{
    Mix_Chunk* chunk; /// Chunk
    int channel; /// Channel
    bool played; /// Has the sound been played at least once
}
SOUND;

/// Init audio
/// > 0 on success, 1 on error
int audio_init();

/// Set global sound volume
/// < vol Volume
void set_global_sound_volume(float vol);

/// Load a sound
/// < path Path
/// > A new sound
SOUND* load_sound(const char* path);

/// Play sound
/// < s Sound to play
/// < vol Volume
void play_sound(SOUND* s, float vol);

/// Destroy sound & free memory
/// < s Sound to destroy
void destroy_sound(SOUND* s);

#endif // __AUDIO__