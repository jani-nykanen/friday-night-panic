/// Pause scene (source)
/// (c) 2017 Jani Nykänen

#include "pause.h"

#include "engine/app.h"
#include "engine/graphics.h"
#include "engine/assets.h"

#include "vpad.h"

/// Font bitmap
static BITMAP* bmpFont;
/// Hand bitmap
static BITMAP* bmpHand;

/// Pause mode
static int mode;
/// Hand pos
static int handPos;

/// Init pause scene
static int pause_init()
{
    bmpFont = get_bitmap("font16");
    bmpHand = get_bitmap("hand");
    mode = 0;
    handPos = 1;

    return 0;
}

/// Draw pause when mode is 0
static void draw_pause()
{
    fill_rect(160-48-1,100-16-1,96+2,32+2,0b00111101);
    fill_rect(160-48,100-16,96,32,0);

    draw_text(bmpFont,(Uint8*)"GAME PAUSED",11,160,100-8,-8,0,true);
}

/// Draw quit screen when mode is 1
static void draw_quit()
{
    fill_rect(160-96-1,100-24-1,192+2,48+2,0b00111101);
    fill_rect(160-96,100-24,192,48,0);

    draw_text(bmpFont,(Uint8*)"Do you want to quit?",20,160,100-20,-8,0,true);

    draw_text(bmpFont,(Uint8*)"YES        NO",13,160,100+2,-8,0,true);
    draw_bitmap(bmpHand,160-72 + 88 * handPos,100+2,0);
}

/// Update pause screen
/// < tm Time mul.
static void pause_update(float tm)
{
    if(vpad_get_button(2) == PRESSED)
    {
        if(mode == 0 || handPos == 1)
            app_swap_scene("game");
        else
        {
            app_terminate();
        }
    }

    if(vpad_get_button(3) == PRESSED)
    {
        app_swap_scene("game");
    }

    if(fabs(vpad_get_stick().x) > 0.1f)
        handPos = vpad_get_stick().x > 0.0f ? 1 : 0;

    vpad_update();
}

/// Draw pause scene
static void pause_draw()
{
    if(mode == 0)
        draw_pause();
    else
        draw_quit();
}

/// Set pause mode
void set_pause_mode(int m)
{
    mode = m;
    handPos = 1;
}

/// Return the pause scene
SCENE pause_get_scene()
{
    // Set scene functions
    SCENE s = (SCENE){pause_init,pause_update,pause_draw,NULL};
        
    // Set scene name
    set_scene_name(&s,"pause");

    return s;
        
}
