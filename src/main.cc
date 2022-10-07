
#include "driver.h"
#include <Arduino.h>

#include "waves/base.h"
#include "waves/explode.h"

static struct fl_driver driver;
static struct wave* wv;

void
setup()
{
#if 1
    randomSeed(analogRead(A0));
    // Serial.begin(9600);
    driver = driver_init(60);

    wv = wave_random(&driver);
#else
// wv = NULL;
#endif
}

void
loop()
{
#if 1
    if (wv && !wave_tick(wv)) {
        driver.fastled->clear(true);
        wave_destroy(wv);
        wv = wave_random(&driver);
    };

    Serial.println((int)(uintptr_t)wv);
#else
    driver.fastled->clear();
    driver.out[0] = CRGB::Blue;
    driver.fastled->show();
#endif
}