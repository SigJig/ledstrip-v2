
#include "sin.h"
#include "utils/pool.h"

#define POOL_LENGTH 20

static pool_byte_ty pool[POOL_REAL_SIZE(POOL_LENGTH, sizeof(uint8_t))];

static CRGB
get(void* data)
{
    if (!data) {
        return CRGB::Red;
    }

    uint8_t* mem = (uint8_t*)(uintptr_t)data;
    *mem += 1;

    return CHSV(sin8(*mem), 255, 100);
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
colorizer_sin(uint8_t offset)
{
    POOL_ENSURE_INIT(pool, POOL_LENGTH, sizeof(uint8_t));

    uint8_t* mem = (uint8_t*)p_alloc(pool);

    if (mem) {
        *mem = offset;
    }

    return (struct colorizer){.data = mem, .iface = &iface};
}
