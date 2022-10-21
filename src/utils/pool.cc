
#include "pool.h"

void
p_init(pool_byte_ty* pool_mem, size_t length, size_t elem_size)
{
    for (size_t i = 0; i < POOL_REAL_SIZE(length, elem_size); i++) {
        pool_mem[i] = 0;
    }

    struct pool* p = (struct pool*)pool_mem;

    *p = (struct pool){.mem = (pool_byte_ty*)(p + 1),
                       .length = length,
                       .elem_size = elem_size};
}

void*
p_alloc(pool_byte_ty* pool_mem)
{
    struct pool* p = (struct pool*)pool_mem;

    for (size_t i = 0; i < p->length; i++) {
        pool_byte_ty* addr =
            p->mem + (p->elem_size + sizeof(pool_header_ty)) * i;

        if (!(*(pool_header_ty*)addr)) {
            return addr;
        }
    }

    return NULL;
}

void
p_free(void* mem)
{
    if (!mem) {
        return;
    }

    pool_header_ty* header = ((pool_header_ty*)mem) - 1;

    *header = 0;
}