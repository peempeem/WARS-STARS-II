#include "gdu.h"




int gdu_run() {
    if (gdu_mem->running)
        return 0;
    gdu_mem->running = 1;
    return 1;
}

int gdu_is_running() { return gdu_mem->running; }

void gdu_set_frame(uint16_t* frame) { gdu_mem->frame_address = frame; }

void construct_sprite(sprite_t* sprite, const uint16_t* address, uint16_t w, uint16_t h, uint16_t dx, uint16_t dy, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey) {
    sprite->address     = (uint16_t*) address;
    sprite->dimensions  = ((uint32_t) w << 16) + h;
    sprite->screen_xy   = ((uint32_t) dx << 16) + dy;
    sprite->start_xy    = ((uint32_t) sx << 16) + sy;
    sprite->end_xy      = ((uint32_t) ex << 16) + ey;
}

int allocate_sprite(sprite_t* sprite) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (gdu_mem->sprites[i].address == 0) {
            gdu_mem->sprites[i] = *sprite;
            return i;
        }
    }
    return -1;
}

int pop_sprite(int index) {
    if (index >= 0 && index < MAX_SPRITES) {
        gdu_mem->sprites[index].address = 0;
        return 1;
    }
    return 0;
}