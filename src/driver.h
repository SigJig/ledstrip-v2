
#ifndef DRIVER_H
#define DRIVER_H

#include <FastLED.h>

#define PIN 12

struct fl_driver {
    int num_leds;
    CRGB* out;
    CFastLED* fastled;
};

static inline struct fl_driver
driver_init(int num_leds)
{
    struct fl_driver driver = {.num_leds = num_leds,
                               .out = (CRGB*)malloc(sizeof(CRGB) * num_leds),
                               .fastled = &FastLED};

    memset(driver.out, 0, sizeof(*driver.out) * num_leds);

    driver.fastled->addLeds<NEOPIXEL, PIN>(driver.out, driver.num_leds);

    return driver;
}

#endif // DRIVER_H