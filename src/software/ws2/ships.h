#ifndef SHIPS_H
#define SHIPS_H

#include "sprites/playership0.h"
#include "sprites/playership1.h"
#include "sprites/enemyship0.h"
#include "sprites/enemyship1.h"
#include "weapons.h"
#include "common.h"

typedef struct SHIP {
    game_object_t* ptr;
    int hp;
    float range;
    float accel;
    float max_v;
    position_t fire_pos;
    physics_t physics;
    const sprite_t* sprite;
    const weapon_data_t* weapon;
} ship_t;

const static ship_t player_fighter = {
    0,                          // ptr
    100,                        // hp
    150.0f,                     // range
    48.0f,                      // accel
    48.0f,                      // max_v
    {0, 0},                     // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &playership0_sprite,        // sprite
    &missle_data                // weapon
};

const static ship_t player_cruiser = {
    0,                          // ptr
    100,                        // hp
    75.0f,                      // range
    28.0f,                      // accel
    28.0f,                      // max_v
    {0, 0},                     // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &playership1_sprite,        // sprite
    &missle_data                // weapon
};

const static ship_t enemy_fighter = {
    0,                          // ptr
    100,                        // hp
    75.0f,                      // range
    16.0f,                      // accel
    32.0f,                      // max_v
    {0, 0},                     // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &enemyship0_sprite,         // sprite
    &missle_data                // weapon
};

const static ship_t enemy_cruiser = {
    0,                          // ptr
    100,                        // hp
    75.0f,                      // range
    8.0f,                       // accel
    16.0f,                      // max_v
    {0, 0},                     // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &enemyship1_sprite,         // sprite
    &missle_data                // weapon
};

#endif
