#ifndef GDU_H
#define GDU_H

#include <stdint.h>

#define MAX_SPRITES 64

typedef struct SPRITE {
    const uint16_t* address;
    uint32_t        width,
                    height,
                    screen_x,
                    screen_y,
                    start_x,
                    start_y,
                    end_x,
                    end_y;
} sprite_t;

int     gdu_run();
int     gdu_is_running();
void    gdu_reset();
void    gdu_set_frame(uint16_t* frame);
int     push_sprite(sprite_t* sprite, uint32_t index);
int     pop_sprite(int index);

#endif