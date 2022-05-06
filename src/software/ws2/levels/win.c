/*
 * win.c
 *
 *  Created on: May 5, 2022
 *      Author: macra
 */

#include "end.h"
#include "../sprites/lightspeed_background.h"
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

	game_object_t* ship                     = allocate_object(&scene, EFFECTS,      USED | VISABLE | CENTERED);
	game_object_t* background               = allocate_object(&scene, BACKGROUND,   USED | VISABLE);
	background->sprite              = lightspeed_background_sprite; 	//to be added, add winner sprite
	ship->sprite                    = title_screen_ship_sprite;


    scene.max.x = SCREEN_WIDTH;
    scene.max.y = SCREEN_HEIGHT;

    fade_t fader = create_fade(0xFF00, FADE_FROM);
        start_fade(&fader, 3);


        int running = 1;
    while(running){

	if (!is_fade_done(&fader))
	    show_fade(&fader);
	float dx = 0;
	    float x = ship->pos.x;

	 dx += 0.35f;
	  x += dx;
	  ship->pos.x = x;

    while (gdu_is_running());
                push_scene(&scene);
                start_render();
    }
}




