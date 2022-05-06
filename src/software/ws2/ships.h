#ifndef SHIPS_H
#define SHIPS_H

#include "sprites/playership0.h"
#include "sprites/playership1.h"
#include "sprites/enemyship0.h"
#include "sprites/enemyship1.h"
#include "weapons.h"
#include "common.h"

const static ship_t player_fighter = {
    0,                          // ptr
    100,                        // hp
    125.0f,                     // range
    64.0f,                      // accel
    64.0f,                      // max_v
    20,                         // hitradius
    {8, 0},                     // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &playership0_sprite,        // sprite
    &player_missle_data,        // weapon
    1.5f,                       // firerate
    {0, 0}                      // firerate_data
};

const static ship_t player_cruiser = {
    0,                          // ptr
    200,                        // hp
    300.0f,                     // range
    28.0f,                      // accel
    28.0f,                      // max_v
    40,                         // hitradius
    {16, 0},                    // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &playership1_sprite,        // sprite
    &player_missle_data,        // weapon
    0.75f,                      // firerate
    {0, 0}                      // firerate_data
};

const static ship_t enemy_fighter = {
    0,                          // ptr
    100,                        // hp
    125.0f,                     // range
    -64.0f,                     // accel
    64.0f,                      // max_v
    20,                         // hitradius
    {-8, 0},                    // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &enemyship0_sprite,         // sprite
    &enemy_missle_data,         // weapon
    1.5f,                       // firerate
    {0, 0}                      // firerate_data
};

const static ship_t enemy_cruiser = {
    0,                          // ptr
    200,                        // hp
    300.0f,                     // range
    -28.0f,                     // accel
    28.0f,                      // max_v
    40,                         // hitradius
    {-16, 0},                   // fire_pos
    {                       
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}, 
        {0.0f, 0.0f}
    },                          // physics
    &enemyship1_sprite,         // sprite
    &enemy_missle_data,         // weapon
    0.75f,                      // firerate
    {0, 0}                      // firerate_data
};

#endif
