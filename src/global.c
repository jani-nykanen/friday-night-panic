/// Global scene (source)
/// (c) 2017 Jani Nykänen

#define SDL_MAIN_HANDLED


#include "global.h"

#include "vpad.h"
#include "engine/frame.h"
#include "engine/graphics.h"

#include "stdlib.h"
#include "math.h"
#include "stdio.h"


/// Initialize global scene
/// > 0 on success, 1 on error
static int global_init()
{
    // Init vpad
    vpad_init();
    vpad_add_button(0,(int)SDL_SCANCODE_Z,0);
    vpad_add_button(1,(int)SDL_SCANCODE_SPACE,1);
    vpad_add_button(2,(int)SDL_SCANCODE_RETURN,7);
    vpad_add_button(3,(int)SDL_SCANCODE_ESCAPE,6);

    return 0;
}

/// Update global scene
/// < tm Time mul.
static void global_update(float tm)
{
    // Palette swap
    if(get_key_state((int)SDL_SCANCODE_F2) == PRESSED)
    {
        FRAME* f = get_current_frame();
        frame_swap_cga_palette(f);
    }

    vpad_update();
}

/// Return the global scene
SCENE get_global_scene()
{
    // Set scene functions
    SCENE s = (SCENE){global_init,global_update,NULL,NULL};
        
    // Set scene name
    set_scene_name(&s,"global");

    return s; 

}
