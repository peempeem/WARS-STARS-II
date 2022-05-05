#include "display.h"
#include "vga.h"
#include "gdu.h"
#include "timer.h"
#include "game.h"
#include "mouse.h"

void start_render() {
    static int frame = 0;
    while (!is_vblank());
    set_frame(frame);
    gdu_set_frame(get_frame(!frame));
    gdu_run();
    frame = !frame;
}

void double_render() {
    start_render();
    while (gdu_is_running());
    start_render();
    while (gdu_is_running());
}

fade_t create_fade(uint16_t color, int dir) {
    fade_t fade = {
        0,
        0,
        dir,
        color
    };
    return fade;
}

void start_fade(fade_t* fade, float seconds) {
    float time  = get_time();
    fade->start = time;
    fade->end   = time + seconds;
}

int is_fade_done(fade_t* fade) { return (get_time() - fade->start >= fade->end); }

int show_fade(fade_t* fade) {
    float bias = (get_time() - fade->start) / (fade->end - fade->start);
    int done = bias > 1;
    if (done)
        bias = 1;
    if (fade->dir == FADE_FROM)
        bias = 1 - bias;
    set_aura(fade->color, (uint32_t) (64 * bias));
    return done;
}

int mouse_over_object(scene_t* scene, mouse_t* mouse, int object) {
    game_object_t* obj = &scene->objects.untyped[object];
    int x = mouse->pos.x;
    int y = mouse->pos.y;
    if (obj->flags & SCROLL) {
        x += scene->scroll.pos.x;
        y += scene->scroll.pos.y;
    }
    int w = obj->sprite.end_x - obj->sprite.start_x;
    int h = obj->sprite.end_y - obj->sprite.start_y;
    if (obj->flags & CENTERED) {
        return (x >= obj->pos.x - w / 2 &&
                x <= obj->pos.x + w / 2 &&
                y >= obj->pos.y - h / 2 &&
                y <= obj->pos.y + h / 2);
    }
    return (x >= obj->pos.x && 
            x <= obj->pos.x + w &&
            y >= obj->pos.y && 
            y <= obj->pos.y + h);
}

position_t mouse_to_game(scene_t* scene, mouse_t* mouse) {
    position_t pos = {scene->scroll.pos.x + mouse->pos.x, scene->scroll.pos.y + mouse->pos.y};
    return pos;
}

void handle_mouse(mouse_t* mouse, scene_t* scene, int h_scroll, int v_scroll) {
    if (mouse->pos.x < 0) {
        if (h_scroll) {
            scene->scroll.pos.x += mouse->pos.x;
            if (scene->scroll.pos.x < 0)
                scene->scroll.pos.x = 0;
        }
        mouse->pos.x = 0;
    } else if (mouse->pos.x > SCREEN_WIDTH) {
        if (h_scroll) {
            scene->scroll.pos.x += mouse->pos.x - SCREEN_WIDTH;
            if (scene->scroll.pos.x > scene->scroll.max.x)
                scene->scroll.pos.x = scene->scroll.max.x;
        }
        mouse->pos.x = SCREEN_WIDTH;
    }       
    if (mouse->pos.y < 0) {
        if (v_scroll) {
            scene->scroll.pos.y += mouse->pos.y;
            if (scene->scroll.pos.y < 0)
                scene->scroll.pos.y = 0;
        }
        mouse->pos.y = 0;
    } else if (mouse->pos.y > SCREEN_HEIGHT) {
        if (v_scroll) {
            scene->scroll.pos.y += mouse->pos.y - SCREEN_HEIGHT;
            if (scene->scroll.pos.y > scene->scroll.max.y)
                scene->scroll.pos.y = scene->scroll.max.y;
        }
        mouse->pos.y = SCREEN_HEIGHT;
    }
}