/// Controls (source)
/// (c) 2017 Jani Nykänen

#include "controls.h"

/// Amount of keys
#define KEYMAX 256
/// Button max
#define BUTTONMAX 4

/// An array of keys
static STATE keystate[KEYMAX];
/// An array of mouse buttons
static STATE mousestate[BUTTONMAX];
/// Cursor pos
static SDL_Point mousePos;
/// Mouse wheel pos
static int wheel;

/// Init controls
void ctr_init()
{
    // Let's initialize the keystate array (put UP state to all keys)
    int i = 0;
    for(; i < KEYMAX; i++)
    {
        keystate[i] = UP;
        if( i < BUTTONMAX)
            mousestate[i] = UP;
    }

    wheel = 0;
    mousePos.x = 0;
    mousePos.y = 0;
}

/// On key down
void ctr_on_key_down(SDL_Scancode scancode)
{
    int key = (int)scancode;

    // If the key is in range, make it be pressed
    if(key < 0 || key >= KEYMAX || keystate[key] == DOWN) 
        return;
    
    keystate[key] = PRESSED;
}

/// On key up
void ctr_on_key_up(SDL_Scancode scancode)
{
    int key = (int)scancode;

    // If the key is in range, make it be released
    if(key < 0 || key >= KEYMAX || keystate[key] == UP) 
        return;
    
    keystate[key] = RELEASED;
}

/// Mouse move event
void ctr_on_mouse_move(int x, int y)
{
    mousePos.x = x;
    mousePos.y = y;
}

/// Mouse button pressed
void ctr_on_mouse_down(int button)
{
    if(button < 0 || button >= BUTTONMAX || mousestate[button] == DOWN) 
        return;
    
    mousestate[button] = PRESSED;
}

/// Mouse button released
void ctr_on_mouse_up(int button)
{
    if(button < 0 || button >= BUTTONMAX || mousestate[button] == UP) 
        return;

    mousestate[button] = RELEASED;
}

/// Mouse wheel event
void ctr_on_mouse_wheel(int pos)
{
    wheel = pos;
}

/// Update
void ctr_update()
{
    wheel = 0;

    /*
     * Go through every key. If key is pressed, make it be
     * down, if released, up. This way pressed and released
     * states will only last a frame.
     */
    int i = 0;
    for(; i < KEYMAX; i++)
    {
        if(keystate[i] == RELEASED)
            keystate[i] = UP;

        else if(keystate[i] == PRESSED)
            keystate[i] = DOWN;

        if(i < BUTTONMAX)
        {
            if(mousestate[i] == RELEASED)
                mousestate[i] = UP;

            else if(mousestate[i] == PRESSED)
                mousestate[i] = DOWN;
        }
    }
}

/// Get key state
STATE get_key_state(int key)
{
    if(key < 0 || key >= KEYMAX) 
        return UP;

    return keystate[key];
}

/// Get button state
STATE get_mouse_button_state(int button)
{
    if(button < 0 || button >= BUTTONMAX) 
        return UP;
    
    return mousestate[button];
}

/// Get mouse pos
SDL_Point get_cursor_pos()
{
    return mousePos;
}

/// Get wheel position
int get_mouse_wheel()
{
    return wheel;
}