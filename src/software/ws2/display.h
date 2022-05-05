#ifndef DISPLAY_H
#define DISPLAY_H

#include "mouse.h"
#include "game.h"

#define FADE_TO     0
#define FADE_FROM   1

typedef struct FADE {
    float       start;
    float       end;
    int         dir;
    uint16_t    color;
} fade_t;

void    start_render();
void    double_render();
fade_t  create_fade(uint16_t color, int dir);
void    start_fade(fade_t* fade, float seconds);
int     is_fade_done(fade_t* fade);
int     show_fade(fade_t* fade);
int     is_clicked(mouse_t* mouse, game_object_t* object);
void    handle_mouse(mouse_t* mouse, scene_t* scene, int h_scroll, int v_scroll);

#endif
