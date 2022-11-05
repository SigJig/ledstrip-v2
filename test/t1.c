
#include "common.h"
#include "sigunit.h"
#include "utils/pool.h"

#define TEST_LENGTH 100

static int
test_required_bitmaps(void)
{
    su_assert(_required_bytes(TEST_LENGTH) == 13, "required byes == 13");
    su_assert(_bitmap_tot_sz(TEST_LENGTH) == 16, "bitmap_tot_sz == 16");

    return 0;
}

static int
test_init(void)
{
    pool_byte_ty mem[POOL_REAL_SIZE(TEST_LENGTH, sizeof(int))];

    POOL_ENSURE_INIT(mem, TEST_LENGTH, sizeof(int));

    struct pool* p = (struct pool*)mem;

    size_t total_size = _bitmap_tot_sz(TEST_LENGTH);
    size_t required = _required_bytes(TEST_LENGTH);

    su_assert(
        p->mem == (uintptr_t)mem + sizeof(*p) + total_size,
        "p->mem == mem + offset"
    );

    for (size_t i = 0; i < total_size - required; i++) {

        su_assert(
            *(pool_bitmap_ty*)(mem + sizeof(*p) + required + i * sizeof(pool_bitmap_ty)) ==
                0xff,
            "padding mem == 0xff"
        );
    }

    su_assert(
        *(pool_bitmap_ty*)(mem + sizeof(*p) + required - sizeof(pool_bitmap_ty)) ==
            0xf0,
        "padding bits == 1"
    );

    su_assert(p->length == TEST_LENGTH, "p->length == length");
    su_assert(p->elem_size == sizeof(int), "p->elem_size == sizeof");

    return 0;
}

static int
test_alloc(void)
{
    pool_byte_ty mem[POOL_REAL_SIZE(TEST_LENGTH, sizeof(int))];

    POOL_ENSURE_INIT(mem, TEST_LENGTH, sizeof(int));

    su_assert(
        p_alloc(mem) ==
            (void*)((uintptr_t)mem + sizeof(struct pool) + _bitmap_tot_sz(TEST_LENGTH)),
        "alloc == first slot"
    );

    for (size_t i = 1; i < TEST_LENGTH; i++) {
        su_assert(
            p_alloc(mem) ==
                ((void*)((uintptr_t)mem + sizeof(struct pool) + _bitmap_tot_sz(TEST_LENGTH) + i * sizeof(int))
                ),
            "alloc == correct slot"
        );
    }

    for (size_t i = 0; i < _bitmap_tot_sz(TEST_LENGTH); i++) {
        su_assert(
            *(pool_bitmap_ty*)(mem + sizeof(struct pool) + i * sizeof(pool_bitmap_ty)) ==
                0xff,
            "all bitmaps == 0xff"
        );
    }

    return 0;
}

static int
test_free(void)
{
    pool_byte_ty mem[POOL_REAL_SIZE(TEST_LENGTH, sizeof(int))];

    POOL_ENSURE_INIT(mem, TEST_LENGTH, sizeof(int));

    int* addresses[TEST_LENGTH];

    for (size_t i = 0; i < TEST_LENGTH; i++) {
        addresses[i] = p_alloc(mem);
    }

    for (size_t i = 0; i < TEST_LENGTH; i++) {
        p_free(mem, addresses[i]);
    }

    for (int i = 0; i < 12; i++) {
        su_assert(
            !(*(pool_bitmap_ty*)(mem + sizeof(struct pool) + i * sizeof(pool_bitmap_ty))
            ),
            "bitmaps == 0x00"
        );
    }
    su_assert(
        (*(pool_bitmap_ty*)(mem + sizeof(struct pool) + 12 * sizeof(pool_bitmap_ty))
        ) == 0xf0,
        "bits == 0"
    );

    return 0;
}

int
main(void)
{
    su_test_case tests[] = {
        test_required_bitmaps, test_init, test_alloc, test_free};

    su_run_all_tests(tests);
}