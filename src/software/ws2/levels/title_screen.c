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
    
    game_object_t* background               = allocate_object(&scene, BACKGROUND,   USED | VISABLE);
    game_object_t* ship                     = allocate_object(&scene, EFFECTS,      USED | VISABLE | CENTERED);
    game_object_t* title_text               = allocate_object(&scene, EFFECTS,      USED | VISABLE);
    game_object_t* start_button_unpressed   = allocate_object(&scene, SHIPS,        USED | VISABLE);
    game_object_t* start_button_pressed     = allocate_object(&scene, SHIPS,        USED);
    game_object_t* cursor                   = allocate_object(&scene, CURSOR,       USED | VISABLE);
    
    background->sprite              = lightspeed_background_sprite;
    ship->sprite                    = title_screen_ship_sprite;
    title_text->sprite              = WAR_STARS_II_sprite;
    start_button_unpressed->sprite  = start_button_unpressed_sprite;
    start_button_pressed ->sprite   = start_button_pressed_sprite;
    cursor->sprite                  = cursor_sprite;

    ship->pos.x = 100;
    ship->pos.y = scene.max.y / 2;

    title_text->pos.x = (scene.max.x - title_text->sprite.width) / 2;
    title_text->pos.y = 100;

    start_button_unpressed->pos.x = 3 * scene.max.x / 4 - start_button_unpressed->sprite.width / 2;
    start_button_unpressed->pos.y = scene.max.y - start_button_unpressed->sprite.height - 50;

    start_button_pressed->pos = start_button_unpressed->pos;

    float dx = 0;
    float x = ship->pos.x;

    int start_fading = 0;
    int fading = 0;
    int exiting = 0;

    fade_t fade = create_fade(0x0000, FADE_TO);

    mouse_t mouse = new_mouse(scene.max.x / 2, scene.max.y / 2);

    int running = 1;
    while (running) {
        poll_mouse(&mouse, 0, 1);

        if (is_ready(&frame_rate)) {
            handle_mouse(&mouse, &scene, 0, 0); 
            cursor->pos = mouse.pos;

            background->sprite.start_x += 2;
            if (background->sprite.start_x >= background->sprite.width - SCREEN_WIDTH)
                background->sprite.start_x = 0;
            background->sprite.end_x = background->sprite.start_x + SCREEN_WIDTH;

            if (is_clicked(&mouse, MOUSE_BUTTON_LEFT)) {
                if (mouse_over_object(&scene, &mouse, start_button_unpressed)) {
                    start_button_unpressed->flags &= ~VISABLE;
                    start_button_pressed ->flags |= VISABLE;
                    exiting = 1;
                }
            }

            if (exiting) {
                dx += 0.35f;
                x += dx;
                ship->pos.x = x;
            }

            fading = ship->pos.x + ship->sprite.width > scene.max.x;

            if (fading) {
                if (!start_fading) {
                    start_fade(&fade, 1);
                    start_fading = 1;
                }
                running = !show_fade(&fade);
            }
            
            while (gdu_is_running());
            push_scene(&scene);
            start_render();
        }
    }
}
