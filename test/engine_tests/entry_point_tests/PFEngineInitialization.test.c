// paciFIST studios. 2025. MIT License

#include <check.h>
#include <pftest_utilities.h>
#include <engine/entry_point/PFEngineInitialization.h>

// stdlib
// framework
// engine
#include <core/define.h>
#include <engine/PFEngineConfiguration.h>
#include <memory/allocator/PFMemoryArena.h>





// pf_allocate_engine_memory ---------------------------------------------------------------------------------
START_TEST(fn_pf_allocate_engine_memory__is_defined) {
    void*(*fptr)(size_t) = &pf_allocate_engine_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_deallocate_engine_memory -------------------------------------------------------------------------------
START_TEST(fn_pf_deallocate_engine_memory__is_defined) {
    void(*fptr)(void*,size_t) = &pf_deallocate_engine_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_initialize_sdl_video_systems -----------------------------------------------------------------------
START_TEST(fn_pf_initialize_sdl_video_systems__is_defined) {
    int32_t(*fptr)(int, char*[]) = &pf_try_initialize_sdl_video_systems;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// pf_try_initialize_sdl_image -------------------------------------------------------------------------------
START_TEST(fn_pf_initialize_sdl_image__is_defined) {
    int32_t(*fptr)(int, char*[]) = &pf_try_initialize_sdl_image;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_initialize_sdl_font --------------------------------------------------------------------------------
START_TEST(fn_pf_initialize_sdl_font__is_defined) {
    int32_t(*fptr)(int, char*[]) = &pf_try_initialize_sdl_font;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_initialize_sdl_audio -------------------------------------------------------------------------------
START_TEST(fn_pf_initialize_sdl_audio__is_defined) {
    int32_t(*fptr)(int, char*[]) = &pf_try_initialize_sdl_audio;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_allocate_engine_memory_from_os ---------------------------------------------------------------------
START_TEST(fn_pf_try_allocate_engine_memory_from_os__is_defined) {
    int32_t(*fptr)(size_t, void**) = &pf_try_allocate_engine_memory_from_os;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_try_allocate_engine_memory_from_os__returns_false__for_null_out_memory_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_allocate_engine_memory_from_os(1, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_allocate_engine_memory_from_os__sets_correct_error_message__for_null_out_memory_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_allocate_engine_memory_from_os(1, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Need an out-param in order to allocate engine memory from os!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_allocate_engine_memory_from_os__returns_false__for_zero_memory_size_param) {
    void* memory = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_allocate_engine_memory_from_os(0, &memory));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_allocate_engine_memory_from_os__sets_correct_error_message__for_zero_memory_size_param) {
    void* memory = NULL;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_allocate_engine_memory_from_os(0, &memory));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot allocate 0 bytes for engine memory!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_allocate_engine_memory_from_os__can_allocate_memory__when_used) {
    void* memory = NULL;
    size_t const size = 128;
    ck_assert_int_eq(TRUE, pf_try_allocate_engine_memory_from_os(size, &memory));
    ck_assert_ptr_nonnull(memory);
    pf_deallocate_engine_memory(memory, size);
}

// todo: figure out what makes malloc fail, and do that, so we can test that condition


// pf_try_create_engine_lifetime_allocator -------------------------------------------------------------------
START_TEST(fn_pf_try_create_engine_lifetime_allcoator__is_defined) {
    int32_t(*fptr)(void*, size_t, PFAllocator_MemoryArena_t**) = &pf_try_create_engine_lifetime_allocator;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__returns_false__for_null_ptr_to_lifetime_memory_base) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(NULL, 0, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__sets_correct_error_message__for_null_ptr_to_lifetime_memory_base) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(NULL, 0, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got NULL ptr for lifetime-memory-base param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__returns_false__for_zero_memory_size) {
    char memory[16];
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(memory, 0, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__sets_correct_error_message__for_zero_memory_size) {
    char memory[16];
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(memory, 0, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got zero for memory-size param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__returns_false__for_null_out_lifetime_allocator) {
    size_t const size = 16;
    char memory[size];
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(memory, size, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_lifetime_allocator__sets_correct_error_message__for_null_out_lifetime_allocator) {
    size_t const size = 16;
    char memory[size];
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_lifetime_allocator(memory, size, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got null ptr for out-lifetime-allocator param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

// pf_try_create_engine_state_struct -------------------------------------------------------------------------
START_TEST(fn_pf_try_create_engine_state_struct__is_defined) {
    int32_t(*fptr)(PFAllocator_MemoryArena_t*, PFEngineState_t**) = &pf_try_create_engine_state_struct;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__returns_false__for_null_allocator_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__sets_correct_error_message__for_null_allocator_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got NULL ptr to allocator param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__returns_false__for_out_engine_state_param) {
    PFAllocator_MemoryArena_t arena = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(&arena, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__sets_correct_error_message__for_out_engine_state_param) {
    PFAllocator_MemoryArena_t arena = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(&arena, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got NULL ptr to out-engine-state param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__returns_false__if_allocator_cannot_push_memory) {
    PFAllocator_MemoryArena_t arena = {0};
    PF_SUPPRESS_ERRORS
    PFEngineState_t* engine_state = NULL;
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(&arena, &engine_state));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_struct__sets_correct_error_message__if_allocator_cannot_push_memory) {
    PFAllocator_MemoryArena_t arena = {0};
    PF_SUPPRESS_ERRORS
    PFEngineState_t* engine_state = NULL;
    ck_assert_int_eq(FALSE, pf_try_create_engine_state_struct(&arena, &engine_state));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Could not allocate from provided allocator!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

// this fn also allocates the string internmnet
// this fn also allocates the string internment of the correct size



// pf_try_read_engine_configuration --------------------------------------------------------------------------
START_TEST(fn_pf_try_read_engine_configuration__is_defined) {
    int32_t(*fptr)(int, char*[], PFAllocator_MemoryArena_t*, PFEngineConfiguration_t**) = &pf_try_read_engine_configuration;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


