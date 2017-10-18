/// HUD (source)
/// (c) 2017 Jani Nykänen

#include "hud.h"

#include "stage.h"
#include "status.h"

#include "../engine/assets.h"
#include "../engine/tilemap.h"
#include "../engine/graphics.h"

/// Font bitmap
static BITMAP* bmpFont;
/// Frame bitmap
static BITMAP* bmpFrame;

/// Time string
static Uint8 timeString[16];
/// Darkness timer
static float darkTimer;

/// Set time string
static void set_time_string()
{   
    float gameTime = get_global_status()->time;

    if(gameTime <= 0.0f)
    {
        const char* s = "TIME: 0:00";
        strcpy((char*)timeString,s);
        return;
    }

    timeString[0] = 'T';
    timeString[1] = 'I';
    timeString[2] = 'M';
    timeString[3] = 'E';
    timeString[4] = ':';
    timeString[5] = ' ';

    timeString[6] = 48+ (int)floor(gameTime/ 3600.0f);
    timeString[7] = ':';
    float sec = (float) ( (int)floor(gameTime/60.0f) % 60 );
    timeString[8] = 48+  (int)floor(sec / 10.0f);
    timeString[9] = 48+ ((int)sec - 10* (int)floor(sec / 10.0f));
    timeString[10] = '\0'; 
}

/// Draw frame
static void draw_frame()
{
    // Draw corners
    // TODO: Use frame dimensions instead of numeric constants
    draw_bitmap_region(bmpFrame,0,0,8,8,0,0,FLIP_NONE);
    draw_bitmap_region(bmpFrame,24,0,8,8,320-8,0,FLIP_NONE);
    draw_bitmap_region(bmpFrame,0,24,8,8,0,168,FLIP_NONE);
    draw_bitmap_region(bmpFrame,24,24,8,8,320-8,168,FLIP_NONE);

    // Draw frame borders
    int i = 0;
    for(; i < 19; i++)
    {
        // Horizontal
        draw_bitmap_region(bmpFrame,8,0,16,8,8 + i*16,0,FLIP_NONE);
        draw_bitmap_region(bmpFrame,8,24,16,8,8 + i*16,168,FLIP_NONE);

        // Vertical
        if(i < 10)
        {
            draw_bitmap_region(bmpFrame,0,8,8,16,0,8 + i*16,FLIP_NONE);
            draw_bitmap_region(bmpFrame,24,8,8,16,312,8 + i*16,FLIP_NONE);
        }
    }
}

/// Draw HUD text
static void draw_hud_text()
{
    draw_text(bmpFont,get_current_map()->name,32,2,174,-7,0,false);

    set_time_string();
    draw_text(bmpFont,timeString,16,2,186,-7,0,false);

    int i = 0;
    for(; i < get_global_status()->lives; i++)
    {
        draw_text(bmpFont,(Uint8*)"* ",32,320-64 + i*18,186,-7,0,false);
    }
    draw_text(bmpFont,(Uint8*)"Lives:",32,320-64,174,-7,0,false);
}

/// Initialize HUD
void init_hud()
{
    bmpFrame = get_bitmap("frame");
    bmpFont = get_bitmap("font16");
    darkTimer = 60.0f;
}

/// Update HUD
void hud_update(float tm)
{
    if(darkTimer > 0.0f)
        darkTimer -= 1.0f * tm;
}

/// Return the dark timer
float hud_get_dark_timer()
{
    return darkTimer;
}

/// Reset dark timer
void hud_reset_dark_timer()
{
    darkTimer = 60.0f;
}

/// Draw HUD
void hud_draw()
{
    draw_frame();
    draw_hud_text();

    // Draw darkess
    if(darkTimer > 0.0f)
    {
        int pos = (int)(100.0f / 60.0f * darkTimer);
        fill_rect(0,0,320,pos,0);
        fill_rect(0,200-pos,320,pos,0);
    }
}
