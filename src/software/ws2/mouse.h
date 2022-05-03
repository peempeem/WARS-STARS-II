#ifndef MOUSE_H
#define MOUSE_H

#include "common.h"

typedef struct MOUSE_BUTTONS {
    int left,
        right;
} mouse_buttons_t;

typedef struct MOUSE {
    position_t      pos;
    mouse_buttons_t buttons;
} mouse_t;

void    init_mouse();
mouse_t new_mouse();
int     poll_mouse(mouse_t* mouse, int reverse_x, int reverse_y);

#endif
