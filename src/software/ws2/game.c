#include "game.h"
#include "vga.h"
#include <string.h>
#include <math.h>

void init_object(game_object_t* object, int flags) {
    memset(object, 0, sizeof(game_object_t));
    object->flags = flags;
}

game_object_t* allocate_object(scene_t* scene, int type, int flags) {
    switch (type) {
        case BACKGROUND:
            for (int i = 0; i < BACKGROUND_SPRITES; i++) {
                game_object_t* obj = &scene->objects.typed.background[i];
                if (!(obj->flags & USED)) {
                    init_object(obj, flags);
                    return obj;
                }
            }
            break;
        
        case SHIPS:
            for (int i = 0; i < SHIP_SPRITES; i++) {
                game_object_t* obj = &scene->objects.typed.ships[i];
                if (!(obj->flags & USED)) {
                    init_object(obj, flags);
                    return obj;
                }
            }
            break;
        
        case EFFECTS:
            for (int i = 0; i < EFFECT_SPRITES; i++) {
                game_object_t* obj = &scene->objects.typed.effects[i];
                if (!(obj->flags & USED)) {
                    init_object(obj, flags);
                    return obj;
                }
            }
            break;
        
        case CURSOR:
            if (!(scene->objects.typed.cursor.flags & USED)) {
                init_object(&scene->objects.typed.cursor, flags);
                return &scene->objects.typed.cursor;
            }
            break;
    }
    return NULL;
}

void deallocate_object(game_object_t* obj) {
    if (obj != NULL)
        obj->flags = 0;
}

void push_scene(scene_t* scene) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        game_object_t* obj = &scene->objects.untyped[i];
        int pop = 1;
        
        if ((obj->flags & USED) && (obj->flags & VISABLE)) {
            int x = obj->pos.x;
            int y = obj->pos.y;

            if (obj->flags & SCROLL) {
                x -= scene->scroll.pos.x;
                y -= scene->scroll.pos.y;
            }

            int w = obj->sprite.end_x - obj->sprite.start_x;
            int h = obj->sprite.end_y - obj->sprite.start_y;

            if (obj->flags & CENTERED) {
                x -= w / 2;
                y -= h / 2;
            }

            if (x >= -w && x < SCREEN_WIDTH && y >= -h && y < SCREEN_HEIGHT) {
                obj->sprite.screen_x = x;
                obj->sprite.screen_y = y;
                pop = 0;
            }
        }

        if (pop)
            pop_sprite(i);
        else
            push_sprite(&obj->sprite, i);
              
    }
}

void clear_scene(scene_t* scene) {
    memset(scene, 0, sizeof(scene_t));
    scene->last_update = 0.0f;
}

ship_t* ship_select(scene_t* scene, int user) {
    switch (user) {
        case PLAYER:
            return scene->ships.player;
        case ENEMY:
            return scene->ships.enemy;
    }
    return NULL;
}

ship_t* allocate_ship(scene_t* scene, int user) {
    ship_t* ships = ship_select(scene, user);
    if (ships == NULL)
        return NULL;
    
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].ptr == NULL)
            return &ships[i];
    }
    return NULL;
}

int ship_count(scene_t* scene, int user) {
    ship_t* ships = NULL;

    switch (user) {
        case PLAYER:
            ships = scene->ships.player;
            break;
        case ENEMY:
            ships = scene->ships.enemy;
            break;
    }

    if (ships == NULL)
        return -1;
    
    int count = 0;
    for (int i = 0; i < USER_SHIPS; i++) {
        if (scene->ships.player[i].ptr != NULL)
            count++;
    }
    
    return count;
}

ship_t* spawn_ship(scene_t* scene, const ship_t* spawn, int user, position_t pos) {
    ship_t* ship = allocate_ship(scene, user);
    if (ship == NULL)
        return NULL;
    
    game_object_t* obj = allocate_object(scene, SHIPS, USED | VISABLE | SCROLL | CENTERED);
    if (obj == NULL)
        return NULL;

    *ship       = *spawn;
    ship->ptr   = obj;
    obj->sprite = *ship->sprite;
    obj->pos    = pos;
    ship->physics.p.x = pos.x;
    ship->physics.p.y = pos.y;
    ship->firerate_data = create_rate(ship->firerate);
    return ship;
}

