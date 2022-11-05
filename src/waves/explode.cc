
#include "explode.h"
#include "colors/sin.h"
#include "utils.h"
#include "utils/pool.h"

struct explode_data {
    struct colorizer color;
    uint8_t offset;
    uint8_t center;
};

#define POOL_LENGTH 1

static pool_byte_ty
    pool[POOL_REAL_SIZE(POOL_LENGTH, sizeof(struct explode_data))];

static uint8_t
tick(struct wave* wv)
{
    struct explode_data* data = (struct explode_data*)wv->data;

    wv->driver->out[data->center + data->offset] = colorizer_get(&data->color);
    wv->driver->out[data->center - data->offset] = colorizer_get(&data->color);
    wv->driver->fastled->show();

    data->offset++;

    if (data->center + data->offset >= wv->driver->num_leds) {
        return 0;
    }

    return 1;
}

static void
destroy(void* data)
{
    colorizer_destroy(&((struct explode_data*)data)->color);

    if (data) {
        p_free(pool, data);
    }
}

static void*
make(struct wave* wv)
{
    POOL_ENSURE_INIT(pool, POOL_LENGTH, sizeof(struct explode_data));

    struct explode_data* data = (struct explode_data*)p_alloc(pool);

    if (!data) {
        return NULL;
    }

    data->color = colorizer_sin(random(0, 256));
    data->offset = 0;
    data->center = (wv->driver->num_leds >> 1) - 1;

    return data;
}

static struct wave_iface iface {
    .tick = tick, .make = make, .destroy = destroy
};

struct wave*
explode_make(struct fl_driver* driver)
{
    return wave_make(driver, &iface);
}