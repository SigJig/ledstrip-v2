
#include <Arduino.h>

#include "base.h"
#include "explode.h"
#include "implode.h"
#include "pulse.h"
#include "utils/pool.h"

#define POOL_LENGTH 10

static pool_byte_ty pool[POOL_REAL_SIZE(POOL_LENGTH, sizeof(struct wave))];

static struct wave* (*wave_factories[])(struct fl_driver*) = {
    explode_make,
    implode_make,
    pulse_make,
};

struct wave*
wave_random(struct fl_driver* driver)
{
    struct wave* (*fac)(struct fl_driver*) = wave_factories[random(
        0, sizeof(wave_factories) / sizeof(*wave_factories))];

    return fac(driver);
}

struct wave*
wave_make(struct fl_driver* driver, struct wave_iface* iface)
{
    static uint8_t pool_initialized = 0;

    if (!pool_initialized) {
        p_init(pool, POOL_LENGTH, sizeof(struct wave));
        pool_initialized = 1;
    }

    struct wave* wave = (struct wave*)p_alloc(pool);

    if (!wave) {
        return NULL;
    }

    *wave = (struct wave){.last_tick = 0,
                          .tick_interval = WAVE_TICK_INTERVAL,
                          .driver = driver,
                          .iface = iface,
                          .data = NULL};

    if (wave->iface->make != NULL) {
        wave->data = wave->iface->make(wave);

        if (!wave->data) {
            wave_destroy(wave);
            return NULL;
        }
    }

    return wave;
}

void
wave_destroy(struct wave* wv)
{
    if (wv->data) {
        wv->iface->destroy(wv->data);
    }

    p_free(pool, wv);
}

uint8_t
wave_tick(struct wave* wv)
{
    if (millis() - wv->last_tick < wv->tick_interval) {
        return 1;
    }

    uint8_t result = wv->iface->tick(wv);
    wv->last_tick = millis();

    return result;
}