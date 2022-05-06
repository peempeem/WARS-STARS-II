#ifndef COMMON_H
#define COMMON_H

#include "gdu.h"

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

void update_physics(physics_t* phys, float dt);
void cap_velocity(physics_t* phys, float max_v);
void slow_down(physics_t* phys, float accel);

#endif