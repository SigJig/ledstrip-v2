
#include <Arduino.h>

#include "base.h"
#include "explode.h"
#include "implode.h"

static struct wave* (*wave_factories[])(struct fl_driver*) = {explode_make,
                                                              implode_make};

struct wave*
wave_random(struct fl_driver* driver)
{
    return wave_factories[random(0, sizeof(wave_factories) /
                                        sizeof(*wave_factories))](driver);
}

struct wave*
wave_make(struct fl_driver* driver, struct wave_iface* iface)
{
    Serial.println(1);
    struct wave* wave = (struct wave*)malloc(sizeof(*wave));

    if (!wave) {
        return NULL;
    }

    Serial.println(2);
    *wave = (struct wave){.last_tick = 0,
                          .tick_interval = 25,
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

    Serial.println(3);

    return wave;
}

void
wave_destroy(struct wave* wv)
{
    if (wv->data) {
        wv->iface->destroy(wv->data);
    }

    free(wv);
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