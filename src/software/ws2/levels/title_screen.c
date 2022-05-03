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

    scene.max.x = SCREEN_WIDTH;
    scene.max.y = SCREEN_HEIGHT;
    
    int background              = allocate_object(&scene, BACKGROUND, 1, 0);
    int ship                    = allocate_object(&scene, EFFECTS, 1, 0);
    int title_text              = allocate_object(&scene, EFFECTS, 1, 0);
    int start_button_unpressed  = allocate_object(&scene, SHIPS, 1, 0);
    int start_button_pressed    = allocate_object(&scene, SHIPS, 0, 0);
    int cursor                  = allocate_object(&scene, CURSOR, 1, 0);
    
    scene.objects.untyped[background            ].sprite = lightspeed_background_sprite;
    scene.objects.untyped[ship                  ].sprite = title_screen_ship_sprite;
    scene.objects.untyped[title_text            ].sprite = WAR_STARS_II_sprite;
    scene.objects.untyped[start_button_unpressed].sprite = start_button_unpressed_sprite;
    scene.objects.untyped[start_button_pressed  ].sprite = start_button_pressed_sprite;
    scene.objects.untyped[cursor                ].sprite = cursor_sprite;

    scene.objects.untyped[ship].pos.x = 50;
    scene.objects.untyped[ship].pos.y = (scene.max.y - scene.objects.untyped[ship].sprite.height) / 2;

    scene.objects.untyped[title_text].pos.x = (scene.max.x - scene.objects.untyped[title_text].sprite.width) / 2;
    scene.objects.untyped[title_text].pos.y = 100;

    scene.objects.untyped[start_button_unpressed].pos.x = 3 * scene.max.x / 4 - scene.objects.untyped[start_button_unpressed].sprite.width / 2;
    scene.objects.untyped[start_button_unpressed].pos.y = scene.max.y - scene.objects.untyped[start_button_unpressed].sprite.height - 50;

    scene.objects.untyped[start_button_pressed].pos = scene.objects.untyped[start_button_unpressed].pos;

    float dx = 0;
    float x = scene.objects.untyped[ship].pos.x;

    int start_fading = 0;
    int fading = 0;
    int exiting = 0;

    fade_t fade;

    mouse_t mouse = new_mouse(scene.max.x / 2, scene.max.y / 2);

    int running = 1;
    while (running) {
        poll_mouse(&mouse, 0, 1);

        if (is_ready(&frame_rate)) {
            handle_mouse(&mouse, &scene, 0, 0); 
            scene.objects.untyped[cursor].pos = mouse.pos;

            scene.objects.untyped[background].sprite.start_x += 2;
            if (scene.objects.untyped[background].sprite.start_x >= scene.objects.untyped[background].sprite.width - SCREEN_WIDTH)
                scene.objects.untyped[background].sprite.start_x = 0;
            scene.objects.untyped[background].sprite.end_x = scene.objects.untyped[background].sprite.start_x + SCREEN_WIDTH;

            if (mouse.buttons.left) {
                if (is_clicked(&mouse, &scene.objects.untyped[start_button_unpressed])) {
                    scene.objects.untyped[start_button_unpressed].visable = 0;
                    scene.objects.untyped[start_button_pressed  ].visable = 1;
                    exiting = 1;
                }
            }

            if (exiting) {
                dx += 0.35f;
                x += dx;
                scene.objects.untyped[ship].pos.x = x;
            }

            fading = scene.objects.untyped[ship].pos.x > scene.max.x;

            if (fading) {
                if (!start_fading) {
                    fade = create_fade(0x0000, 2);
                    start_fading = 1;
                }
                running = !fade_to(&fade);
            }
            
            while (gdu_is_running());
            push_scene(&scene);
            start_render();
        }
    }
}
