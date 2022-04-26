#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_SIZE     640 * 480

#define VGA_MEM_LOCATION    0x00002000

typedef struct VGA_MEMORY {
    uint16_t frame0[SCREEN_SIZE];
    uint16_t frame1[SCREEN_SIZE];
} vga_memory_t;

typedef struct VGA_REGISTERS {
    vga_memory_t*   address;
    uint32_t        frame;
} vga_regs_t;

static vga_memory_t vga_mem;
static vga_regs_t*  vga_regs = (vga_regs_t*) VGA_MEM_LOCATION;

void vga_init();
void setPixel(uint16_t* frame, uint32_t x, uint32_t y, uint16_t color);
uint16_t getPixel(uint16_t* frame, uint32_t x, uint32_t y);

#endif
