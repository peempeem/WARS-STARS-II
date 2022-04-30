#include "game.h"

int allocate_object(scene_t* scene, int type, game_object_t* object) {
    switch (type) {
        case BACKGROUND:
            for (int i = 0; i < BACKGROUND_SPRITES; i++) {
                if (!scene->objects[i].used) {
                    scene->objects[i]       = *object;
                    scene->objects[i].used  = 1;
                    scene->objects[i].index = i;
                    return 1;
                }
            }
            break;
        
        case SHIPS:
            for (int i = BACKGROUND_SPRITES; i < SHIP_SPRITES; i++) {
                if (!scene->objects[i].used) {
                    scene->objects[i]       = *object;
                    scene->objects[i].used  = 1;
                    scene->objects[i].index = i;
                    return 1;
                }
            }
            break;
        
        case EFFECTS:
            for (int i = SHIP_SPRITES; i < EFFECT_SPRITES; i++) {
                if (!scene->objects[i].used) {
                    scene->objects[i]       = *object;
                    scene->objects[i].used  = 1;
                    scene->objects[i].index = i;
                    return 1;
                }
            }
            break;
    }
    return -1;
}

void deallocate_object(game_object_t* object) { object->used = 0; }

void push_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (scene->objects->used)
            push_sprite(&scene->objects[i].sprite, i);
        else
            pop_sprite(i);
    }
}

void clear_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++)
        scene->objects->used = 0;
}
