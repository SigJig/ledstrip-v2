
#include "driver.h"
#include <Arduino.h>

#include "waves/base.h"
#include "waves/explode.h"

static struct fl_driver driver;
static struct wave* wv;

#define BUILTIN_TICK_INTERVAL 500

/**
 * @brief Function to indicate whether or not MCU is running
 *
 */
static void
_builtin_tick(void)
{
    static uint64_t time = 0;
    static uint8_t mode;

    uint64_t mill = millis();

    if (mill - time > BUILTIN_TICK_INTERVAL) {
        mode = !mode;
        time = mill;

        digitalWrite(LED_BUILTIN, mode);
    }
}

void
setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(A0));
    Serial.begin(9600);

    driver = driver_init(60 * 2);
    wv = wave_random(&driver);
}

void
loop()
{
    if (wv && !wave_tick(wv)) {
        driver.fastled->clear(true);
        wave_destroy(wv);
        wv = wave_random(&driver);
        Serial.println((int)(uintptr_t)wv);
    };

    _builtin_tick();
}