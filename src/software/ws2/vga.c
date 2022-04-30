#include "vga.h"

#define VGA_MEM_LOCATION 0x00002000

typedef struct VGA_MEMORY {
    uint16_t frame0[SCREEN_SIZE];
    uint16_t frame1[SCREEN_SIZE];
} vga_memory_t;

typedef struct VGA_REGISTERS {
    vga_memory_t*   address;
    uint32_t        frame;
    uint32_t        blend_color;
    uint32_t        blend_bias;
    uint32_t        vblank;
} vga_regs_t;

static vga_memory_t vga_mem;
static volatile vga_regs_t*  vga_regs = (vga_regs_t*) VGA_MEM_LOCATION;

void vga_init() {
    vga_regs->address       = &vga_mem;
    vga_regs->frame         = 0;
    vga_regs->blend_bias    = 0;
}

void set_frame(uint32_t frame) {
    if (frame < 2)
        vga_regs->frame = frame;
}

uint16_t* get_frame(uint32_t frame) {
    switch (frame) {
        case 0:
            return vga_mem.frame0;
        case 1:
            return vga_mem.frame1;
    }
    return 0;
}

int is_vblank() { return vga_regs->vblank; }

void set_aura(uint16_t color, uint32_t bias) {
    vga_regs->blend_color   = (uint32_t) color;
    if (bias > 64)
        bias = 64;
    vga_regs->blend_bias    = bias;
}

void setPixel(uint16_t* frame, uint32_t x, uint32_t y, uint16_t color) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        frame[y * SCREEN_WIDTH + x] = color;
}

uint16_t getPixel(uint16_t* frame, uint32_t x, uint32_t y) {
    if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
        return frame[y * SCREEN_WIDTH + x];
    return 0;
}
