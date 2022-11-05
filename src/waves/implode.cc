
#include "implode.h"
#include "colors/sin.h"
#include "utils.h"
#include "utils/pool.h"

struct implode_data {
    struct colorizer color;
    uint8_t center, offset;
};

#define POOL_LENGTH 1

static pool_byte_ty
    pool[POOL_REAL_SIZE(POOL_LENGTH, sizeof(struct implode_data))];

static uint8_t
tick(struct wave* wv)
{
    struct implode_data* data = (struct implode_data*)wv->data;

    wv->driver->out[data->offset] = colorizer_get(&data->color);
    wv->driver->out[wv->driver->num_leds - 1 - data->offset] =
        colorizer_get(&data->color);

    wv->driver->fastled->show();

    data->offset++;

    if (data->offset > data->center) {
        return 0;
    }

    return 1;
}

static void
destroy(void* data)
{
    if (data) {
        colorizer_destroy(&((struct implode_data*)data)->color);

        p_free(pool, data);
    }
}

static void*
make(struct wave* wv)
{
    POOL_ENSURE_INIT(pool, POOL_LENGTH, sizeof(struct implode_data));

    struct implode_data* data = (struct implode_data*)p_alloc(pool);

    if (!data) {
        return NULL;
    }

    data->color = colorizer_sin(random(0, 256));
    data->center = (wv->driver->num_leds >> 1) - 1;
    data->offset = 0;

    return data;
}

static struct wave_iface iface {
    .tick = tick, .make = make, .destroy = destroy
};

struct wave*
implode_make(struct fl_driver* driver)
{
    return wave_make(driver, &iface);
}