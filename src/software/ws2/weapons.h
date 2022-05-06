#ifndef WEAPONS_H
#define WEAPONS_H

#include "sprites/weapon1_0.h"
#include "sprites/weapon1_1.h"
#include "sprites/weapon1_2.h"
#include "sprites/weapon1_3.h"

#include "common.h"

const static weapon_data_t missle_data = {
    50,
    4.0f,
    4.0f,
    5.0f,
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
