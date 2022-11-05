
#ifndef COMMON_H
#define COMMON_H

#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#endif

#if DEBUG_ENABLED
#include <stdio.h>
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

#endif // COMMON_H