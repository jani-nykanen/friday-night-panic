/// Bitmap (source)
/// (c) 2017 Jani Nykänen

#include <SDL2/SDL_image.h>
#include "bitmap.h"
#include "graphics.h"

/// Load bitmap
BITMAP* load_bitmap(const char* path)
{
    // Allocate memory
    BITMAP* bmp = (BITMAP*)malloc(sizeof(BITMAP));
    if(bmp == NULL)
    {
        printf("Failed to allocate memory for a bitmap!\n");
        return NULL;
    }

    // Load surface
    SDL_Surface* surf = IMG_Load(path);
    if(surf == NULL)
    {
        printf("Failed to load a bitmap in %s!\n",path);
        return NULL;
    }

    unsigned int pixelCount = surf->w * surf->h;

    bmp->w = surf->w;
    bmp->h = surf->h;

    // Allocate image and temp buffer data
    bmp->data = (Uint8*)malloc(sizeof(Uint8) * pixelCount);
    if(bmp->data == NULL) 
    {
        printf("Failed to allocate memory for a bitmap!\n");
        return NULL;
    }

    // Go through the data
    int i = 0;
    Uint8 pixel;
    Uint8 r,g,b,a;
    Uint8 er,eg,eb;
    Uint8* pdata = (Uint8*)surf->pixels;
    for(; i < pixelCount; i++)
    {
        a = pdata[i*4 +3];
        if(a != 255)
        {
            bmp->data[i] = 255;
            continue;
        }

        r = pdata[i*4 +2];
        g = pdata[i*4 +1];
        b = pdata[i*4 ];

        er = (r / 85);
        eg = (g / 85);
        eg = eg << 2;
        eb = (b / 85);
        eb = eb << 4;

        pixel = 0;
        pixel = er | eg | eb;

        bmp->data[i] = pixel;
    }

    // Free surface
    free(surf);

    return bmp;
}

/// Destroy bitmap
void destroy_bitmap(BITMAP* bmp)
{
    if(bmp == NULL) return;

    if(bmp->data != NULL) free(bmp->data);
    free(bmp);
}