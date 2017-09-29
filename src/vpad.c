/// Virtual game pad (source)
/// (c) 2017 Jani Nykänen

#include "vpad.h"

/// Button object
typedef struct
{
    int scancode;
    int joybutton;
}
BUTTON;

/// The good ol' gamepad stick
static VEC2 stick;
/// Old stick
static VEC2 oldStick;
/// Delta
static VEC2 delta;
/// Buttons
static BUTTON buttons[256];

/// Initialize virtual gamepad
void vpad_init()
{
    stick.x = 0.0f;
    stick.y = 0.0f;
}

/// Update vpad
void vpad_update()
{
    oldStick = stick;

    stick.x = 0.0f;
    stick.y = 0.0f;

    if(get_key_state((int)SDL_SCANCODE_LEFT) == DOWN)
    {
        stick.x = -1.0f;
    }
    else if(get_key_state((int)SDL_SCANCODE_RIGHT) == DOWN)
    {
        stick.x = 1.0f;
    }

    if(get_key_state((int)SDL_SCANCODE_UP) == DOWN)
    {
        stick.y = -1.0f;
    }
    else if(get_key_state((int)SDL_SCANCODE_DOWN) == DOWN)
    {
        stick.y = 1.0f;
    }

    delta.x = stick.x - oldStick.x;
    delta.y = stick.y - oldStick.y;
}

/// Get stick axis
VEC2 vpad_get_stick()
{
    return stick;
}

/// Get stick delta
VEC2 vpad_get_delta()
{
    return delta;
}

/// Add a new button
void vpad_add_button(Uint8 index, int scancode, int joybutton)
{
    buttons[index] = (BUTTON){scancode,joybutton};
}

/// Get virtual pad button state
int vpad_get_button(Uint8 index)
{
    return get_key_state(buttons[index].scancode);
}