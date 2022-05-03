#include "timer.h"

#define TIMER_MEM_LOCATION 0x00002170

typedef struct TIMER_REGISTERS {
    uint32_t enabled;
    uint64_t time;
} timer_regs_t;

timer_regs_t* timer_regs = (timer_regs_t*) TIMER_MEM_LOCATION;

void enable_master_timer() { timer_regs->enabled = 1; }

void disable_master_timer() { timer_regs->enabled = 0; }

float get_time() { return timer_regs->time / (float) 1e7; }

uint64_t get_micros() { return timer_regs->time / 10; }

rate_t create_rate(float hz) {
    rate_t rate = {
        timer_regs->time,
        (uint32_t) ((1 / hz) * 1e7)
    };
    return rate;
}

int is_ready(rate_t* rate) {
    uint64_t time = timer_regs->time;
    int diff = time - (rate->last_time + rate->inv_rate);
    if (diff >= 0) {
        if (diff >= rate->inv_rate)
            rate->last_time = time;
        else
            rate->last_time = time + diff;
        return 1;
    }
    return 0;
}
