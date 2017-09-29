/// Main file (source)
/// (c) 2017 Jani Nykänen

#include "engine/app.h"
#include "game/game.h"

#include "engine/list.h"
#include "engine/assets.h"

#include "stdlib.h"

/// Configuration
static CONFIG c;

/// Read configuration file
/// < path Path
/// > 0 on success, 1 on error
static int read_config(const char* path)
{
    if(load_list(path) != 0)
    {
        return 1;
    }

    char* param;
    char* value;
    int count = 0;

    int i = 0;
    for(; i <= get_list_word_count(); i++)
    {
        if(count == 0)
        {
            param = get_list_word(i).data;
            count ++;
        }
        else if(count == 1)
        {
            value = get_list_word(i).data;

            if(strcmp(param,"win_width") == 0)
            {
                c.winWidth = atoi(value);
            }
            else if(strcmp(param,"win_height") == 0)
            {
                c.winHeight = atoi(value);
            }
            else if(strcmp(param,"fullscreen") == 0)
            {
                c.fullscreen = atoi(value);
            }
            else if(strcmp(param,"fps") == 0)
            {
                c.fps = atoi(value);
            }
            else if(strcmp(param,"asset_path") == 0)
            {
                strcpy(c.assPath,value);
            }
            else if(strcmp(param,"canvas_width") == 0)
            {
                c.canvasWidth = atoi(value);
            }
            else if(strcmp(param,"canvas_height") == 0)
            {
                c.canvasHeight = atoi(value);
            }

            count = 0;
        }
    }

    return 0;
}

/// Main function
/// < argc Argument count
/// < argv Argument values
/// > Error code, 0 on success, 1 on error
int main(int argc, char** argv)
{
    // Set scenes
    SCENE scenes[] = {
        get_game_scene()
    };
    int sceneCount = 1;

    // Load config
    
    if(read_config("config.list") != 0)
    {
        return 1;
    }

    return app_run(scenes,sceneCount,c);
}