void destroy_ship(scene_t* scene, ship_t* ship) {
    if (ship->sprite->address == player_cruiser.sprite->address)
        scene->shipcountc--;
    if (ship->sprite->address == player_fighter.sprite->address)
        scene->shipcountf--;
    if (ship->sprite->address == enemy_cruiser.sprite->address ||
        ship->sprite->address == enemy_fighter.sprite->address)
        scene->eshipcount--;
    deallocate_object(ship->ptr);
    ship->ptr = NULL;

    // todo: spawn ship exposion??
}

int isqrt(int x) {
    int temp, g = 0, b = 0x8000, shft = 15;
    do {
        if (x >= (temp = (((g << 1) + b) << shft--))) {
           g += b;
           x -= temp;
        }
    } while (b >>= 1);
    return g;
}

int distance(position_t p1, position_t p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return isqrt(dx * dx + dy * dy);
}

ship_t* closest_ship(scene_t* scene, int user, fposition_t pos) {
    ship_t* ships = ship_select(scene, user);
    if (ships == NULL)
        return NULL;
    
    position_t p = { pos.x, pos.y };
    
    int idx = -1;
    int lowest, dist;
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].ptr != NULL) {
            position_t sp = { ships[i].physics.p.x, ships[i].physics.p.y };
            dist = distance(p, sp);
            if (idx == -1 || dist < lowest) {
                idx = i;
                lowest = dist;
            }
        }
    }
    if (idx == -1)
    	return NULL;
    return &ships[idx];
}

weapon_t* weapon_select(scene_t* scene, int user) {
    switch (user) {
        case PLAYER:
            return scene->weapons.player;
        case ENEMY:
            return scene->weapons.enemy;
    }
    return NULL;
}

weapon_t* allocate_weapon(scene_t* scene, int user) {
    weapon_t* weapons = weapon_select(scene, user);
    if (weapons == NULL)
        return NULL;
    
    for (int i = 0; i < USER_WEAPONS; i++) {
        if (weapons[i].ptr == NULL)
            return &weapons[i];
    }
    return 0;
}

void destroy_weapon(weapon_t* weapon) {
    deallocate_object(weapon->ptr);
    weapon->ptr = NULL;

    // todo: spawn weapon exposion??
}

weapon_t* spawn_weapon(scene_t* scene, ship_t* ship, ship_t* target, fposition_t v, int user) {
    weapon_t* weapon = allocate_weapon(scene, user);
    if (weapon == NULL)
        return NULL;
        
    game_object_t* obj = allocate_object(scene, EFFECTS, USED | VISABLE | SCROLL | CENTERED);
    if (obj == NULL)
        return NULL;
        
    weapon->ptr     = obj;
    weapon->data    = (weapon_data_t*) ship->weapon;
    weapon->target  = target;

    weapon->physics.a.x = 0;
    weapon->physics.a.y = 0;
    weapon->physics.v.x = v.x;
    weapon->physics.v.y = v.y;
    weapon->physics.p.x = ship->physics.p.x + ship->fire_pos.x;
    weapon->physics.p.y = ship->physics.p.y + ship->fire_pos.y;
    weapon->timeout = get_time() + ship->weapon->timeout;

    obj->pos.x  = (int) weapon->physics.p.x;
    obj->pos.y  = (int) weapon->physics.p.y;
    obj->sprite = *weapon->data->sprites[0];
    return weapon;
}

