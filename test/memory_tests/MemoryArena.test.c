// paciFIST studios. 2025. MIT License

// include
#include <check.h>

#include "../../src/memory/allocator/MemoryArena.h"

// stdlib
// framework
// project

// struct MemoryArena_t ------------------------------------------------------------------------------------------------

START_TEST(struct_MemoryArena_t__is_defined) {
    MemoryArena_t arena;
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
    ck_assert_int_eq(sizeof(arena), 24); 
}
END_TEST


// fn initialize_arena -------------------------------------------------------------------------------------------------

START_TEST(fn_memory_arena_create__is_defined) {
    MemoryArena_t* (*fptr)(size_t const usable_bytes) = &memory_arena_create;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_memory_arena_create__returns_MemoryArena_t_with_initialized_settings) {
//    MemoryArena_t* arena = (MemoryArena_t*)memory_arena_create(64);
//    ck_assert_ptr_nonnull(arena); 
//    ck_assert_int_eq(arena->size, 64);
//    ck_assert_int_eq(arena->used, 0);
// //   ck_assert_ptr_nonnull(arena->base);
//    
//    free(arena->base);
//    free(arena);
}
END_TEST

// fn memory_arena_destroy ---------------------------------------------------------------------------------------------

START_TEST(fn_memory_arena_destroy__is_defined) {
    void (*fptr)(MemoryArena_t*) = &memory_arena_destroy;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

//START_TEST(fn_memory_arena_destroy__zeroes_out_memory_on_deallocation) {
//    
//}
//END_TEST

