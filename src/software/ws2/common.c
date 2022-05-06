#include "common.h"
#include "math.h"

void update_physics(physics_t* phys, float dt) {
    phys->v.x += phys->a.x * dt;
    phys->v.y += phys->a.y * dt;
    phys->p.x += phys->v.x * dt;
    phys->p.y += phys->v.y * dt;
}

void cap_velocity(physics_t* phys, float max_v) {
    if (phys->v.x > max_v)
        phys->v.x = max_v;
    else if (phys->v.x < -max_v)
        phys->v.x = -max_v;
    if (phys->v.y > max_v)
        phys->v.y = max_v;
    else if (phys->v.y < -max_v)
        phys->v.y = -max_v;
}

void slow_down(physics_t* phys, float accel) {
    if (fabsf(phys->v.x) < 1) {
        phys->v.x = 0;
        phys->a.x = 0;
    } else {
        if (phys->v.x > 0)
            phys->a.x = -accel;
        else if (phys->v.x < 0)
            phys->a.x = accel;
    }

    if (fabsf(phys->v.y) < 1) {
        phys->v.y = 0;
        phys->a.y = 0;
    } else {
        if (phys->v.y > 0)
            phys->a.y = -accel;
        else if (phys->v.y < 0)
            phys->a.y = accel;
    }
}