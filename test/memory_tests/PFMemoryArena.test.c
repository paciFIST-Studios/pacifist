// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/memory/allocator/PFMemoryArena.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../pftest_utilities.h"

// struct MemoryArena_t ------------------------------------------------------------------------------------------------

START_TEST(struct_MemoryArena_t__is_defined) {
    PFAllocator_MemoryArena_t const arena;
    ck_assert_ptr_nonnull(&arena);
}
END_TEST


START_TEST(struct_MemoryArena_t__has_correct_members) {
    PFAllocator_MemoryArena_t arena = {0};
    arena.bytes_owned = 0;
    arena.bytes_used = 0;
    arena.usable_base = NULL;

    ck_assert_int_eq(arena.bytes_owned, 0);
    ck_assert_int_eq(arena.bytes_used, 0);
    ck_assert_ptr_null(arena.usable_base);
}
END_TEST


START_TEST(struct_MemoryArena_t__is_correct_size) {
    PFAllocator_MemoryArena_t arena = {0};
    ck_assert_int_eq(sizeof(arena), 32); 
}
END_TEST


// fn memory_arena_set_bytes_to_zero -----------------------------------------------------------------------------------

START_TEST(fn_pf_memory_arena_set_bytes_to_zero__is_defined) {
    void (*fptr)(void* ptr, size_t const total_bytes) = & pf_memory_arena_set_bytes_to_zero;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_memory_arena_set_bytes_to_zero__does_not_overflow_requested_size) {
    size_t const size = 64;
    void* memory = malloc(size);
    ck_assert_ptr_nonnull(memory);

    // put in 1s for all bytes
    for (size_t i = 0; i < size; i++) {
        uint8_t* byte = (uint8_t*)memory + i;
        *byte = 1;
    }
    for (size_t i = 0; i < size; i++) {
        uint8_t* byte = (uint8_t*)memory + i;
        ck_assert_int_eq(*byte, 1);
    }

    // zero-out, just the last half
    size_t const quarter_size = size / 4;
    size_t const half_size = size / 2;
    size_t const three_quarter_size = quarter_size * 3;
    pf_memory_arena_set_bytes_to_zero(memory+quarter_size, half_size);

    for (size_t i = 0; i < size; i++) {
        uint8_t const * byte = (uint8_t*)memory + i;

        if (i >= quarter_size && i < three_quarter_size) {
            ck_assert_int_eq(*byte, 0);
            continue;
        }

        ck_assert_int_eq(*byte, 1);
    }
    
    free(memory);
}

// fn memory_arena_create_with_memory ----------------------------------------------------------------------------------

START_TEST(fn_pf_memory_arena_create_with_memory__is_defined) {
    PFAllocator_MemoryArena_t*(*fptr)(void*, size_t const) = &pf_memory_arena_create_with_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_memory_arena_create_with_memory__uses_memory_to_create_new_arena_correctly) {
    size_t const total_allocation = 64;
    size_t const one_quarter = total_allocation / 4;
    size_t const one_half = one_quarter * 2;
    size_t const three_quarters = one_quarter * 3;

    void* memory = calloc(total_allocation, 1);
    ck_assert_ptr_nonnull(memory);

    // set every other byte to 1, so we have a pattern to compare against 
    for (int32_t i = 0; i < total_allocation; i++) {
        uint8_t* byte = (uint8_t*)memory + i;
        *byte = 1;
    }

    // create a memory arena with some of the memory we've got,
    // let's start one quarter of the way into the memory, and
    // have the arena take up half of the memory total
    void* memory_arena_start = memory + one_quarter;
    PFAllocator_MemoryArena_t const * arena = pf_memory_arena_create_with_memory(memory_arena_start, one_half);

    // make sure it was initialized correctly
    ck_assert_ptr_nonnull(arena);
    ck_assert_ptr_nonnull(arena->usable_base);
    ck_assert_int_eq(arena->bytes_owned, one_half);
    ck_assert_int_eq(arena->bytes_used, 0);

    // re-scan all of the memory we have allocated, and make sure the memory
    // arena has been allocated correctly.  Don't forget, the memory arena
    // zeroes out all the memory it owns during creation.  This will allow us
    // to observe the changes
    for (int32_t i = 0; i < total_allocation; i++) {
        uint8_t const * byte = (uint8_t*)memory + i;

        // check against the memory arena that was created with this memory
        if (i >= one_quarter && i < three_quarters) {
            // don't forget, the first 32 bytes are reserved for the arena struct
            if (byte > arena->usable_base) {
                ck_assert_int_eq(*byte, 0);
            }
        // we're outside of the arena's owned memory,
        // so it should still be set to 1
        } else {
            ck_assert_int_eq(*byte, 1);
        }
    }
    
    free(memory);
}
END_TEST

