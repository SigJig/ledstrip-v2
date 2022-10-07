
#include "driver.h"
#include <Arduino.h>

#include "waves/base.h"
#include "waves/explode.h"

static struct fl_driver driver;
static struct wave* wv;

void
setup()
{
    randomSeed(analogRead(A0));
    // put your setup code here, to run once:
    Serial.begin(9600);
    driver = driver_init(60);

    wv = explode_make(&driver);
}

void
loop()
{
    // put your main code here, to run repeatedly:
    if (wv && !wave_tick(wv)) {
        driver.fastled->clear(true);
        wave_destroy(wv);
        wv = wave_random(&driver);
    };

    Serial.println((int)(uintptr_t)wv);
}