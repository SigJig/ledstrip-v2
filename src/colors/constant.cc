
#include "constant.h"

static CRGB
get(void* data)
{
    if (!data) {
        return CRGB::Black;
    }

    return *(CRGB*)data;
}

static struct colorizer_iface iface {
    .get = get, .destroy = free
};

struct colorizer
constant_color(CRGB color)
{
    CRGB* mem = (CRGB*)malloc(sizeof(*mem));

    if (mem) {
        *mem = color;
    }

    return (struct colorizer){.data = mem, .iface = &iface};
}
