
#include "flash.h"

static uint8_t
tick(struct wave* wv)
{

    return 0;
}

static void*
make(struct wave* wv)
{

    return NULL;
}

static void
destroy(void* data)
{
}

static struct wave_iface iface {
    .tick = tick, .make = make, .destroy = destroy
};

struct wave*
flash_make(struct fl_driver* driver)
{
    return wave_make(driver, &iface);
}