
#ifndef WAVE_BASE_H
#define WAVE_BASE_H

#include "../driver.h"

#define WAVE_TICK_INTERVAL 25

struct wave_iface {
    uint8_t (*tick)(struct wave*);
    void* (*make)(struct wave*);
    void (*destroy)(void*);
};

struct wave {
    uint64_t last_tick;
    uint32_t tick_interval;
    struct fl_driver* driver;

    struct wave_iface* iface;
    void* data;
};

struct wave* wave_random(struct fl_driver* driver);

uint8_t wave_tick(struct wave* wv);
struct wave* wave_make(struct fl_driver* driver, struct wave_iface* iface);

void wave_destroy(struct wave* wv);

#endif // WAVE_BASE_H