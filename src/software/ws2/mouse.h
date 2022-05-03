#ifndef MOUSE_H
#define MOUSE_H

#include "common.h"

typedef struct MOUSE_DATA {
    position_t  pos;
    int         left_click,
                right_click;
} mouse_t;

void    init_mouse();
mouse_t new_mouse();
int     poll_mouse(mouse_t* mouse);

#endif
