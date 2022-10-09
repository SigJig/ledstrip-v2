
#include "sin.h"

static CRGB
get(void* data)
{
    if (!data) {
        return CRGB::Red;
    }

    uint8_t* mem = (uint8_t*)(uintptr_t)data;
    *mem += 1;

    return CHSV(sin8(*mem), 255, 255);
}

static struct colorizer_iface iface {
    .get = get, .destroy = free
};

struct colorizer
sin_color(uint8_t offset)
{
    uint8_t* mem = (uint8_t*)malloc(sizeof(*mem));

    if (mem) {
        *mem = offset;
    }

    return (struct colorizer){.data = mem, .iface = &iface};
}
