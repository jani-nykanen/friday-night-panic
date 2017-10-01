/// Status (header)
/// (c) 2017 Jani Nykänen

#ifndef __STATUS__
#define __STATUS__

/// Status object
typedef struct
{
    int lives; /// Lives
    float time; /// Game time
}
STATUS;

/// Initialize global status
void init_global_status();

/// Get the global status object
/// > Status object
STATUS* get_global_status();

#endif // __STATUS__