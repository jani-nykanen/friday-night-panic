/// Graphics (source)
/// (c) 2017 Jani Nykänen

#include "graphics.h"
#include "mathext.h"

#include "malloc.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Min delta
static const float DEPTH_MIN = 0.025f;

/// Global renderer
static SDL_Renderer* grend;
/// Global frame
static FRAME* gframe;
/// Window dim
static SDL_Point windowDim;

/// Translate x
static int transX;
/// Translate y
static int transY;

/// Put pixel to the screen
/// < x X coordinate
/// < y Y coordinate
/// < index Color index
static void put_pixel(int x, int y, Uint8 index)
{
    if(index == 255 || x < 0 || y < 0 || x >= gframe->w || y >= gframe->h) return;
    gframe->colorData[y*gframe->w+x] = index;
}

/// Set global renderer
void set_global_renderer(SDL_Renderer* rend)
{
    grend = rend;
}

/// Get grend
SDL_Renderer* get_global_renderer()
{
    return grend;
}

/// Clear screen
void clear(unsigned char r, unsigned char g, unsigned char b)
{
    SDL_SetRenderDrawColor(grend, r,g,b, 255);
    SDL_RenderClear(grend);
}

/// Set window dimensions
void set_dimensions(int w, int h)
{
    windowDim.x = w;
    windowDim.y = h;
}

/// Get window dimensions
SDL_Point get_dimensions()
{
    return windowDim;
}

/// Bind frame
void bind_frame(FRAME* fr)
{
    gframe = fr; 
}

/// Return currently used frame
FRAME* get_current_frame()
{
    return gframe;
}

/// Clear frame
void clear_frame(Uint8 index)
{
    memset(gframe->colorData,index,gframe->size);
}

/// Draw a non-scaled bitmap
void draw_bitmap(BITMAP* b, int dx, int dy, int flip)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = 0; // Pixel X
    int py = 0; // Pixel Y

    // Go though every pixel and put them to a frame
    for(; y < dy+b->h; y++)
    {
        for(x = dx; x < dx+b->w; x++)
        {
            put_pixel(x,y, b->data[py*b->w +px]);
            px ++;
        }
        py ++;
        px = 0;
    } 
}

/// Draw a bitmap region
void draw_bitmap_region(BITMAP* b, int sx, int sy, int sw, int sh, int dx, int dy, int flip)
{
    dx += transX;
    dy += transY;

    int x; // Screen X
    int y = dy; // Screen Y
    int px = sx; // Pixel X
    int py = sy; // Pixel Y

    int beginx = flip == FLIP_NONE ? dx : dx+sw-1;
    int endx = flip == FLIP_NONE ? dx+sw : dx;
    int stepx = flip == FLIP_NONE ? 1 : -1;

    // Go though every pixel and put them to a frame
    for(; y < dy+sh; y++)
    {
        for(x = beginx; x != endx; x += stepx)
        {
            put_pixel(x,y, b->data[py*b->w +px]);

            px ++;
        }
        py ++;
        px = sx;
    } 
}

/// Draw text using a bitmap font
void draw_text(BITMAP* b, Uint8* text, int len, int dx, int dy, int xoff, int yoff, bool center)
{
    int x = dx;
    int y = dy;
    int cw = b->w / 16;
    int ch = b->h / 16;
    int i=0;
    Uint8 c;
    int sx;
    int sy;

    if(center)
    {
        x -= (len+1)/2 * (cw+xoff);
    }

    for(; i < len && text[i] != '\0'; i++)
    {
        c = text[i];
        sx = c % 16;
        sy = c / 16;

        draw_bitmap_region(b,sx*16,sy*16,cw,ch,x,y,0);

        x += cw + xoff;
    }
}

/// Fill rectangle
void fill_rect(int x, int y, int w, int h, Uint8 index)
{
    x += transX;
    y += transY;

    int dx = x;
    int dy = y;

    for(; dy < y+h; dy++)
    {
        for(dx = x; dx < x+w; dx++)
        {
            put_pixel(dx,dy,index);
        }
    }
}

/// Set translation
void set_translation(int x, int y)
{
    transX = x;
    transY = y;
}