// fn memory_arena_push_size ---------------------------------------------------------------------------------

START_TEST(fn_pf_memory_arena_push_size__is_defined) {
    void*(*fptr)(PFAllocator_MemoryArena_t*, size_t const) = pf_memory_arena_push_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_memory_arena_push_size__returns_null__for_null_ptr_to_arena) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_memory_arena_push_size(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST


START_TEST(fn_pf_memory_arena_push_size__returns_null__for_not_enough_memory) {
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 32;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    // allowed, b/c this is 16 of 32 free bytes
    void* alloc1 = pf_memory_arena_push_size(arena, 16);
    ck_assert_ptr_nonnull(arena);

    // not allowed, b/c there isn't this much left
    PF_SUPPRESS_ERRORS
    void* alloc2 = pf_memory_arena_push_size(arena, 17);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(alloc2);

    // not allowed, b/c there is exactly this much left
    PF_SUPPRESS_ERRORS
    void* alloc3 = pf_memory_arena_push_size(arena, 16);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(alloc3);

    // allowed b/c we can spare this much, and maybe make another allocation
    PF_SUPPRESS_ERRORS
    void* alloc4 = pf_memory_arena_push_size(arena, 8);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_nonnull(alloc4);

    free(memory);
}
END_TEST


START_TEST(fn_pf_memory_arena_push_size__returns_null__for_zero_request) {
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 8;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_memory_arena_push_size(arena, 0));
    PF_UNSUPPRESS_ERRORS
    
    free(memory);
}
END_TEST


START_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_null_ptr_to_arena) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_memory_arena_push_size(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot push data to PFAllocator_MemoryArena_t, without valid ptr to arena!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_not_enough_memory) {
    pf_clear_error();
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 8;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_memory_arena_push_size(arena, 16));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot push data to PFAllocator_MemoryArena_t. Not enough memory!";
    ck_assert_in_error_buffer(expected);
    
    free(memory);
}
END_TEST

START_TEST(fn_pf_memory_arena_push_size__writes_correct_error__for_zero_request) {
    pf_clear_error();
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 8;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_memory_arena_push_size(arena, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot fulfill a memory request of 0 bytes!";
    ck_assert_in_error_buffer(expected);

    free(memory);
}
END_TEST


// macro PF_PUSH_STRUCT --------------------------------------------------------------------------------------

START_TEST(fn_macro_PF_PUSH_STRUCT__works) {
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 65;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    void* alloc1 = PF_PUSH_STRUCT(arena, PFAllocator_MemoryArena_t);
    ck_assert_ptr_nonnull(alloc1);
    
    void* alloc2 = PF_PUSH_STRUCT(arena, PFAllocator_MemoryArena_t);
    ck_assert_ptr_nonnull(alloc2);

    PF_SUPPRESS_ERRORS
    void* alloc3 = PF_PUSH_STRUCT(arena, PFAllocator_MemoryArena_t);
    ck_assert_ptr_null(alloc3);
    PF_UNSUPPRESS_ERRORS

    free(memory);
}
END_TEST


// macro PF_PUSH_ARRAY ---------------------------------------------------------------------------------------

START_TEST(fn_macro_PF_PUSH_ARRAY__works) {
    // the memory arena struct sits inside this memory
    size_t const memory_size = sizeof(PFAllocator_MemoryArena_t) + 8;
    void* memory = malloc(memory_size);
    PFAllocator_MemoryArena_t* arena = pf_memory_arena_create_with_memory(memory, memory_size);
    ck_assert_ptr_nonnull(arena);

    void* alloc1 = PF_PUSH_ARRAY(arena, 7, char);
    ck_assert_ptr_nonnull(alloc1);

    PF_SUPPRESS_ERRORS
    void* alloc2 = PF_PUSH_ARRAY(arena, 1, char);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(alloc2);
    
    free(memory);
}
END_TEST