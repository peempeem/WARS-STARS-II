#include "level1.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"

#include "../sprites/level1_background.h"
#include "../sprites/level1_player_planet.h"
#include "../sprites/level1_enemy_planet.h"
#include "../sprites/cursor.h"

int run_level1() {
    rate_t frame_rate = create_rate(20);
    scene_t scene;
    clear_scene(&scene);

    scene.scroll.max.x  = 3 * (level1_background_sprite.width - SCREEN_WIDTH);
    scene.scroll.max.y  = 0;
    scene.max.x         = 3 * level1_background_sprite.width;
    scene.max.y         = SCREEN_HEIGHT;

    int background      = allocate_object(&scene, BACKGROUND, 1);
    int player_planet   = allocate_object(&scene, BACKGROUND, 1);
    int enemy_planet    = allocate_object(&scene, BACKGROUND, 1);
    int cursor          = allocate_object(&scene, CURSOR, 1);

    scene.objects[background    ].sprite = level1_background_sprite;
    scene.objects[player_planet ].sprite = level1_player_planet_sprite;
    scene.objects[enemy_planet  ].sprite = level1_enemy_planet_sprite;
    scene.objects[cursor        ].sprite = cursor_sprite;

    scene.objects[player_planet].pos.x = 50;
    scene.objects[player_planet].pos.y = (scene.max.y - scene.objects[player_planet].sprite.height) / 2;

    scene.objects[enemy_planet].pos.x = scene.max.x - 50;
    scene.objects[enemy_planet].pos.y = (scene.max.y - scene.objects[enemy_planet].sprite.height) / 2;

    mouse_t mouse = new_mouse();
    mouse.pos.x = SCREEN_WIDTH / 2;
    mouse.pos.y = SCREEN_HEIGHT / 2;

    fade_t fade_in = create_fade(0x0000, 1);

    int running = 1;
    while (running) {
        poll_mouse(&mouse);

        if (is_ready(&frame_rate)) {
            fade_from(&fade_in);
            
            scene.objects[background].sprite.start_x = scene.scroll.pos.x / 3;

            scene.objects[cursor].pos = mouse.pos;

            while (gdu_is_running());
                push_scene(&scene);
                start_render();
        }
    }
}