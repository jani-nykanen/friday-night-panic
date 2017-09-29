/// Frame (source)
/// (c) 2017 Jani Nykänen

#include "graphics.h"
#include "frame.h"
#include "stdio.h"
#include "malloc.h"

/// Create palette lookup table
/// < fr Frame
void fr_gen_palette(FRAME* fr)
{
    // Fill palette
    int i = 0;
    Uint8 r,g,b;
    for(; i < 64; i++)
    {
        r = i >> 4;
        g = i << 4;
        g = g >> 6;
        b = i << 6;
        b = b >> 6; 

        fr->palette[i*3] = r *85;
        fr->palette[i*3 +1] = g *85;
        fr->palette[i*3 +2] = b *85;

        fr->palette[i*3 + 64*3] = fr->palette[i*3] > 0 ? fr->palette[i*3] -85 : 0;
        fr->palette[i*3 + 64*3 +1] = fr->palette[i*3 +1] > 0 ? fr->palette[i*3 +1] -85 : 0;
        fr->palette[i*3 + 64*3 +2] = fr->palette[i*3 +2] > 0 ? fr->palette[i*3 +2] -85 : 0;

        fr->palette[i*3 + 128*3] = fr->palette[i*3+ 64*3] > 0 ? fr->palette[i*3+ 64*3] -85 : 0;
        fr->palette[i*3 + 128*3 +1] = fr->palette[i*3+ 64*3 +1] > 0 ? fr->palette[i*3+ 64*3 +1] -85 : 0;
        fr->palette[i*3 + 128*3 +2] = fr->palette[i*3+ 64*3 +2] > 0 ? fr->palette[i*3+ 64*3 +2] -85 : 0;

        fr->palette[i*3 + 192*3] = fr->palette[i*3+ 128*3] > 0 ? fr->palette[i*3+ 128*3] -85 : 0;
        fr->palette[i*3 + 192*3 +1] = fr->palette[i*3+ 128*3 +1] > 0 ? fr->palette[i*3+ 128*3 +1] -85 : 0;
        fr->palette[i*3 + 192*3 +2] = fr->palette[i*3+ 128*3 +2] > 0 ? fr->palette[i*3+ 128*3 +2] -85 : 0;
    }
}

/// Create frame
FRAME* frame_create(int w, int h)
{
    // Allocate memory
    FRAME* fr = (FRAME*)malloc(sizeof(FRAME));
    if(fr == NULL)
    {
        printf("Memory allocation error!\n");
        return NULL;
    }
    fr->data = (Uint8*)malloc(sizeof(Uint8) * w * h *4);
    if(fr->data == NULL)
    {
        free(fr);
        printf("Memory allocation error!\n");
        return NULL;
    }
    fr->colorData = (Uint8*)malloc(sizeof(Uint8) * w * h *3);
    if(fr->data == NULL)
    {
        free(fr);
        printf("Memory allocation error!\n");
        return NULL;
    }
    fr->depth = (float*)malloc(sizeof(float) * w);
    if(fr->depth == NULL)
    {
        free(fr);
        printf("Memory allocation error!\n");
        return NULL;
    }

    // Clear data
    int x = 0;
    int y = 0;
    for(; y < h; y++)
    {
        for(x=0; x < w; x++)
        {
            fr->data[y*w*4 +x*4] = 255;
            fr->data[y*w*4 +x*4 +1] = 255;
            fr->data[y*w*4 +x*4 +2] = 255;
            fr->data[y*w*4 +x*4 +3] = 255;

            fr->colorData[y*w +x] = 0;

        }
    }

    // Store dimensions
    fr->w = w;
    fr->h = h;

    // Store size
    fr->size = w*h;

    // Create texture
    fr->tex = SDL_CreateTexture(get_global_renderer(),SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_STREAMING ,w,h);
    if(fr->tex == NULL)
    {
        free(fr->data);
        free(fr);
        printf("Failed to create a texture!\n");
        return NULL;
    }

    // Generate palette
    fr_gen_palette(fr);

    return fr;
}

/// Update frame texture
void frame_update_tex(FRAME* fr)
{
    if(fr == NULL) return;

    int i = 0;
    int pos = 0;
    for(; i < fr->size; i++)
    {
        pos = fr->colorData[i]*3;
        fr->data[i*4 + 1] = fr->palette[ pos +2 ];
        fr->data[i*4 + 2] = fr->palette[ pos + 1];
        fr->data[i*4 + 3] = fr->palette[ pos ];
    }

    SDL_UpdateTexture(fr->tex,NULL,fr->data,fr->w*4);
}
