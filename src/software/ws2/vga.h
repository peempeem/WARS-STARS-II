#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_SIZE     640 * 480

void        vga_init();
void        set_frame(uint32_t frame);
uint16_t*   get_frame(uint32_t frame);
int         is_vblank();
void        set_aura(uint16_t color, uint32_t bias);
void        setPixel(uint16_t* frame, uint32_t x, uint32_t y, uint16_t color);
uint16_t    getPixel(uint16_t* frame, uint32_t x, uint32_t y);

#endif
