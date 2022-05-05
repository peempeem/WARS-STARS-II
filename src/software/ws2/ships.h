#ifndef SHIPS_H
#define SHIPS_H

#include "sprites/playership0.h"
#include "sprites/playership1.h"

typedef struct SHIP {
    int             hp,
                    speed,
                    range;
    float           firerate;
    const sprite_t* sprite;
} ship_t;

const static ship_t player_fighter = {
    100,
    3,
	75,
    0.5f,
    &playership0_sprite
};

const static ship_t player_cruiser = {
    100,
    1,
	200,
    2.0f,
    &playership1_sprite
};

#endif
