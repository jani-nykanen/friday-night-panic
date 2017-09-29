/// Configuration file (header)
/// (c) 2017 Jani Nykänen

#ifndef __CONFIG__
#define __CONFIG__

/// Configuration type
typedef struct
{
    int winWidth; /// Default window width
    int winHeight; /// Default window height
    int canvasWidth; /// Canvas width
    int canvasHeight; /// Canvas height
    int fps; /// Maximum fps
    int fullscreen; /// Fullscreen enabled
    char assPath[256]; /// Asset path
}
CONFIG;

#endif // __CONGIF__