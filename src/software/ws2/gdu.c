#include "gdu.h"


#define GDU_MEM_LOCATION 0x00004000

typedef struct GDU_SPRITE {
    uint16_t*   address;
    uint32_t    dimensions,
                screen_xy,
                start_xy,
                end_xy;
} gdu_sprite_t;

typedef struct GDU_MEMORY {
    uint32_t        running;
    uint16_t*       frame_address;
    gdu_sprite_t    sprites[MAX_SPRITES];
} gdu_memory_t;

static volatile gdu_memory_t* gdu_mem = (gdu_memory_t*) GDU_MEM_LOCATION;

int gdu_run() {
    if (gdu_mem->running)
        return 0;
    gdu_mem->running = 1;
    return 1;
}

int gdu_is_running() { return gdu_mem->running; }

void gdu_reset() {
    for (int i = 0; i < MAX_SPRITES; i++)
        pop_sprite(i);
}

void gdu_set_frame(uint16_t* frame) { gdu_mem->frame_address = frame; }

void convert_sprite(volatile gdu_sprite_t* gdu_sprite, sprite_t* sprite) {
    gdu_sprite->address     = (uint16_t*) sprite->address;
    gdu_sprite->dimensions  = ((uint32_t) sprite->width     << 16) + sprite->height;
    gdu_sprite->screen_xy   = ((uint32_t) sprite->screen_x  << 16) + sprite->screen_y;
    gdu_sprite->start_xy    = ((uint32_t) sprite->start_x   << 16) + sprite->start_y;
    gdu_sprite->end_xy      = ((uint32_t) sprite->end_x     << 16) + sprite->end_y;
}

int push_sprite(sprite_t* sprite, uint32_t index) {
    if (index > MAX_SPRITES)
        return 0;
    convert_sprite(&gdu_mem->sprites[index], sprite);
    return 1;
}

int pop_sprite(int index) {
    if (index > MAX_SPRITES)
        return 0;
    gdu_mem->sprites[index].address = 0;
    return 1;
}
