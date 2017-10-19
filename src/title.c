/// Title screen scene (header)
/// (c) 2017 Jani Nykänen


#include "title.h"

#include "engine/app.h"
#include "engine/assets.h"
#include "engine/graphics.h"
#include "vpad.h"
#include "game/game.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Logo bitmap
static BITMAP* bmpLogo;
/// Bitmap font
static BITMAP* bmpFont;
/// Hand bitmap
static BITMAP* bmpHand;
/// Beginning sound
static SOUND* sndBegin;
/// Select sound
static SOUND* sndSelect;
/// Accept sound
static SOUND* sndAccept;

/// Phase
static int phase;
/// Timer
static float timer;
/// Hand pos
static int handPos;
/// Needed for something
static bool released;

/// Draw menu text
/// < x X coordinate
/// < y Y coordinate
static void draw_text_content(int x, int y)
{
    FRAME* f = get_current_frame();

    // Draw menu options
    draw_text(bmpFont,(Uint8*)"PLAY GAME",11,x,y,-7,0,true);
    draw_text(bmpFont,(Uint8*)"FULL SCREEN",11,x,y+14,-7,0,true);
    draw_text(bmpFont,
        is_audio_enabled() ? (Uint8*)"AUDIO: ON" : (Uint8*)"AUDIO: OFF",
        11,x,y+28,-7,0,true);
    draw_text(bmpFont,
        (f->palette[0b00110101*3+2] == 85) ? (Uint8*)"PALETTE: 1" : (Uint8*)"PALETTE: 2",
        11,x,y+42,-7,0,true);
    draw_text(bmpFont,(Uint8*)"QUIT GAME",11,x,y+56,-7,0,true);

    // Draw hand cursor
    draw_bitmap(bmpHand,160-70,y + handPos*14 + 2,FLIP_NONE);

    // Draw copyright
    draw_text(bmpFont,(Uint8*)"(c)2017 Jani Nyk~nen",21,x,y+76,-7,0,true);
}

/// Take action
static void take_action()
{

    switch(handPos)
    {
    case 0:

        phase = 3;
        timer = 0.0f;
        break;

    case 1:
        app_toggle_fullscreen();
        break;

    case 2:
        // Toggle audio
        enable_audio(!is_audio_enabled());
        break;

    case 3:
        frame_swap_cga_palette(get_current_frame());
        break;

    case 4:
        app_terminate();
        break;

    default:
        break;
    }

    play_sound(sndAccept,0.50f);
}

/// Init pause scene
static int title_init()
{
    bmpLogo = get_bitmap("logo");
    bmpFont = get_bitmap("font16");
    bmpHand = get_bitmap("hand");

    sndBegin = get_sound("begin");
    sndSelect = get_sound("select");
    sndAccept = get_sound("accept");

    timer = 0.0f;
    phase = 0;
    released = false;

    return 0;
}

/// Update pause screen
/// < tm Time mul.
static void title_update(float tm)
{
    if(phase != 2)
        timer += 1.0f * tm;

    if(timer >= 60.0f)
    {
        if(phase == 0)
            play_sound(sndBegin,0.50f);
            
        if(phase == 3)
        {
            game_reset();
            app_swap_scene("game");
            phase = 0;
            timer = 0.0f;
            return;
        }

        phase ++;
        timer -= 60.0f;
    }

    if(phase == 2)
    {
        int hpos = handPos;

        if((vpad_get_button(0) == PRESSED || vpad_get_button(1) == PRESSED) || vpad_get_button(2) == PRESSED)
        {
            take_action();
            return;
        }

        float d = vpad_get_stick().y;
        if(fabs(d) > 0.25)
        {  
            if(released)
            {

                handPos += d > 0.0f ? 1 : -1;
                if(handPos < 0)
                    handPos = 4;
                else if(handPos > 4)
                    handPos = 0;

                 released = false;   
            }
        }
        else
        {
            released = true;
        }

        if(hpos != handPos)
        {
            play_sound(sndSelect,0.50f);
        }
    }
}

/// Draw pause scene
static void title_draw()
{
    clear_frame(0);

    if(phase == 0)
    {
        float pos = -160.0f + 320.0f/60.0f * timer;

        int i = 0;
        for(; i < bmpLogo->h / 2; i++)
        {
            draw_bitmap_region(bmpLogo,0,i*2,bmpLogo->w,2, 
                i % 2 == 0 ? pos-bmpLogo->w/2 : 320.0f-pos-bmpLogo->w/2,-12 + i*2,FLIP_NONE);
        }
    }
    else if(phase != 3)
    {
        if(phase == 1)
        {
            draw_text_content(160,200 - (int) ( 92.0f/60.0f * timer) );
        }
        else
        {
            draw_text_content(160,200-92);
        }

        draw_bitmap(bmpLogo,160-bmpLogo->w/2,-12,FLIP_NONE);
    }
    else
    {
        draw_bitmap(bmpLogo,160-bmpLogo->w/2,-12 - 128/60.0 * timer,FLIP_NONE);
        draw_text_content(160,200-92/60.0 * (60.0f-timer));
    }
}

/// Return the title scene
SCENE title_get_scene()
{
    // Set scene functions
    SCENE s = (SCENE){title_init,title_update,title_draw,NULL};
        
    // Set scene name
    set_scene_name(&s,"title");

    return s;
}
