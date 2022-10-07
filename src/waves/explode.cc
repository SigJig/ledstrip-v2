
#include "explode.h"
#include "../utils.h"

struct explode_data {
    CRGB color;
    uint8_t offset;
    uint8_t center;
};

static uint8_t
tick(struct wave* wv)
{
    struct explode_data* data = (struct explode_data*)wv->data;

    wv->driver->out[data->center + data->offset] = data->color;
    wv->driver->out[data->center - data->offset] = data->color;
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
    if (data) {
        free(data);
    }
}

static void*
make(struct wave* wv)
{
    struct explode_data* data = (struct explode_data*)malloc(sizeof(*data));

    if (!data) {
        return NULL;
    }

    data->color = random_crgb();
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