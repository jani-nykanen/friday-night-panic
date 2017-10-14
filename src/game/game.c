/// Game scene (source)
/// (c) 2017 Jani Nykänen

#include "game.h"

#include "../engine/app.h"
#include "../engine/graphics.h"
#include "../engine/bitmap.h"
#include "../engine/controls.h"
#include "../engine/assets.h"
#include "../engine/tilemap.h"

#include "objcontrol.h"
#include "stage.h"
#include "hud.h"
#include "status.h"

#include "../vpad.h"
#include "../pause.h"

/// Is the game over
static int gameOver;

/// Game Over! timer
static float goverTimer;

/// Game over bitmap
static BITMAP* bmpGameOver;

/// Init game
static int game_init()
{

    // Init object control
    init_object_control();
    // Init stage
    init_stage();
    /// Init hud
    init_hud();
    /// Init status
    init_global_status();

    // Init vpad
    // TODO: Move to somewhere else, maybe?
    vpad_init();
    vpad_add_button(0,(int)SDL_SCANCODE_Z,1);
    vpad_add_button(1,(int)SDL_SCANCODE_X,2);
    vpad_add_button(2,(int)SDL_SCANCODE_RETURN,9);
    vpad_add_button(3,(int)SDL_SCANCODE_ESCAPE,8);

    // Set default values of things
    gameOver = 0;
    goverTimer = 0.0f;

    // Init bitmaps
    bmpGameOver = get_bitmap("gameOver");

    return 0;
}

/// Update game
/// tm Time multiplier
static void game_update(float tm)
{
    // If game over, update timer and ignore every
    // else
    if(gameOver != 0)
    {
        goverTimer -= 1.0f * tm;
        if(goverTimer <= 0.0f)
        {
            gameOver = false;
        }
        return;
    }

    get_global_status()->time -= 1.0f * tm;

    update_obj_control(tm);
    stage_update(tm);
    hud_update(tm);

    vpad_update();

    // TEMP, put into another file
    // Palette swap
    if(get_key_state((int)SDL_SCANCODE_F2) == PRESSED)
    {
        FRAME* f = get_current_frame();
        frame_swap_cga_palette(f);
    }

    // Pause & quit
    if(vpad_get_button(2) == PRESSED)
    {
        set_pause_mode(0);
        app_swap_scene("pause");
    }
    else if(vpad_get_button(3) == PRESSED)
    {
        set_pause_mode(1);
        app_swap_scene("pause");
    }
}

/// Draw game
static void game_draw()
{
    
    if(gameOver != 0)
    {
        draw_bitmap_region(bmpGameOver,0,32*(gameOver-1),128,32, 160-64,72,0);
        return;
    }
    clear_frame(0);    

    set_translation(0,0);
    stage_draw();
    draw_objects();

    set_translation(0,0);
    hud_draw();
}

/// Destroy game
static void game_destroy()
{
    destroy_assets();
}

/// Enable game over
void enable_game_over(int mode)
{
    gameOver = mode;
    goverTimer = 90.0f + ((mode != 1) ? 30.0f : 0.0f);
    stage_shake(0.0f);
}

/// Get game scene
SCENE get_game_scene()
{
    // Set scene functions
    SCENE s = (SCENE){game_init,game_update,game_draw,game_destroy};

    // Set scene name
    set_scene_name(&s,"game");

    return s;
}