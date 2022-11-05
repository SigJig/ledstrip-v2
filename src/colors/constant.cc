
#include "constant.h"
#include "utils/pool.h"

#define POOL_LENGTH 20

static pool_byte_ty pool[POOL_REAL_SIZE(POOL_LENGTH, sizeof(CRGB))];

static CRGB
get(void* data)
{
    if (!data) {
        return CRGB::Black;
    }

    return *(CRGB*)data;
}

static void
destroy(void* mem)
{
    p_free(pool, mem);
}

static struct colorizer_iface iface {
    .get = get, .destroy = destroy
};

struct colorizer
colorizer_constant(CRGB color)
{
    POOL_ENSURE_INIT(pool, POOL_LENGTH, sizeof(CRGB));

    CRGB* mem = (CRGB*)p_alloc(pool);

    if (mem) {
        *mem = color;
    }

    return (struct colorizer){.data = mem, .iface = &iface};
}
