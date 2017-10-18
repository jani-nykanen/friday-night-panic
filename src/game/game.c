/// Game scene (source)
/// (c) 2017 Jani Nykänen

#include "game.h"

#include "../engine/app.h"
#include "../engine/graphics.h"
#include "../engine/bitmap.h"
#include "../engine/controls.h"
#include "../engine/assets.h"
#include "../engine/tilemap.h"
#include "../engine/audio.h"

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
/// Game over music
static SOUND* sndGameOver;
/// Victory music
static SOUND* sndVictory;
/// Start music
static SOUND* sndStart;
/// Pause sound
static SOUND* sndPause;

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

    // Set default values of things
    gameOver = 0;
    goverTimer = 0.0f;

    // Init bitmaps & audio
    bmpGameOver = get_bitmap("gameOver");
    sndGameOver = get_sound("gameover");
    sndVictory = get_sound("victory");
    sndStart = get_sound("start");
    sndPause = get_sound("pause");

    return 0;
}

/// Update game
/// tm Time multiplier
static void game_update(float tm)
{
    hud_update(tm);

    // If dark, skip events
    if(hud_get_dark_timer() > 0.0f)
        return;

    // If game over, update timer and ignore every
    // else
    if(gameOver != 0)
    {
        goverTimer -= 1.0f * tm;
        if(goverTimer <= 0.0f)
        {
            gameOver = false;
            play_sound(sndStart,0.65f);
        }
        return;
    }

    get_global_status()->time -= 1.0f * tm;

    update_obj_control(tm);
    stage_update(tm);

    // Pause & quit
    if(vpad_get_button(2) == PRESSED)
    {
        play_sound(sndPause,0.60f);
        set_pause_mode(0);
        app_swap_scene("pause");
    }
    else if(vpad_get_button(3) == PRESSED)
    {
        play_sound(sndPause,0.60f);
        set_pause_mode(1);
        app_swap_scene("pause");
    }

    // TEMP
    if(get_key_state((int)SDL_SCANCODE_P) == PRESSED)
    {
        app_swap_scene("title");
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
    goverTimer = 90.0f + ((mode != 1) ? 60.0f : 30.0f);
    stage_shake(0.0f);

    play_sound(mode != 2 ? sndGameOver : sndVictory,0.70f);
}

/// Reset game
void game_reset()
{
    play_sound(sndStart,0.65f);
    reset_obj_control();
    hud_reset_dark_timer();
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