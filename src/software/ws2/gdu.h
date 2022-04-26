#ifndef GDU_H
#define GDU_H

#include <stdint.h>

typedef struct SPRITE {
    uint16_t*   address;
    uint32_t    dimensions,
                screen_xy,
                start_xy,
                end_xy;
} sprite_t;

#define MAX_SPRITES         32
#define GDU_MEM_LOCATION    0x00004000

typedef struct GDU_MEMORY {
    uint32_t    running;
    uint16_t*   frame_address;
    sprite_t    sprites[MAX_SPRITES];
} gdu_memory_t;

static volatile gdu_memory_t* gdu_mem = (gdu_memory_t*) GDU_MEM_LOCATION;

int     gdu_run();
int     gdu_is_running();
void    gdu_set_frame(uint16_t* frame);
void    construct_sprite(sprite_t* sprite, const uint16_t* address, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);
int     allocate_sprite(sprite_t* sprite);
int     pop_sprite(int index);

#endif