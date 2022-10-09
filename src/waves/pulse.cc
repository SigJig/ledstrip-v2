
#include "pulse.h"
#include "../utils.h"
#include "colors/sin.h"

struct pulse {
    struct colorizer color;
    int16_t pos;
    struct pulse* next;
};

struct pulse_data {
    uint8_t count;
    uint8_t num_pulses;
    uint8_t pulse_interval;
    uint8_t pulse_length;
    uint8_t reverse;

    struct pulse* head;
    struct pulse* tail;
};

static uint8_t
_pulse_tick(struct pulse* pulse, struct pulse_data* data, struct wave* wv)
{
    pulse->pos++;

    if (data->reverse) {
        int16_t rpos = wv->driver->num_leds - 1 - pulse->pos;

        if (rpos + data->pulse_length < wv->driver->num_leds - 1) {
            wv->driver->out[rpos + data->pulse_length] = CRGB::Black;
        }

        if (rpos >= 0) {
            wv->driver->out[rpos] = colorizer_get(&pulse->color);
        } else if (rpos + data->pulse_length < 0) {
            return 0;
        }

        return 1;
    }

    if (pulse->pos >= data->pulse_length) {
        wv->driver->out[pulse->pos - data->pulse_length] = CRGB::Black;
    }

    if (pulse->pos < wv->driver->num_leds) {
        wv->driver->out[pulse->pos] = colorizer_get(&pulse->color);
    } else if (pulse->pos - data->pulse_length >= wv->driver->num_leds) {
        return 0;
    }

    return 1;
}

static struct pulse*
_pulse_new(struct pulse_data* pd)
{
    struct pulse* p = (struct pulse*)malloc(sizeof(*p));

    if (!p) {
        return NULL;
    }

    p->color = colorizer_sin((pd->count - 1) * (255 / pd->num_pulses));
    p->pos = 0;
    p->next = NULL;

    return p;
}

static void
_pulse_destroy(struct pulse* p)
{
    colorizer_destroy(&p->color);

    free(p);
}

static uint8_t
__available(struct pulse* last, struct pulse_data* data,
            struct fl_driver* driver)
{
    return !data->reverse && last->pos >= data->pulse_length &&
           last->pos - data->pulse_length >= data->pulse_interval;
}

static uint8_t
__available_reverse(struct pulse* last, struct pulse_data* data,
                    struct fl_driver* driver)
{
    return data->reverse && driver->num_leds - last->pos + data->pulse_length <
                                driver->num_leds - data->pulse_interval;
}

static uint8_t
tick(struct wave* wv)
{
    struct pulse_data* data = (struct pulse_data*)wv->data;
    struct pulse* last = NULL;
    struct pulse* cur = data->head;
    uint8_t last_result = 0;

    while (cur) {
        if (!(last_result = _pulse_tick(cur, data, wv))) {
            if (last) {
                last->next = cur->next;
            }

            if (data->head == cur) {
                data->head = cur->next;
            }

            struct pulse* tmp = cur->next;
            free(cur);

            last = NULL;
            cur = tmp;
        } else {
            last = cur;
            cur = cur->next;
        }
    }

    if (!last) {
        return 0;
    }

    wv->driver->fastled->show();

    if (data->count < data->num_pulses) {
        if (!last || __available(last, data, wv->driver) ||
            __available_reverse(last, data, wv->driver)) {

            data->tail->next = _pulse_new(data);
            data->tail = data->tail->next;
            data->count++;
        }
    } else {
        return last_result;
    }

    return 1;
}

static void destroy(void*);

static void*
make(struct wave* wv)
{
    struct pulse_data* data = (struct pulse_data*)malloc(sizeof(*data));

    if (!data) {
        return NULL;
    }

    data->count = 1;
    data->num_pulses = NUM_PULSES;
    data->pulse_interval = PULSE_INTERVAL;
    data->pulse_length = PULSE_LENGTH;
    data->reverse = random(0, 2);
    data->head = _pulse_new(data);

    if (!data->head) {
        destroy(data);

        return NULL;
    }

    data->tail = data->head;

    return data;
}

static void
destroy(void* data)
{
    if (!data) {
        return;
    }

    struct pulse* cur = ((struct pulse_data*)data)->head;
    struct pulse* tmp = NULL;

    while (cur) {
        tmp = cur->next;
        _pulse_destroy(cur);
        cur = tmp;
    }

    free(data);
}

static struct wave_iface iface = {
    .tick = tick, .make = make, .destroy = destroy};

struct wave*
pulse_make(struct fl_driver* driver)
{
    return wave_make(driver, &iface);
}