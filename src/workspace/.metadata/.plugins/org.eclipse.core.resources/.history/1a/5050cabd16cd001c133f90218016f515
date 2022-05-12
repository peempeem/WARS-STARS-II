/*
 * lose.c
 *
 *  Created on: May 5, 2022
 *      Author: macra
 */

#include "lose.h"
#include "../sprites/loser.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"
#include "../sprites/cursor.h"

void run_lose()	{

    scene_t scene;
    clear_scene(&scene);


    game_object_t* background = allocate_object(&scene, BACKGROUND, USED | VISABLE);
    background->sprite        = loser_sprite;

    scene.max.x = SCREEN_WIDTH;
    scene.max.y = SCREEN_HEIGHT;

    fade_t fader = create_fade(0x0F00, FADE_FROM);
    start_fade(&fader, 3);

    int running = 1;
    while (running) {

        show_fade(&fader);

        while (gdu_is_running());
        push_scene(&scene);
        start_render();

    }
}


