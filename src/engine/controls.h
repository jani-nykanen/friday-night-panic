/// Controls (header)
/// (c) 2017 Jani Nykänen

#ifndef __CONTROLS__
#define __CONTROLS__

#include <SDL2/SDL.h>

/// Key states
typedef enum
{
    UP = 0,
    DOWN = 1,
    PRESSED = 2,
    RELEASED = 3 
}
STATE;

/// Initialize controls
void ctr_init();

/// Update controls
void ctr_update();

/// Triggered when a key is pressed down
///scancode Key scancode
void ctr_on_key_down(SDL_Scancode scancode);

///Triggered when a key is released
/// scancode Key scancode
void ctr_on_key_up(SDL_Scancode scancode);

/// Mouse move event
/// x Mouse x
/// y Mouse y
void ctr_on_mouse_move(int x, int y);

/// Mouse button pressed event
/// button Button
void ctr_on_mouse_down(int button);

/// Mouse button released event
/// button Button
void ctr_on_mouse_up(int button);

/// Mouse wheel event
/// pos Wheel position
void ctr_on_mouse_wheel(int pos);

/// Get a key state
/// key Key
/// Returns a key state
STATE get_key_state(int key);

/// Get a mouse button state
/// button Button
/// Returns a button state
STATE get_mouse_button_state(int button);

/// Returns cursor position
SDL_Point get_cursor_pos();

/// Returns the mouse wheel position
int get_mouse_wheel();

#endif // __CONTROLS__