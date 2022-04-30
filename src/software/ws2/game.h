#ifndef GAME_H
#define GAME_H

#include "gdu.h"

#define BACKGROUND  0
#define SHIPS       1
#define EFFECTS     2

#define BACKGROUND_SPRITES  8
#define SHIP_SPRITES        24
#define EFFECT_SPRITES      MAX_SPRITES - BACKGROUND_SPRITES - SHIP_SPRITES

typedef struct GAME_OBJECT {
    uint32_t used;
    uint32_t index;
    uint32_t hp;
    sprite_t sprite;
} game_object_t;

typedef struct SCENE {
    game_object_t objects [MAX_SPRITES];
} scene_t;

int     allocate_sprite(scene_t* scene, int type, game_object_t* object);
void    deallocate_sprite(game_object_t* object);
void    push_scene(scene_t* scene);
void    clear_scene(scene_t* scene);

#endif