/// Frame (header)
/// (c) 2017 Jani Nykänen

#ifndef __FRAME__
#define __FRAME__

#include "SDL2/SDL.h"

/// Frame structure
typedef struct
{
    int w; /// Width
    int h; /// Height
    unsigned int size; /// Actual size in pixels
    Uint8* data; /// Frame data
    Uint8* colorData; /// Color data
    float* depth; /// Depth buffer
    Uint8 palette [256*3]; /// Palette lookup table

    SDL_Texture* tex; /// Frame texture   
}
FRAME;

/// Creates a new frame
/// < w Width
/// < h Height
/// > A pointer to a new frame
FRAME* frame_create(int w, int h);

/// Update frame texture
/// < fr Frame
void frame_update_tex(FRAME* fr);

#endif // __FRAME__