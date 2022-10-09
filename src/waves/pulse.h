
#ifndef PULSE_H
#define PULSE_H

#include "base.h"

#define NUM_PULSES 5
#define PULSE_LENGTH 10
#define PULSE_INTERVAL 20

struct wave* pulse_make(struct fl_driver*);

#endif // PULSE_H