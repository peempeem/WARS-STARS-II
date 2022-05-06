/*
 * win.c
 *
 *  Created on: May 5, 2022
 *      Author: macra
 */

#include "../sprites/lightspeed_background.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"
#include "../sprites/cursor.h"

void run_win()	{

	rate_t frame_rate = create_rate(20);
	scene_t scene;
	clear_scene(&scene);


	game_object_t* background               = allocate_object(&scene, BACKGROUND,   USED | VISABLE);
	background->sprite              = lightspeed_background_sprite; 	//to be added, add winner sprite

	scene.max.x = SCREEN_WIDTH;
	scene.max.y = SCREEN_HEIGHT;

	fade_t fader = create_fade(0xFF00, FADE_FROM);
	    start_fade(&fader, 3);

	    mouse_t mouse = new_mouse(scene.max.x / 2, scene.max.y / 2);

	    int running = 1;
	while(running){

	if (!is_fade_done(&fader))
	    show_fade(&fader);


	while (gdu_is_running());
	            push_scene(&scene);
	            start_render();
	}
}




