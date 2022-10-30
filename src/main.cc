
#include "driver.h"
#include <Arduino.h>

#include "waves/base.h"
#include "waves/explode.h"

static struct fl_driver driver;
static struct wave* wv;

#include "utils/pool.h"

static pool_byte_ty pool[POOL_REAL_SIZE(10, sizeof(int))];

void
setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    randomSeed(analogRead(A0));
    Serial.begin(9600);
    driver = driver_init(60 * 2);

    Serial.print("Initializing pool with size: ");
    Serial.println(POOL_REAL_SIZE(10, sizeof(int)));
    p_init(pool, 10, sizeof(int));

    wv = wave_random(&driver);
#if 0
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
        Serial.println((int)(uintptr_t)wv);
    };

    Serial.println("Alloc: ");
    void* mem = p_alloc(pool);
    Serial.println("Free: ");
    p_free(mem);
    Serial.println("Done");
    digitalWrite(LED_BUILTIN, HIGH);

#elif 0
    driver.fastled->clear();
    driver.out[0] = CRGB::Blue;
    driver.fastled->show();
#else
    digitalWrite(LED_BUILTIN, HIGH);
#endif
}