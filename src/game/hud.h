/// HUD (header)
/// (c) 2017 Jani Nykänen

#ifndef __HUD__
#define __HUD__

/// Initialize HUD
void init_hud();

/// Update HUD
/// < tm Time mul.
void hud_update(float tm);

/// Return the dark timer
float hud_get_dark_timer();

/// Reset dark timer
void hud_reset_dark_timer();

/// Draw HUD
void hud_draw();

#endif // __HUD__