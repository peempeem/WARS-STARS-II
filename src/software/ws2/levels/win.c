/*
 * win.c
 *
 *  Created on: May 5, 2022
 *      Author: macra
 */

#include "../sprites/win.h"
#include "../sprites/title_screen_ship.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"
#include "../sprites/cursor.h"

void run_win() {

    scene_t scene;
    clear_scene(&scene);

    game_object_t* ship                     = allocate_object(&scene, EFFECTS,      USED | VISABLE);
    game_object_t* background               = allocate_object(&scene, BACKGROUND,   USED | VISABLE);
    background->sprite              = win_sprite; 	//to be added, add winner sprite
    ship->sprite                    = title_screen_ship_sprite;


    scene.max.x = SCREEN_WIDTH;
    scene.max.y = SCREEN_HEIGHT;

    fade_t fader = create_fade(0x0FFF, FADE_FROM);
    start_fade(&fader, 3);

    ship->pos.x = 0;
    ship->pos.y = scene.max.y - 200;

    float dx = 0;
    float x = ship->pos.x;

    int running = 1;
    while (running) {
        show_fade(&fader);

        dx += 0.35f;
        x += dx;
        ship->pos.x = x;

        if (x > scene.max.x)
            running = 0;

        while (gdu_is_running());
        push_scene(&scene);
        start_render();
    }
}




