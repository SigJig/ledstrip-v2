/*
    Copyright © 2022 Sigmund Klåpbakken

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the “Software”), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#ifndef SU_USE_STDLIB
#define SU_USE_STDLIB 1
#endif

#ifndef SU_USE_ZEPHYR
#define SU_USE_ZEPHYR 0
#endif

#if SU_USE_STDLIB
#include <stddef.h>
#include <stdio.h>

#define SU_PRINTF(...) fprintf(stdout, __VA_ARGS__)
#define SU_ERRF(...) fprintf(stderr, __VA_ARGS__)
#elif SU_USE_ZEPHYR
#include <logging/log.h>
#include <zephyr.h>

#define SU_PRINTF(...) LOG_INF(__VA_ARGS__)
#define SU_ERRF(...) LOG_ERR(__VA_ARGS__)
#else
#error "missing substitutions for stdlib functions"
#endif

typedef int (*su_test_case)(void);

#define __su_assertion_fail(check, ...)                                        \
    do {                                                                       \
        SU_ERRF("<%s: %s failed>: ", __func__, check);                         \
        SU_ERRF(__VA_ARGS__);                                                  \
        SU_ERRF("\n");                                                         \
        return 1;                                                              \
    } while (0)

#define __su_assert_helper(check, cond, msg)                                   \
    do {                                                                       \
        if (cond) {                                                            \
            break;                                                             \
        }                                                                      \
        __su_assertion_fail(check, msg);                                       \
    } while (0)

#define su_assert(cond, msg) __su_assert_helper("su_assert", cond, msg)

#define su_run_test(test, result)                                              \
    do {                                                                       \
        result = test() || result;                                             \
    } while (0)

#define su_run_all_tests(tests)                                                \
    do {                                                                       \
        int num_tests = 0, num_failed = 0;                                     \
        for (size_t i = 0; i < sizeof(tests) / sizeof(*tests); i++) {          \
            su_test_case test = tests[i];                                      \
            if (test()) {                                                      \
                num_failed++;                                                  \
            }                                                                  \
            num_tests++;                                                       \
        }                                                                      \
        SU_PRINTF(                                                             \
            "<%s> SUMMARY: Ran %i tests, where %i of them failed\n", __FILE__, \
            num_tests, num_failed                                              \
        );                                                                     \
        return num_failed != 0;                                                \
    } while (0)
