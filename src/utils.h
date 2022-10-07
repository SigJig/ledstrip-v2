
#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <FastLED.h>

static inline uint8_t
random_color(void)
{
    return (uint8_t)random(0, 256);
}

static inline struct CRGB
random_crgb(void)
{
    return (struct CRGB){random_color(), random_color(), random_color()};
}

#endif // UTILS_H