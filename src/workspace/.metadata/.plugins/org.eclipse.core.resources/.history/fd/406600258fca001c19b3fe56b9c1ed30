#include "game.h"
#include "vga.h"

int allocate_object(scene_t* scene, int type, int visable) {
    switch (type) {
        case BACKGROUND:
            for (int i = 0; i < BACKGROUND_SPRITES; i++) {
                if (!scene->typed.background[i].used) {
                    scene->typed.background[i].used     = 1;
                    scene->typed.background[i].visable  = visable;
                    return i;
                }
            }
            break;
        
        case SHIPS:
            for (int i = 0; i < SHIP_SPRITES; i++) {
                if (!scene->typed.ships[i].used) {
                    scene->typed.ships[i].used      = 1;
                    scene->typed.ships[i].visable   = visable;
                    return i + BACKGROUND_SPRITES;
                }
            }
            break;
        
        case EFFECTS:
            for (int i = 0 + SHIP_SPRITES; i < EFFECT_SPRITES; i++) {
                if (!scene->typed.effects[i].used) {
                    scene->typed.effects[i].used     = 1;
                    scene->typed.effects[i].visable = visable;
                    return i + BACKGROUND_SPRITES + SHIP_SPRITES;
                }
            }
            break;
        
        case CURSOR:
            if (!scene->typed.cursor.used) {
                scene->typed.cursor.used     = 1;
                scene->typed.cursor.visable = visable;
                return MAX_SPRITES - 1;
            }
            break;
    }
    return -1;
}

void deallocate_object(game_object_t* object) { object->used = 0; }

void push_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        game_object_t* obj = &scene->objects[i];
        int pop = 1;
        
        if (obj->used && obj->visable) {
            obj->sprite.screen_x = obj->pos.x - scene->scroll.x;
            obj->sprite.screen_y = SCREEN_HEIGHT - (obj->pos.y - scene->scroll.y) - 1;

            if (obj->sprite.screen_x >= -obj->sprite.width &&
                obj->sprite.screen_x < SCREEN_WIDTH &&
                obj->sprite.screen_y >= -obj->sprite.height &&
                obj->sprite.screen_y < SCREEN_HEIGHT)
                pop = 0;
        }

        if (pop)
            pop_sprite(i);
        else
            push_sprite(&obj->sprite, i);   
    }
}

void clear_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++)
        scene->objects[i].used = 0;
}
