#include "vga.h"

void vga_init() {
    vga_regs->address   = &vga_mem - 1;
    vga_regs->frame     = 0;
}

void setPixel(uint16_t* frame, uint32_t x, uint32_t y, uint16_t color) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        frame[y * SCREEN_WIDTH + x] = color;
}

uint16_t getPixel(uint16_t* frame, uint32_t x, uint32_t y) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        return frame[y * SCREEN_WIDTH + x];
}
