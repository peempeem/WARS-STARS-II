#ifndef WEAPONS_H
#define WEAPONS_H

#include "sprites/weapon1_0.h"
#include "sprites/weapon1_1.h"
#include "sprites/weapon1_2.h"
#include "sprites/weapon1_3.h"

#include "timer.h"

typedef struct WEAPON_DATA {
    const int damage;
    const int hitradius;
    const float change_rate;
    const float accel;
    const int num_sprites;
    const sprite_t* sprites[6];
} weapon_data_t;

typedef struct WEAPON {
    weapon_data_t* data;
    game_object_t* target;
    physics_t physics;
    rate_t change_rate;
} weapon_t;

const static weapon_data_t missle_data = {
    130,
    3,
    4.0f,
    4.0f,
    4,
    {
        &weapon1_0_sprite,
        &weapon1_1_sprite,
        &weapon1_2_sprite,
        &weapon1_3_sprite,
        0,
        0
    }
};


#endif
