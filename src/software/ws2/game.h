#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "ships.h"

#define BACKGROUND  0
#define SHIPS       1
#define EFFECTS     2
#define CURSOR      3

#define BACKGROUND_SPRITES  8
#define SHIP_SPRITES        24
#define EFFECT_SPRITES      31

#define USER_SHIPS      6
#define USER_WEAPONS    10

#define PLAYER  0
#define ENEMY   1

#define USED        0x01
#define VISABLE     0x02
#define SCROLL      0x04
#define CENTERED    0x08

typedef struct SCENE {
    float last_update;
    int shipcountc;
    int shipcountf;
    int eshipcount;
    ship_t playerplanet;
    ship_t enemyplanet;
    struct {
        position_t  pos,
                    max;
    } scroll;
    position_t  max;
    struct {
        ship_t player   [USER_SHIPS];
        ship_t enemy    [USER_SHIPS];
    } ships;
    struct {
        weapon_t player [USER_WEAPONS];
        weapon_t enemy  [USER_WEAPONS];
    } weapons;
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

game_object_t*  allocate_object(scene_t* scene, int type, int flags);
void            deallocate_object(game_object_t* obj);
void            push_scene(scene_t* scene);
void            clear_scene(scene_t* scene);

ship_t*         spawn_ship(scene_t* scene, const ship_t* spawn, int user, position_t pos);
void            destroy_ship(scene_t* scene, ship_t* ship);

void            update_game(scene_t* scene);

#endif
