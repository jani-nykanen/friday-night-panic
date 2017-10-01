/// Status (source)
/// (c) 2017 Jani Nykänen

#include "status.h"

/// Global status
static STATUS gstat; 

/// Initialize global status
void init_global_status()
{
    gstat.time = 2.5f * 60.0f * 60.0f;
    gstat.lives = 3;
}

/// Get the global status object
/// > Status object
STATUS* get_global_status()
{
    return &gstat;
}