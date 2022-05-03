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

fade_t create_fade(uint16_t color, float seconds) {
    float time = get_time();
    fade_t fade = {
        time,
        time + seconds,
        color
    };
    return fade;
}

int fade_to(fade_t* fade) {
    float bias = (get_time() - fade->start) / (fade->end - fade->start);
    int done = bias > 1;
    if (done)
        bias = 1;
    set_aura(fade->color, (uint32_t) (64 * bias));
    return done;
}

int fade_from(fade_t* fade) {
    float bias = (get_time() - fade->start) / (fade->end - fade->start);
    int done = bias > 1;
    if (done)
        bias = 1;
    bias = 1 - bias;
    set_aura(fade->color, (uint32_t) (64 * bias));
    return done;
}

int is_clicked(mouse_t* mouse, game_object_t* object) {
    return (mouse->pos.x >= object->pos.x && 
        mouse->pos.x <= object->pos.x + (object->sprite.end_x - object->sprite.start_x) &&
        mouse->pos.y >= object->pos.y && 
        mouse->pos.y <= object->pos.y + (object->sprite.end_y - object->sprite.start_y));
}

void handle_mouse(mouse_t* mouse, scene_t* scene, int h_scroll, int v_scroll) {
    if (mouse->pos.x < 0) {
        if (h_scroll) {
            scene->scroll.pos.x -= mouse->pos.x;
            if (scene->scroll.pos.x < 0)
                scene->scroll.pos.x = 0;
        }
        mouse->pos.x = 0;
    } else if (mouse->pos.x > SCREEN_WIDTH) {
        if (h_scroll) {
            scene->scroll.pos.x += mouse->pos.x - SCREEN_WIDTH;
            if (scene->scroll.pos.x > scene->scroll.pos.x)
                scene->scroll.pos.x = scene->scroll.pos.x;
        }
        mouse->pos.x = SCREEN_WIDTH;
    }       
    if (mouse->pos.y < 0) {
        if (v_scroll) {
            scene->scroll.pos.y -= mouse->pos.y;
            if (scene->scroll.pos.y < 0)
                scene->scroll.pos.y = 0;
        }
        mouse->pos.y = 0;
    } else if (mouse->pos.y > SCREEN_HEIGHT) {
        if (v_scroll) {
            scene->scroll.pos.y += mouse->pos.y - SCREEN_HEIGHT;
            if (scene->scroll.pos.y > scene->scroll.pos.y)
                scene->scroll.pos.y = scene->scroll.pos.y;
        }
        mouse->pos.y = SCREEN_HEIGHT;
    }
}