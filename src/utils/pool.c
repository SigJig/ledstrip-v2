
#define DEBUG_ENABLED 0

#include "pool.h"
#include "../common.h"
#include <string.h>

#define _num_bitmap(x) (_bitmap_tot_sz(x) / sizeof(pool_bitmap_ty))

static void
_init(uintptr_t mem, size_t length, size_t elem_size)
{
    memset((void*)mem, 0, POOL_REAL_SIZE(length, elem_size));

    struct pool* p = (struct pool*)mem;

    *p = (struct pool){
        .mem = mem + sizeof(*p) + _bitmap_tot_sz(length),
        .length = length,
        .elem_size = elem_size};

    DEBUG(printf("mem: %lu, %lu\n", mem, p->mem));

    size_t required = _required_bytes(length);
    size_t padding = _bitmap_tot_sz(length) - required;
    size_t numbits = _BITMAP_SIZE;

    pool_bitmap_ty* last_used =
        (pool_bitmap_ty*)(mem + sizeof(*p) + required - 1);

    DEBUG(printf(
        "pool: <padding: %zu, required: %zu, numbits: %zu>\n", padding,
        required, numbits
    ));

    /*
    Set the bits that are added for padding to 1.
    This will prevent them from being seen as an available slot
    */
    for (size_t i = 1; i <= padding; i++) {
        *(last_used + i) = 0xff;
    }
    for (uint8_t i = length % numbits; i < numbits; i++) {
        *last_used |= 0x1 << i;
    }
}

static uintptr_t
_mem_offset(struct pool* p, uint8_t offset, uint8_t index)
{
    return p->mem + ((offset * _BITMAP_SIZE) + index) * p->elem_size;
}

static uintptr_t
_alloc(uintptr_t pool_mem)
{
    if (!pool_mem) {
        return 0;
    }

    struct pool* p = (struct pool*)pool_mem;
    pool_bitmap_ty* bmap = (pool_bitmap_ty*)(pool_mem + sizeof(*p));
    uint8_t offset = 0;

    // the inverse of 0xff becomes 0x100, which is why we
    // & with the least significant 8 bits
    while (!(~(*bmap) & 0xff)) {
        // DEBUG(printf("offset: %i\n", offset));
        if (++offset >= _num_bitmap(p->length)) {
            return 0;
        }

        bmap++;
    }

    uint8_t index = 0;

    for (; (*bmap >> index) & 0x1; index++) {
    }

    DEBUG(printf("setting index: %i, offset: %i\n", index, offset));

    *bmap |= (0x1 << index);

    return _mem_offset(p, offset, index);
}

static void
_free(uintptr_t pool_mem, uintptr_t mem)
{
    if (!mem) {
        return;
    }

    struct pool* p = (struct pool*)pool_mem;

    uintptr_t diff = (mem - p->mem) / p->elem_size;
    uint8_t offset = diff / _BITMAP_SIZE;
    uint8_t index = diff % _BITMAP_SIZE;

    if (POOL_FREE_NULLIFY) {
        memset((void*)_mem_offset(p, offset, index), 0, p->elem_size);
    }

    *(pool_bitmap_ty*)(pool_mem + sizeof(*p) + offset) &= ~(0x1 << index);
}

void
p_init(pool_byte_ty* pool_mem, size_t length, size_t elem_size)
{
    _init((uintptr_t)pool_mem, length, elem_size);
}

void*
p_alloc(pool_byte_ty* pool_mem)
{
    return (void*)_alloc((uintptr_t)pool_mem);
}

void
p_free(pool_byte_ty* pool_mem, void* mem)
{
    _free((uintptr_t)pool_mem, (uintptr_t)mem);
}