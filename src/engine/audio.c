/// Audio (source)
/// (c) 2017 Jani Nykänen

#include "audio.h"

#include "stdio.h"

/// Global volume
static float globalSoundVol;

/// Init audio
int audio_init()
{
    // Init Ogg loading
    int flags = MIX_INIT_OGG;
    int initted = Mix_Init(flags);
    if( (initted&flags) != flags )
    {
        printf("Failed to initialize OGG support!\n");
        return 1;
    }

    // Open audio
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) 
    {
        printf("Failed to open audio!");
        return 1;
    }

    // Set default values
    globalSoundVol = 1.0f;

    return 0;
}

/// Set global volume
void set_global_sound_volume(float vol)
{
    globalSoundVol = vol;
}

/// Load a sound
SOUND* load_sound(const char* path)
{
    // Allocate memory
    SOUND * s = (SOUND*)malloc(sizeof(s));
    if(s == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }

    // Load WAV
    s->chunk =Mix_LoadWAV(path);
    if(!s->chunk) 
    {
        printf("Failed to load a sound in %s!\n",path);
        free(s);
        return NULL;
    }

    // Set default values
    s->channel = 0;
    s->played = false;

    return s;
}

/// Play sound
void play_sound(SOUND* s, float vol)
{
    if(s == NULL) return;

    if(!s->played)
    {
        // Get channel, halt, set volume, play again
        s->channel = Mix_PlayChannel(-1,s->chunk,0);
        Mix_HaltChannel(s->channel);

        Mix_Volume(s->channel,(int)(MIX_MAX_VOLUME * vol * globalSoundVol));
        Mix_PlayChannel(-1,s->chunk,0);
        

        s->played = true;
    }
    else
    {
        Mix_HaltChannel(s->channel);
        Mix_Volume(s->channel,(int)(MIX_MAX_VOLUME * vol * globalSoundVol));
        Mix_PlayChannel(s->channel,s->chunk,0);
    }
}

/// Stop all audio tracks
void stop_all_audio()
{
    Mix_HaltChannel(-1);
}

/// Destroy sound & free memory
void destroy_sound(SOUND* s)
{
    if(s == NULL) return;

    Mix_FreeChunk(s->chunk);
    free(s);
}