#include <stdio.h>
#include <stdint.h>

#include "sprites/background.h"
#include "sprites/playerplanet.h"
#include "sprites/enemyplanet.h"
#include "sprites/redplanet.h"
#include "sprites/enemy_ship.h"


#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_SIZE     640 * 480

typedef struct VGA_MEMORY {
    uint16_t frame0[SCREEN_SIZE];
    uint16_t frame1[SCREEN_SIZE];
} vga_memory_t;

typedef struct VGA_REGISTERS {
    vga_memory_t*   address;
    uint32_t        frame;
} vga_regs_t;

vga_memory_t    vga_mem;
vga_regs_t*     vga_regs = (vga_regs_t*) 0x00002158;

void setPixel(uint16_t* frame, uint32_t x, uint32_t y, uint16_t color) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        frame[y * SCREEN_WIDTH + x] = color;
}

int main() {
    printf("Hello from Nios II!\n");

    printf("Initializing VGA control registers...");
    vga_regs->address   = &vga_mem;
    vga_regs->frame     = 0;
    printf("Done\n");

    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            setPixel(vga_mem.frame0, x, y, background_bitmap[y * background_width + x]);
            setPixel(vga_mem.frame1, x, y, background_bitmap[y * background_width + x]);
        }
    }

    for (int y = 0; y < playerplanet_height; y++) {
        for (int x = 0; x < playerplanet_width; x++) {
            uint16_t color = playerplanet_bitmap[y * playerplanet_width + x];
            if (color & 0x1000)
                setPixel(vga_mem.frame0, x - playerplanet_width / 2, y - playerplanet_height / 2 + SCREEN_HEIGHT / 2, color);
        }
    }

    for (int y = 0; y < enemyplanet_height; y++) {
        for (int x = 0; x < enemyplanet_width; x++) {
            uint16_t color = enemyplanet_bitmap[y * enemyplanet_width + x];
            if (color & 0x1000) {
                setPixel(vga_mem.frame0, x - enemyplanet_width / 2 + SCREEN_WIDTH, y - enemyplanet_height / 2 + SCREEN_HEIGHT / 2, color);
                setPixel(vga_mem.frame1, x - enemyplanet_width / 2 + SCREEN_WIDTH / 2, y - enemyplanet_height / 2 + SCREEN_HEIGHT / 2, color);
            }
        }
    }

    for (int y = 0; y < redplanet_height; y++) {
        for (int x = 0; x < redplanet_width; x++) {
            uint16_t color = redplanet_bitmap[y * redplanet_width + x];
            if (color & 0x1000)
                setPixel(vga_mem.frame0, x - redplanet_width / 2 + SCREEN_WIDTH / 2, y - redplanet_height / 2 + SCREEN_HEIGHT, color);
        }
    }

    for (int y = 0; y < enemy_ship_height; y++) {
        for (int x = 0; x < enemy_ship_width; x++) {
            uint16_t color = enemy_ship_bitmap[y * enemy_ship_width + x];
            if (color & 0x1000)
                setPixel(vga_mem.frame0, x - enemy_ship_width / 2 + SCREEN_WIDTH / 2, y - enemy_ship_height / 2 + SCREEN_HEIGHT / 2, color);
        }
    }

    while (1) {
        vga_regs->frame = 0;
        usleep(100000);
        vga_regs->frame = 1;
        usleep(100000);
    }

    return 0;
}