void update_ships(scene_t* scene, int user, float dt) {
    ship_t* ships = ship_select(scene, user);
    ship_t* planet;
    position_t enemy_planet_pos;
    switch (user) {
        case PLAYER:
            planet = &scene->enemyplanet;
            enemy_planet_pos.x = (int) scene->objects.typed.background[2].pos.x;
            enemy_planet_pos.y = (int) scene->objects.typed.background[2].pos.y;
            break;
        case ENEMY:
            planet = &scene->playerplanet;
            enemy_planet_pos.x = (int) scene->objects.typed.background[1].pos.x;
            enemy_planet_pos.y = (int) scene->objects.typed.background[1].pos.y;
            break;
        default:
            return;
    }
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].ptr != NULL) {
            update_physics(&ships[i].physics, dt);
            cap_velocity(&ships[i].physics, ships[i].max_v);

            position_t p = { ships[i].physics.p.x, ships[i].physics.p.y };
            fposition_t v = { 32.0f, 0.0f };
            if (user == ENEMY)
                v.x = -v.x;

            if (distance(p, enemy_planet_pos) < ships[i].range) {
                if (is_ready(&ships[i].firerate_data))
                    spawn_weapon(scene, &ships[i], planet, v, user);
                slow_down(&ships[i].physics, ships[i].accel);
            } else {
                ship_t* enemy = closest_ship(scene, !user, ships[i].physics.p);
                position_t epos = { enemy->physics.p.x, enemy->physics.p.y };
                if (enemy == NULL || distance(p, epos) > ships[i].range) {
                    ships[i].physics.a.x = ships[i].accel;
                    ships[i].physics.a.y = 0;
                } else {
                    if (is_ready(&ships[i].firerate_data))
                        spawn_weapon(scene, &ships[i], enemy, v, user);
                    slow_down(&ships[i].physics, ships[i].accel);
                }
            }
            
            ships[i].ptr->pos.x = (int) ships[i].physics.p.x;
            ships[i].ptr->pos.y = (int) ships[i].physics.p.y;

            if (ships[i].physics.p.x > scene->max.x || ships[i].physics.p.x < 0)
                destroy_ship(scene, &ships[i]);
        }
    }
}

void update_weapons(scene_t* scene, int user, float dt) {
    weapon_t* weapons = weapon_select(scene, user);
    float time = get_time();
    for (int i = 0; i < USER_WEAPONS; i++) {
        if (weapons[i].ptr != NULL) {
            if (weapons[i].target->physics.p.x - weapons[i].physics.p.x > 0)
                weapons[i].physics.a.x = 200.0f;
            else
                weapons[i].physics.a.x = -200.0f;
            
            if (weapons[i].target->physics.p.y - weapons[i].physics.p.y > 0)
                weapons[i].physics.a.y = 200.0f;
            else
                weapons[i].physics.a.y = -200.0f;
            
            cap_velocity(&weapons[i].physics, 100.0f);

            update_physics(&weapons[i].physics, dt);

            weapons[i].ptr->pos.x = (int) weapons[i].physics.p.x;
            weapons[i].ptr->pos.y = (int) weapons[i].physics.p.y;

            position_t pos = { weapons[i].physics.p.x, weapons[i].physics.p.y };
            position_t tpos = { weapons[i].target->physics.p.x, weapons[i].target->physics.p.y };

            if (weapons[i].physics.p.x < 0 ||
                weapons[i].physics.p.x > scene->max.x ||
                weapons[i].physics.p.y < 0 ||
                weapons[i].physics.p.y > scene->max.y ||
                weapons[i].timeout < time ||
                weapons[i].target->ptr == NULL)
                destroy_weapon(&weapons[i]);
            
            if (distance(pos, tpos) < weapons[i].target->hitradius) {
                weapons[i].target->hp -= weapons[i].data->damage;
                if (weapons[i].target->hp < 0)
                    destroy_ship(scene, weapons[i].target);
                destroy_weapon(&weapons[i]);
            }
        }
    }
}

void update_game(scene_t* scene) {
    float time = get_time();
    float dt = time - scene->last_update;
    scene->last_update = time;
    update_ships(scene, PLAYER, dt);
    update_ships(scene, ENEMY, dt);
    update_weapons(scene, PLAYER, dt);
    update_weapons(scene, ENEMY, dt);
}
