
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

static inline CRGB
colorizer_get(struct colorizer* colorizer)
{
    return colorizer->iface->get(colorizer->data);
}

static inline void
colorizer_destroy(struct colorizer* colorizer)
{
    if (colorizer->iface->destroy) {
        colorizer->iface->destroy(colorizer->data);
    }
}

#endif // COLORS_BASE_H