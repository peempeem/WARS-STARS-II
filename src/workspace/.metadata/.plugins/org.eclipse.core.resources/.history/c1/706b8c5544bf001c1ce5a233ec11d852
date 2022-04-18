#include "vga.h"

void clearScreen() {
    for (int i = 0; i < 2; i++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            for (int x = 0; x < SCREEN_WIDTH; x++)
                setPixel(i, x, y, 0);
        }
    }
}

void setPixel(int frame, uint32_t x, uint32_t y, uint16_t color) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    switch (frame) {
        case 0:
            vga_mem->frame0[y * SCREEN_WIDTH + x] = color;
            break;
        case 1:
            vga_mem->frame1[y * SCREEN_WIDTH + x] = color;
            break;
    }
}

int getFrame() { return vga_mem->control & 0x01; }
