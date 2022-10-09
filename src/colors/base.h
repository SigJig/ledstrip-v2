
#ifndef COLORS_BASE_H
#define COLORS_BASE_H

#include <FastLED.h>

struct colorizer_iface {
    CRGB (*get)(void*);
    void (*destroy)(void*);
};

struct colorizer {
    void* data;
    struct colorizer_iface* iface;
};

#endif // COLORS_BASE_H