/// HUD (header)
/// (c) 2017 Jani Nykänen

#ifndef __HUD__
#define __HUD__

/// Initialize HUD
void init_hud();

/// Update HUD
/// < tm Time mul.
void hud_update(float tm);

/// Draw HUD
void hud_draw();

#endif // __HUD__