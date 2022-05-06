#ifndef WEAPONS_H
#define WEAPONS_H

#include "sprites/weapon1_0.h"
#include "sprites/weapon2_0.h"

#include "common.h"

const static weapon_data_t player_missle_data = {
    50,
    4.0f,
    4.0f,
    5.0f,
    1,
    {
        &weapon1_0_sprite,
        0,
        0,
        0,
        0,
        0
    }
};

const static weapon_data_t enemy_missle_data = {
    50,
    4.0f,
    4.0f,
    5.0f,
    1,
    {
        &weapon2_0_sprite,
        0,
        0,
        0,
        0,
        0
    }
};

#endif
