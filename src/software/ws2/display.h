#ifndef DISPLAY_H
#define DISPLAY_H

#include "mouse.h"
#include "game.h"

typedef struct FADE {
    float       start;
    float       end;
    uint16_t    color;
} fade_t;

void    start_render();
void    double_render();
fade_t  create_fade(uint16_t color, float seconds);
int     fade_to(fade_t* fade);
int     fade_from(fade_t* fade);
int     is_clicked(mouse_t* mouse, game_object_t* object);
void    handle_mouse(mouse_t* mouse, scene_t* scene, int h_scroll, int v_scroll);

#endif
