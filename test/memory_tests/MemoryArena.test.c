// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/memory/allocator/MemoryArena.h"

// stdlib
// framework
// engine


// struct MemoryArena_t ------------------------------------------------------------------------------------------------

START_TEST(struct_MemoryArena_t__is_defined) {
    MemoryArena_t const arena;
    ck_assert_ptr_nonnull(&arena);
}
END_TEST


START_TEST(struct_MemoryArena_t__has_correct_members) {
    MemoryArena_t arena = {0};
    arena.bytes_owned = 0;
    arena.bytes_used = 0;
    arena.usable_base = NULL;

    ck_assert_int_eq(arena.bytes_owned, 0);
    ck_assert_int_eq(arena.bytes_used, 0);
    ck_assert_ptr_null(arena.usable_base);
}
END_TEST


START_TEST(struct_MemoryArena_t__is_correct_size) {
    MemoryArena_t arena = {0};
    ck_assert_int_eq(sizeof(arena), 32); 
}
END_TEST


// fn memory_arena_set_bytes_to_zero -----------------------------------------------------------------------------------

START_TEST(fn_memory_arena_set_bytes_to_zero__is_defined) {
    void (*fptr)(void* ptr, size_t const total_bytes) = & memory_arena_set_bytes_to_zero;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_memory_arena_set_bytes_to_zero__does_not_overflow_requested_size) {
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
    memory_arena_set_bytes_to_zero(memory+quarter_size, half_size);

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

START_TEST(fn_memory_arena_create_with_memory__is_defined) {
    MemoryArena_t*(*fptr)(void*, size_t const) = &memory_arena_create_with_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_memory_arena_create_with_memory__uses_memory_to_create_new_arena_correctly) {
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
    MemoryArena_t const * arena = memory_arena_create_with_memory(memory_arena_start, one_half);

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


//// fn memory_arena_create_with_allocator -------------------------------------------------------------------------------
//
//START_TEST(fn_memory_arena_create_with_allocator__is_defined) {
//    MemoryArena_t* (*fptr)(size_t const, AllocatorFunction_t) = &memory_arena_create_with_allocator;
//    ck_assert_ptr_nonnull(fptr); 
//}
//END_TEST
//
//
//START_TEST(fn_memory_arena_create_with_allocator__uses_allocator_to_create_arena) {
//    size_t const outer_scope_size = 512;
//    size_t const inner_scope_size = 64;
//
//    MemoryArena_t* outer = memory_arena_create(outer_scope_size);
//    ck_assert_ptr_nonnull(outer);
//
//    //MemoryArena_t* inner = memory_arena_create_with_allocator(inner_scope_size, outer);
//    
//    memory_arena_destroy(outer);
//}
//END_TEST
//
//
//
//// fn memory_arena_create ----------------------------------------------------------------------------------------------
//
//START_TEST(fn_memory_arena_create__is_defined) {
//    MemoryArena_t* (*fptr)(size_t const usable_bytes) = &memory_arena_create;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST
//
//
//START_TEST(fn_memory_arena_create__returns_MemoryArena_t_with_initialized_settings) {
//    size_t const bytes_requested = 64;
//    size_t const arena_size = sizeof(MemoryArena_t);
//    
//    MemoryArena_t* arena = memory_arena_create(bytes_requested);
//    ck_assert_ptr_nonnull(arena);
//    ck_assert_int_eq(arena->bytes_owned, bytes_requested + arena_size);
//    ck_assert_int_eq(arena->bytes_used, 0);
//    ck_assert_ptr_nonnull(arena->usable_base);
//
//    // can we prove that the ptr is ending up in the correct place?
//    
//    memory_arena_destroy(arena);
//}
//END_TEST


// fn memory_arena_destroy ---------------------------------------------------------------------------------------------

//START_TEST(fn_memory_arena_destroy__is_defined) {
//    void (*fptr)(MemoryArena_t*) = &memory_arena_destroy;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST


//START_TEST(fn_memory_arena_destroy__zeroes_out_memory_on_deallocation) {
//    
//}
//END_TEST

