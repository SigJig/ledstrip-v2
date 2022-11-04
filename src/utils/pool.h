/**
 * TODO:
 * [   ] Change all the headers to the start and pack with 1 bit each address
 * [   ] Make sure every slot is aligned
 */
#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t pool_byte_ty;
typedef uint8_t pool_bitmap_ty;

#ifndef POOL_FREE_NULLIFY
#define POOL_FREE_NULLIFY 1
#endif

struct pool {
    uintptr_t mem;
    size_t length;
    size_t elem_size;
};

#define __POOL_ALIGNMENT 4
#define _BITMAP_SIZE (sizeof(pool_bitmap_ty) * 8)
#define _align(x) ((x + (__POOL_ALIGNMENT - 1)) & ~(__POOL_ALIGNMENT - 1))
#define _required_bytes(x) ((x / _BITMAP_SIZE) + ((x % _BITMAP_SIZE) != 0))
#define _bitmap_tot_sz(x) (_align(_required_bytes(x)))

#define POOL_REAL_SIZE(nitems, bytes)                                          \
    (sizeof(struct pool) + _bitmap_tot_sz(nitems) + bytes * nitems)

void p_init(pool_byte_ty*, size_t, size_t);
void* p_alloc(pool_byte_ty*);
void p_free(pool_byte_ty*, void*);

#ifdef __cplusplus
}
#endif

#endif // POOL_H