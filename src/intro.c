/// Intro screen scene (header)
/// (c) 2017 Jani Nykänen

#include "intro.h"

#include "engine/app.h"
#include "engine/assets.h"
#include "engine/graphics.h"
#include "vpad.h"
#include "game/game.h"

/// Avatar bitmap
static BITMAP* bmpAvatar;
/// Bitmap font
static BITMAP* bmpFont;
/// Bling sound
static SOUND* sndBling;

/// Timer
static float timer;

/// Init pause scene
static int intro_init()
{
    bmpAvatar = get_bitmap("avatar");
    bmpFont = get_bitmap("font16");
    sndBling = get_sound("bling");

    timer = 0.0f;

    return 0;
}

/// Update pause screen
/// < tm Time mul.
static void intro_update(float tm)
{
    float oldTimer = timer;
    timer += ((timer <= 60.0f || timer >= 180.0f) ? 2.0f : 1.0f) * tm;
    if(timer >= 240.0f)
    {
        app_swap_scene("title");
    }

    if(oldTimer < 75.0f && timer >= 75.0f)
    {
        play_sound(sndBling,0.6f);
    }
}

/// Draw pause scene
static void intro_draw()
{
    clear_frame(0);

    float pos = 0.0f;
    float pos2 = 0.0f;
    if(timer <= 60.0f)
    {
        pos = -64.0f + (224.0f)/60.0f * timer;
        pos2 = 160.0f + (224.0f)/60.0f * (60.0f-timer);
    }
    else if(timer <= 180.0f)
    {
        pos = 160.0f;
        pos2 = pos;    
    }
    else if(timer <= 240.0f)
    {
        pos = 160.0f + (224.0f)/60.0f * (timer-180.0f);
        pos2 = -64.0f + (224.0f)/60.0f * (60.0f -timer+180.0f);
    }

    draw_bitmap(bmpAvatar,pos - 32.0f,48,0);
    draw_text(bmpFont,(Uint8*)"Created by",10,pos2,112.0f,-7.0f,0.0f,true);
    draw_text(bmpFont,(Uint8*)"JANI NYK@NEN",12,pos2,124.0f,-7.0f,0.0f,true);
}

/// Return the intro scene
SCENE intro_get_scene()
{
    // Set scene functions
    SCENE s = (SCENE){intro_init,intro_update,intro_draw,NULL};
        
    // Set scene name
    set_scene_name(&s,"intro");

    return s;
}
