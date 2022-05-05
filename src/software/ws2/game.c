#include "game.h"
#include "vga.h"
#include <string.h>
#include <math.h>

void init_object(game_object_t* object, int flags) {
    memset(object, 0, sizeof(game_object_t));
    object->flags = flags;
}

int allocate_object(scene_t* scene, int type, int flags) {
    switch (type) {
        case BACKGROUND:
            for (int i = 0; i < BACKGROUND_SPRITES; i++) {
                if (!(scene->objects.typed.background[i].flags & USED)) {
                    init_object(&scene->objects.typed.background[i], flags);
                    return i;
                }
            }
            break;
        
        case SHIPS:
            for (int i = 0; i < SHIP_SPRITES; i++) {
                if (!(scene->objects.typed.ships[i].flags & USED)) {
                    init_object(&scene->objects.typed.ships[i], flags);
                    return i + BACKGROUND_SPRITES;
                }
            }
            break;
        
        case EFFECTS:
            for (int i = 0 + SHIP_SPRITES; i < EFFECT_SPRITES; i++) {
                if (!(scene->objects.typed.effects[i].flags & USED)) {
                    init_object(&scene->objects.typed.effects[i], flags);
                    return i + BACKGROUND_SPRITES + SHIP_SPRITES;
                }
            }
            break;
        
        case CURSOR:
            if (!(scene->objects.typed.cursor.flags & USED)) {
                init_object(&scene->objects.typed.cursor, flags);
                return MAX_SPRITES - 1;
            }
            break;
    }
    return -1;
}

void deallocate_object(scene_t* scene, uint32_t object) {
    if (object < MAX_SPRITES)
        scene->objects.untyped[object].flags = 0;
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

    for (int i = 0; i < SHIP_SPRITES; i++) {
        scene->ships.player[i].index    = -1;
        scene->ships.enemy[i].index     = -1;
    }
}

ship_data_t* ship_select(scene_t* scene, int user) {
    switch (user) {
        case PLAYER:
            return scene->ships.player;
        case ENEMY:
            return scene->ships.enemy;
    }
    return NULL;
}

int allocate_ship(scene_t* scene, int user) {
    ship_data_t* ships = ship_select(scene, user);
    if (ships == NULL)
        return -1;
    
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].index == -1)
            return i;
    }
    return -1;
}

int ship_count(scene_t* scene, int user) {
    ship_data_t* ships = NULL;

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
        if (scene->ships.player[i].index != -1)
            count++;
    }
    
    return count;
}

int spawn_ship(scene_t* scene, const ship_t* ship, int user, position_t pos) {
    int sd_idx  = allocate_ship(scene, user);
    if (sd_idx == -1)
        return -1;
    
    int obj_idx = allocate_object(scene, SHIPS, USED | VISABLE | SCROLL | CENTERED);
    if (obj_idx == -1)
        return -1;

    ship_data_t* ships = ship_select(scene, user);

    ships[sd_idx].index     = obj_idx;
    ships[sd_idx].hp        = ship->hp;
    ships[sd_idx].speed     = ship->speed;
    ships[sd_idx].firerate  = ship->firerate;
    
    scene->objects.untyped[obj_idx].sprite  = *ship->sprite;
    scene->objects.untyped[obj_idx].pos     = pos;
    return sd_idx;
}

void destroy_ship(scene_t* scene, int user, uint32_t ship) {
    ship_data_t* ships = ship_select(scene, user);
    if (ships == NULL || ship >= USER_SHIPS)
        return;
    
    deallocate_object(scene, ships[ship].index);
    ships[ship].index = -1;

    // todo: spawn exposion??
}

float distance(position_t p1, position_t p2) { return sqrtf(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)); }

int closest_ship(scene_t* scene, int user, position_t pos) {
    ship_data_t* ships = ship_select(scene, user);
    if (ships == NULL)
        return -1;
    
    int idx = -1;
    float lowest, dist;
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].index != -1) {
            dist = distance(pos, scene->objects.untyped[ships[i].index].pos);
            if (idx == -1 || dist < lowest) {
                idx = i;
                lowest = dist;
            }
        }
    }
    return idx;
}

void update_game(scene_t* scene) {
    ship_data_t* ships = scene->ships.player;
    for (int i = 0; i < USER_SHIPS; i++) {
        if (ships[i].index != -1) {
            scene->objects.untyped[ships[i].index].pos.x += ships[i].speed;

            if (scene->objects.untyped[ships[i].index].pos.x >= 1000)
                destroy_ship(scene, PLAYER, i);
        }
    }
}

