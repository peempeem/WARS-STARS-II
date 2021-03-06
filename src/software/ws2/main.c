#include <stdio.h>
#include "mouse.h"
#include "vga.h"
#include "gdu.h"
#include "timer.h"
#include "display.h"

#include "levels/title_screen.h"
#include "levels/level1.h"
#include "levels/win.h"
#include "levels/lose.h"
#include "sprites/mouse.h"

int main() {
    printf("Running War Stars 2: Defense of the Homes!\n");
    disable_master_timer();
    usleep(1000);
    enable_master_timer();
    
    vga_init();
    gdu_reset();

    sprite_t mouse_s = mouse_sprite;
    mouse_s.screen_x = (SCREEN_WIDTH - mouse_s.width) / 2;
    mouse_s.screen_y = (SCREEN_HEIGHT - mouse_s.height) / 2;
    push_sprite(&mouse_s, 0);
    double_render();
    pop_sprite(0);
    
    init_mouse();
    mouse_t mouse;
    while (!poll_mouse(&mouse, 0, 0));
    
    run_title_screen();
    if (run_level1()) {
        printf("you lost\n");
        run_lose();
    } else {
        printf("you won\n");
        run_win();
    }	    
    
    printf("Done!\n");
    return 0;
}
