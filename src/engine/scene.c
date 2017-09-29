/// Scene (source)
/// (c) 2017 Jani Nykänen

#include "scene.h"

#include "string.h"

/// Set scene name
void set_scene_name(SCENE* s, const char* name)
{
    // Clear old name
    int i = 0;
    for(; i < 8; i++)
    {
        s->name[i] = 0;
    }

    // Copy new name (and null terminate, if possible)
    for(i=0; i < strlen(name); i++)
    {
        s->name[i] = name[i];
    }
    if(i < 7)
        s->name[7] = '\0';
}