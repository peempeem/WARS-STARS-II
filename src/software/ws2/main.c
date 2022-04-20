#include <stdio.h>
#include <stdint.h>

typedef struct VGA_MEMORY {
    uint16_t frame0[640 * 480];
    uint16_t frame1[640 * 480];
} vga_memory_t;

typedef struct VGA_REGISTERS {
    vga_memory_t*   address;
    uint32_t        frame;
} vga_regs_t;

vga_memory_t    vga_mem;
vga_regs_t*     vga_regs = (vga_regs_t*) 0x00002158;


int main() {
    printf("Hello from Nios II!\n");

    printf("Initializing VGA control registers...");
    vga_regs->address   = &vga_mem;
    vga_regs->frame     = 0;
    printf("Done\n");

    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            vga_mem.frame0[y * 640 + x] = 0;
        }
    }

    for (int y = 0; y < 240; y++) {
        uint16_t color = 0;
        for (int x = 0; x < 320; x++) {
            vga_mem.frame0[y * 640 + x] = color++;
        }
    }

    for (int y = 240; y < 480; y++) {
        uint16_t color = 0;
        for (int x = 320; x < 640; x++) {
            vga_mem.frame0[y * 640 + x] = color--;
        }
    }

    for (int i = 0; i < 480; i++) {
        vga_mem.frame0[i * 640 + i] = 0x0FFF;
    }

    while (1)
        printf("meme\n");
    return 0;
}
