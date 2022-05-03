#include "title_screen.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"

#include "../sprites/lightspeed_background.h"
#include "../sprites/title_screen_ship.h"
#include "../sprites/WAR_STARS_II.h"
#include "../sprites/start_button_unpressed.h"
#include "../sprites/start_button_pressed.h"
#include "../sprites/cursor.h"

void run_title_screen() {
    rate_t frame_rate = create_rate(20);
    scene_t scene;
    clear_scene(&scene);

    scene.scroll.max.x  = 0;
    scene.scroll.max.y  = 0;
    scene.max.x         = SCREEN_WIDTH;
    scene.max.y         = SCREEN_HEIGHT;
    
    int background              = allocate_object(&scene, BACKGROUND, 1);
    int ship                    = allocate_object(&scene, EFFECTS, 1);
    int title_text              = allocate_object(&scene, EFFECTS, 1);
    int start_button_unpressed  = allocate_object(&scene, SHIPS, 1);
    int start_button_pressed    = allocate_object(&scene, SHIPS, 0);
    int cursor                  = allocate_object(&scene, CURSOR, 1);
    
    scene.objects[background            ].sprite = lightspeed_background_sprite;
    scene.objects[ship                  ].sprite = title_screen_ship_sprite;
    scene.objects[title_text            ].sprite = WAR_STARS_II_sprite;
    scene.objects[start_button_unpressed].sprite = start_button_unpressed_sprite;
    scene.objects[start_button_pressed  ].sprite = start_button_pressed_sprite;
    scene.objects[cursor                ].sprite = cursor_sprite;

    scene.objects[ship].pos.x = ((scene.max.x / 2) - scene.objects[ship].sprite.width) / 2;
    scene.objects[ship].pos.y = scene.max.y / 2;

    scene.objects[title_text].sprite.screen_x = (scene.max.x - scene.objects[title_text].sprite.width) / 2;
    scene.objects[title_text].sprite.screen_y = 100;

    scene.objects[start_button_unpressed].sprite.screen_x = SCREEN_WIDTH - SCREEN_WIDTH / 4 - scene.objects[start_button_unpressed].sprite.width / 2;
    scene.objects[start_button_unpressed].sprite.screen_y = SCREEN_HEIGHT - 50 - scene.objects[start_button_unpressed].sprite.height;

    scene.objects[start_button_pressed].sprite.screen_x = scene.objects[start_button_unpressed].sprite.screen_x;
    scene.objects[start_button_pressed].sprite.screen_y = scene.objects[start_button_unpressed].sprite.screen_y;

    float dx = 0;
    float x = scene.objects[ship].sprite.screen_x;

    int start_fading = 0;
    int fading = 0;
    int exiting = 0;

    fade_t fade_out;

    int running = 1;

    mouse_t mouse       = new_mouse();
    mouse_t last_mouse  = new_mouse();
    mouse_t game_mouse  = new_mouse();
    game_mouse.x = SCREEN_WIDTH / 2;
    game_mouse.y = SCREEN_HEIGHT / 2;

    while (running) {
        poll_mouse(&mouse);
        
        if (is_ready(&frame_rate)) {
            scene.objects[background].sprite.start_x += 2;
            if (scene.objects[background].sprite.start_x >= scene.objects[background].sprite.width - SCREEN_WIDTH)
                scene.objects[background].sprite.start_x = 0;
            scene.objects[background].sprite.end_x = scene.objects[background].sprite.start_x + SCREEN_WIDTH;

            game_mouse.x += mouse.x - last_mouse.x;
            game_mouse.y -= mouse.y - last_mouse.y;
            last_mouse = mouse;

            if (game_mouse.x < 0)
                game_mouse.x = 0;
            else if (game_mouse.x > SCREEN_WIDTH)
                game_mouse.x = SCREEN_WIDTH;
            if (game_mouse.y < 0)
                game_mouse.y = 0;
            else if (game_mouse.y > SCREEN_HEIGHT)
                game_mouse.y = SCREEN_HEIGHT;    

            scene.objects[cursor].sprite.screen_x = game_mouse.x;
            scene.objects[cursor].sprite.screen_y = game_mouse.y;

            if (mouse.left_click) {
                if (is_clicked(&game_mouse, &scene.objects[start_button_unpressed])) {
                    scene.objects[start_button_unpressed].visable = 0;
                    scene.objects[start_button_pressed  ].visable = 1;
                    exiting = 1;
                }
            }

            if (exiting) {
                scene.objects[start_button_unpressed].visable   = 0;
                scene.objects[start_button_pressed].visable     = 1;

                dx += 0.35f;
                x += dx;
                scene.objects[ship].sprite.screen_x = (uint32_t) x;
            }

            fading = scene.objects[ship].sprite.screen_x > SCREEN_WIDTH;

            if (!start_fading && fading) {
                fade_out = create_fade(0x0000, 2);
                start_fading = 1;
            }

            if (fading)
                running = !fade_to(&fade_out);
            
            while (gdu_is_running());
            push_scene(&scene);
            start_render();
        }
    }
}
