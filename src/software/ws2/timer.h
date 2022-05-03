#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct TIMER {
    uint64_t        last_time;
    uint32_t        inv_rate;
} rate_t;

void        enable_master_timer();
void        disable_master_timer();
float       get_time();
uint64_t    get_micros();
rate_t      create_rate(float rate);
int         is_ready(rate_t* timer);

#endif
