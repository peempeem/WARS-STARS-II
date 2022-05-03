#ifndef GAME_H
#define GAME_H

#include "gdu.h"
#include "common.h"

#define BACKGROUND  0
#define SHIPS       1
#define EFFECTS     2
#define CURSOR      3

#define BACKGROUND_SPRITES  8
#define SHIP_SPRITES        24
#define EFFECT_SPRITES      31

typedef struct GAME_OBJECT {
    int used,
        visable,
        hp;
    position_t pos;
    sprite_t sprite;
} game_object_t;

typedef struct SCROLL {
    position_t  pos,
                max;
} scroll_t;

typedef union SCENE {
    scroll_t    scroll;
    position_t  max;
    struct TYPED_OBJECTS {
        game_object_t background    [BACKGROUND_SPRITES];
        game_object_t ships         [SHIP_SPRITES];
        game_object_t effects       [EFFECT_SPRITES];
        game_object_t cursor;
    } typed;
    game_object_t objects [MAX_SPRITES];
} scene_t;

int     allocate_object(scene_t* scene, int type, int visable);
void    deallocate_object(game_object_t* object);
void    push_scene(scene_t* scene);
void    clear_scene(scene_t* scene);

#endif