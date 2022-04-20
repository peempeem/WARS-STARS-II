#include <stdio.h>
#include <stdint.h>

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

    for (int y = (SCREEN_HEIGHT / 2) - 20; y < (SCREEN_HEIGHT / 2) + 20; y++) {
        for (int x = (SCREEN_WIDTH / 2) - 20; x < (SCREEN_WIDTH / 2) + 20; x++) {
            setPixel(vga_mem.frame0, x, y, 0x0FFF);
            setPixel(vga_mem.frame1, x, y, 0x0F0F);
        }
    }

    int offset = 0;
    while (1) {
        for (int i = 100; i < 200; i++) {
            setPixel(vga_mem.frame1, i, offset - 2, 0);
            setPixel(vga_mem.frame1, i, offset, 0x0FFF);
        }
        vga_regs->frame = 1;
        offset++;
        usleep(10000);

        for (int i = 100; i < 200; i++) {
            setPixel(vga_mem.frame0, i, offset - 2, 0);
            setPixel(vga_mem.frame0, i, offset, 0x0FFF);
        }
        vga_regs->frame = 0;
        offset++;
        usleep(10000);
        if (offset > SCREEN_HEIGHT + 10)
            offset = 0;
    }
    return 0;
}
