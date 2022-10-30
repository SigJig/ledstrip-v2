/**
 * TODO:
 * [   ] Change all the headers to the start and pack with 1 bit each address
 * [   ] Make sure every slot is aligned
 */
#ifndef POOL_H
#define POOL_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t pool_byte_ty;
typedef uint32_t pool_header_ty;

struct pool {
    pool_byte_ty* mem;
    size_t length;
    size_t elem_size;
};

#define POOL_REAL_SIZE(nitems, bytes)                                          \
    (sizeof(struct pool) + (bytes + sizeof(pool_header_ty)) * nitems)

void p_init(pool_byte_ty*, size_t, size_t);
void* p_alloc(pool_byte_ty*);
void p_free(void*);

#endif // POOL_H