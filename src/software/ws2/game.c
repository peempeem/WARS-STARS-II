#include "game.h"
#include "vga.h"

void init_object(game_object_t* object, int visable, int scrolling) {
    object->used    = 1;
    object->visable = visable;
    object->scroll  = scrolling;
    object->pos.x   = 0;
    object->pos.y   = 0;
}

int allocate_object(scene_t* scene, int type, int visable, int scrolling) {
    switch (type) {
        case BACKGROUND:
            for (int i = 0; i < BACKGROUND_SPRITES; i++) {
                if (!scene->objects.typed.background[i].used) {
                    init_object(&scene->objects.typed.background[i], visable, scrolling);
                    return i;
                }
            }
            break;
        
        case SHIPS:
            for (int i = 0; i < SHIP_SPRITES; i++) {
                if (!scene->objects.typed.ships[i].used) {
                    init_object(&scene->objects.typed.ships[i], visable, scrolling);
                    return i + BACKGROUND_SPRITES;
                }
            }
            break;
        
        case EFFECTS:
            for (int i = 0 + SHIP_SPRITES; i < EFFECT_SPRITES; i++) {
                if (!scene->objects.typed.effects[i].used) {
                    init_object(&scene->objects.typed.effects[i], visable, scrolling);
                    return i + BACKGROUND_SPRITES + SHIP_SPRITES;
                }
            }
            break;
        
        case CURSOR:
            if (!scene->objects.typed.cursor.used) {
                init_object(&scene->objects.typed.cursor, visable, scrolling);
                return MAX_SPRITES - 1;
            }
            break;
    }
    return -1;
}

void deallocate_object(game_object_t* object) { object->used = 0; }

void push_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        game_object_t* obj = &scene->objects.untyped[i];
        int pop = 1;
        
        if (obj->used && obj->visable) {
            obj->sprite.screen_x = obj->pos.x;
            obj->sprite.screen_y = obj->pos.y;

            if (obj->scroll) {
                obj->sprite.screen_x -= scene->scroll.pos.x;
                obj->sprite.screen_y -= scene->scroll.pos.y;
            }

            if (obj->sprite.screen_x >= -(obj->sprite.end_x - obj->sprite.start_x) &&
                obj->sprite.screen_x < SCREEN_WIDTH &&
                obj->sprite.screen_y >= -(obj->sprite.end_y - obj->sprite.start_y) &&
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
    scene->scroll.pos.x = 0;
    scene->scroll.pos.y = 0;
    scene->scroll.max.x = 0;
    scene->scroll.max.y = 0;
    scene->max.x        = 0;
    scene->max.y        = 0;
    for (int i = 0; i < MAX_SPRITES; i++)
        scene->objects.untyped[i].used = 0;
}
