#include <stdio.h>

#include "vga.h"
#include "gdu.h"
#include "timer.h"

#include "sprites/lightspeed_background.h"
#include "sprites/title_screen_ship.h"
#include "sprites/WAR_STARS_II.h"
#include "sprites/start_button_unpressed.h"

int screen_x = 0;

void start_render() {
    static int frame = 0;
    while (!is_vblank());
    set_frame(frame);
    gdu_set_frame(get_frame(!frame));
    gdu_run();
    frame = !frame;
}

int main() {
    printf("Running WarStars 2: Defense of the Homes!\n");
    enable_master_timer();
    vga_init();
    gdu_reset();

    rate_t frame_rate = create_rate(20);
    
    sprite_t background = lightspeed_background_sprite;
    sprite_t playership = title_screen_ship_sprite;
    sprite_t title_text = WAR_STARS_II_sprite;
    sprite_t start_button = start_button_unpressed_sprite;

    playership.screen_x = ((SCREEN_WIDTH / 2) - playership.width) / 2;
    playership.screen_y = (SCREEN_HEIGHT - playership.height) / 2;

    title_text.screen_x = (SCREEN_WIDTH - title_text.width) / 2;
    title_text.screen_y = 100;

    start_button.screen_x = SCREEN_WIDTH - SCREEN_WIDTH / 4 - start_button.width / 2;
    start_button.screen_y = (SCREEN_HEIGHT - start_button.height) / 2;

    int count = 0;
    float dx = 0.0f;
    float x = playership.screen_x;

    float fade_start;

    while (1) {
        float start = get_time();

        background.start_x = screen_x;
        background.end_x = SCREEN_WIDTH / 2 + screen_x;
        if (count >= 400) {
            int offset = count - 400;
            if (offset >= SCREEN_WIDTH / 2)
                offset = SCREEN_WIDTH / 2;
            background.end_x += offset;
        }

        if (count >= 600) {
            dx += 0.2f;
            x += dx;
            playership.screen_x = (uint32_t) x;
        }

        if (count == 900)
            fade_start = get_time();

        if (count >= 900)
            set_aura(0x0000, (uint32_t) (64 * ((get_time() - fade_start))));

        screen_x += 10;
        if (screen_x >= background.width - SCREEN_WIDTH)
            screen_x = 0;
        count += 4;

        while (gdu_is_running());

        push_sprite(&background, 0);
        push_sprite(&playership, 1);
        push_sprite(&title_text, 2);
        push_sprite(&start_button, 3);

        while (!is_ready(&frame_rate));

        start_render();
        
        printf("fps: %f\n", 1 / (get_time() - start));
    }
    return 0;
}
