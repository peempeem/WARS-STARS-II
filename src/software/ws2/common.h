#ifndef COMMON_H
#define COMMON_H

#include "gdu.h"
#include "timer.h"

typedef struct POSITION {
    int x;
    int y;
} position_t;

typedef struct FPOSITION {
    float x;
    float y;
} fposition_t;

typedef struct PHYSICS {
    fposition_t p;
    fposition_t v;
    fposition_t a;
} physics_t;

typedef struct GAME_OBJECT {
    int         flags;
    position_t  pos;
    sprite_t    sprite;
} game_object_t;

typedef struct WEAPON_DATA {
    const int damage;
    const float change_rate;
    const float accel;
    const float timeout;
    const int num_sprites;
    const sprite_t* sprites[6];
} weapon_data_t;

typedef struct SHIP {
    game_object_t* ptr;
    int hp;
    float range;
    float accel;
    float max_v;
    int hitradius;
    position_t fire_pos;
    physics_t physics;
    const sprite_t* sprite;
    const weapon_data_t* weapon;
    float firerate;
    rate_t firerate_data;
} ship_t;

typedef struct WEAPON {
    game_object_t* ptr;
    weapon_data_t* data;
    ship_t* target;
    physics_t physics;
    float timeout;
    rate_t change_rate;
} weapon_t;

void update_physics(physics_t* phys, float dt);
void cap_velocity(physics_t* phys, float max_v);
void slow_down(physics_t* phys, float accel);

#endif