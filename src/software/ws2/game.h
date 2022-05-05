#ifndef GAME_H
#define GAME_H

#include "gdu.h"
#include "common.h"
#include "ships.h"

#define BACKGROUND  0
#define SHIPS       1
#define EFFECTS     2
#define CURSOR      3

#define BACKGROUND_SPRITES  8
#define SHIP_SPRITES        24
#define EFFECT_SPRITES      31

#define USER_SHIPS SHIP_SPRITES / 2

#define PLAYER  0
#define ENEMY   1

#define USED        0x01
#define VISABLE     0x02
#define SCROLL      0x04
#define CENTERED    0x08

typedef struct GAME_OBJECT {
    int         flags;
    position_t  pos;
    sprite_t    sprite;
} game_object_t;

typedef struct SHIP_DATA {
    int     index,
            hp,
            speed;
    float   last_fire,
            firerate;
} ship_data_t;

typedef struct SCENE {
    struct {
        position_t  pos,
                    max;
    } scroll;
    position_t  max;
    struct {
        ship_data_t player  [USER_SHIPS];
        ship_data_t enemy   [USER_SHIPS];
    } ships;
    union {
        struct {
            game_object_t background    [BACKGROUND_SPRITES];
            game_object_t ships         [SHIP_SPRITES];
            game_object_t effects       [EFFECT_SPRITES];
            game_object_t cursor;
        } typed;
        game_object_t untyped [MAX_SPRITES];
    } objects;
} scene_t;

int     allocate_object(scene_t* scene, int type, int flags);
void    deallocate_object(scene_t* scene, uint32_t object);
void    push_scene(scene_t* scene);
void    clear_scene(scene_t* scene);

int     spawn_ship(scene_t* scene, const ship_t* ship, int user, position_t pos);
void    destroy_ship(scene_t* scene, int user, uint32_t ship);

void    update_game(scene_t* scene);

#endif