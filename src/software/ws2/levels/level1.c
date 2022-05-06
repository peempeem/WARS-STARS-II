#include "level1.h"
#include "../mouse.h"
#include "../timer.h"
#include "../game.h"
#include "../display.h"
#include "../vga.h"

#include "../sprites/level1_background.h"
#include "../sprites/level1_player_planet.h"
#include "../sprites/level1_enemy_planet.h"
#include "../sprites/statusbar.h"
#include "../sprites/greenhealth.h"
#include "../sprites/redhealth.h"
#include "../sprites/playership0.h"
#include "../sprites/playership1.h"
#include "../sprites/warning.h"
#include "../sprites/cursor.h"
#include "../ships.h"

#include <stdlib.h>

#define SPAWN_CUTOFF 250

int run_level1() {
    rate_t frame_rate = create_rate(20);
    scene_t scene;
    clear_scene(&scene);

    scene.scroll.max.x  = 3 * (level1_background_sprite.width - SCREEN_WIDTH);
    scene.max.x         = scene.scroll.max.x + SCREEN_WIDTH;
    scene.max.y         = SCREEN_HEIGHT;

    game_object_t* background       = allocate_object(&scene, BACKGROUND,   USED | VISABLE);
    game_object_t* player_planet    = allocate_object(&scene, BACKGROUND,   USED | VISABLE | SCROLL | CENTERED);
    game_object_t* enemy_planet     = allocate_object(&scene, BACKGROUND,   USED | VISABLE | SCROLL | CENTERED);
    game_object_t* statusbar        = allocate_object(&scene, EFFECTS,      USED | VISABLE);
    game_object_t* playerbar        = allocate_object(&scene, EFFECTS,      USED | VISABLE);
    game_object_t* enemybar         = allocate_object(&scene, EFFECTS,      USED | VISABLE);
    game_object_t* spawnship1       = allocate_object(&scene, EFFECTS,      USED | VISABLE | CENTERED);
    game_object_t* spawnship2       = allocate_object(&scene, EFFECTS,      USED | VISABLE | CENTERED);
    game_object_t* spawnwarning     = allocate_object(&scene, EFFECTS,      USED | CENTERED);
    game_object_t* cursor           = allocate_object(&scene, CURSOR,       USED | VISABLE);

    background->sprite      = level1_background_sprite;
    player_planet->sprite   = level1_player_planet_sprite;
    enemy_planet->sprite    = level1_enemy_planet_sprite;
    statusbar->sprite       = statusbar_sprite;
    playerbar->sprite       = greenhealth_sprite;
    enemybar->sprite        = redhealth_sprite;
    spawnship1->sprite      = playership0_sprite;
    spawnship2->sprite      = playership1_sprite;
    spawnwarning->sprite    = warning_sprite;
    cursor->sprite          = cursor_sprite;

    player_planet->pos.x = 50 + player_planet->sprite.width / 2;
    player_planet->pos.y = scene.max.y / 2;

    enemy_planet->pos.x = scene.max.x - 50 - enemy_planet->sprite.width / 2;
    enemy_planet->pos.y = scene.max.y / 2;

    statusbar->pos.y = SCREEN_HEIGHT - statusbar->sprite.height;

    playerbar->pos.x = 36;
    playerbar->pos.y = SCREEN_HEIGHT - 46;
    enemybar->pos.x = 450;
    enemybar->pos.y = SCREEN_HEIGHT - 46;
    

    position_t spawnship1_default = statusbar->pos;
    spawnship1_default.x += 270;
    spawnship1_default.y += 30;
    spawnship1->pos = spawnship1_default;

    position_t spawnship2_default = spawnship1_default;
    spawnship2_default.x = 370;
    spawnship2->pos = spawnship2_default;

    mouse_t mouse = new_mouse(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    fade_t fade = create_fade(0x0000, FADE_FROM);
    start_fade(&fade, 1);

    spawn_ship(&scene, &enemy_cruiser, ENEMY, enemy_planet->pos);

    fade_t fader = create_fade(0xFFFF, FADE_TO);
    fade_t fadew = create_fade(0xFF00, FADE_TO);

    int player_health = 1000;
    int enemy_health = 1000;    

    int lose = 0, win = 0;				//fade out final screens
    int start_fading = 0;

    int placing_ship = 0;
    
    int running = 1;
    while (running) {
        poll_mouse(&mouse, 0, 1);

        if (is_ready(&frame_rate)) {
            handle_mouse(&mouse, &scene, 1, 0); 
            cursor->pos = mouse.pos;

            if (!is_fade_done(&fade))
                show_fade(&fade);
            
            background->sprite.start_x = scene.scroll.pos.x / 3;
            background->sprite.end_x = background->sprite.start_x + SCREEN_WIDTH;

            playerbar->sprite.end_x = (playerbar->sprite.width * player_health) / 1000;
            enemybar->sprite.end_x = (enemybar->sprite.width * enemy_health) / 1000;

            position_t place_pos = mouse_to_game(&scene, &mouse);
            if (place_pos.x > SPAWN_CUTOFF)
                place_pos.x = SPAWN_CUTOFF;

            if (is_clicked(&mouse, MOUSE_BUTTON_LEFT)) {
                switch (placing_ship) {
                    case 0:
                        if (mouse_over_object(&scene, &mouse, spawnship1))
                            placing_ship = 1;
                        else if (mouse_over_object(&scene, &mouse, spawnship2))
                            placing_ship = 2;
                        break;
                    case 1:
                        spawn_ship(&scene, &player_fighter, PLAYER, place_pos);
                        spawnship1->pos = spawnship1_default;
                        placing_ship = 0;
                        scene.shipcountf++;
                        break;
                    case 2:
                        spawn_ship(&scene, &player_cruiser, PLAYER, place_pos);
                        spawnship2->pos = spawnship2_default;
                        placing_ship = 0;
                        scene.shipcountc++;
                        break;
                }
            }

            position_t warn_pos = place_pos;
            warn_pos.x -= scene.scroll.pos.x;
            if (warn_pos.x < spawnwarning->sprite.width / 2)
                warn_pos.x = spawnwarning->sprite.width / 2;

            if (placing_ship && warn_pos.x >= SPAWN_CUTOFF + scene.scroll.pos.x) {
                spawnwarning->flags |= VISABLE;
                spawnwarning->pos   = warn_pos;
            } else
                spawnwarning->flags &= ~VISABLE;

            switch (placing_ship) {
                case 1:
                    spawnship1->pos = mouse.pos;
                    break;
                case 2:
                    spawnship2->pos = mouse.pos;
                    break;
            }

            int r = rand() % 7;
            int yr = ((rand() % 390)+15);

            if ((scene.eshipcount <= (3 + scene.shipcountc + scene.shipcountf)) || enemy_health <= 150) {
                position_t epos;
                epos.y = yr;
                epos.x = 1200;

                switch(r)  {
                    case 0:
                        spawn_ship(&scene, &enemy_cruiser, ENEMY, epos);
                        scene.eshipcount++;
                        break;

                    case 1:
                        spawn_ship(&scene, &enemy_fighter, ENEMY, epos);
                        scene.eshipcount++;
                        break;

                    case 2:
                    break;

                    case 3:
                        spawn_ship(&scene, &enemy_fighter, ENEMY, epos);
                        spawn_ship(&scene, &enemy_cruiser, ENEMY, epos);
                        scene.eshipcount += 2;
                        break;
                    case 4:
                    	epos.y += 12;
                    	spawn_ship(&scene, &enemy_cruiser, ENEMY, epos );
                    	epos.y -= 24;
                    	spawn_ship(&scene, &enemy_cruiser, ENEMY, epos );
                    	break;
                    case 5:
                    	epos.y += 8;
                    	spawn_ship(&scene, &enemy_fighter, ENEMY, epos);
                    	epos.y -= 16;
                    	spawn_ship(&scene, &enemy_fighter, ENEMY, epos);
                    	break;
                    case 6:
                    break;
                }
            }


            if(enemy_health <= 0){
            	if (!start_fading) {
            	start_fade(&fader, 3);
            	start_fading = 1;
            	lose = 1;


            	}
            	running =  !show_fade(&fader);
            }


            if(player_health <= 0){
            	if (!start_fading) {
            	start_fade(&fadew, 3);
            	start_fading = 1;
            	win= 1;


                        }
            	running = !show_fade(&fadew);
            }



            if (!start_fading) {
            	update_game(&scene);

				while (gdu_is_running());
				push_scene(&scene);
				start_render();
            }

        }
    }


    if (win)
    return 0;
    else
    return 1;
}
