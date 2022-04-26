#include <stdio.h>

#include "vga.h"
#include "gdu.h"

#include "sprites/background.h"
#include "sprites/enemyplanet.h"
#include "sprites/enemy_ship.h"

typedef struct Blitter {
    uint32_t    running;
    uint16_t*   frame_address;
    sprite_t    sprite;
    uint32_t    reserved;
} blitter_t;

static volatile blitter_t* blitter = (blitter_t*) (0x00004000);

int main() {
    printf("Hello from Nios II!\n");

    printf("Initializing VGA control registers...");
    vga_init();
    printf("Done\n");
    int frame = 0;

    while (1) {
        for (int i = 0; i < background_width - SCREEN_WIDTH; i++) {
            vga_regs->frame = frame;
            blitter->frame_address = (frame) ? vga_mem.frame0 : vga_mem.frame1;

            construct_sprite(
                &blitter->sprite,
                background_bitmap,
                background_width,
                background_height,
                0,
                0,
                i,
                0,
                SCREEN_WIDTH + i,
                background_height
            );
            blitter->frame_address = (frame) ? vga_mem.frame0 : vga_mem.frame1;
            blitter->running = 1;
            while (blitter->running);

            construct_sprite(
                &blitter->sprite,
                enemyplanet_bitmap,
                enemyplanet_width,
                enemyplanet_height,
                SCREEN_WIDTH - i * 2,
                (SCREEN_HEIGHT - enemyplanet_height) / 2,
                0,
                0,
                enemyplanet_width,
                enemyplanet_height
            );
            blitter->frame_address = (frame) ? vga_mem.frame0 : vga_mem.frame1;
            blitter->running = 1;
            while (blitter->running);

            construct_sprite(
                &blitter->sprite,
                enemy_ship_bitmap,
                enemy_ship_width,
                enemy_ship_height,
                i * 3,
                (SCREEN_HEIGHT - enemy_ship_height) / 2,
                0,
                0,
                enemy_ship_width,
                enemy_ship_height
            );
            
            blitter->running = 1;
            while (blitter->running);

            frame = !frame;
        }
    }


    /*for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            if (getPixel(vga_mem.frame0, x, y) != 0)
                printf("???\n");
        }
    }*/


    /*int index = allocate_sprite(&background_sprite);
    if (index == -1) {
        printf("BRUHHH");
        return -1;
    }

    gdu_set_frame(vga_mem.frame0);
    gdu_run();
    while (gdu_is_running()) {
        printf("waiting for gdu...\n");
        usleep(10000);
    }*/

    while (1) {
        vga_regs->frame = 0;
        printf("Showing frame 0\n");
        usleep(100000);
        vga_regs->frame = 1;
        printf("Showing frame 1\n");
        usleep(100000);
    }

    return 0;
}
