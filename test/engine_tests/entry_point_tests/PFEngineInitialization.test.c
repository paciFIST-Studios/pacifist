// paciFIST studios. 2025. MIT License

#include <check.h>
#include <pftest_utilities.h>
#include <engine/entry_point/pf_engine_initialization.h>

// stdlib
#include <stdlib.h>
// framework
// engine
#include <core/define.h>
#include <engine/engine_define.h>
#include <engine/PFEngineConfiguration.h>
#include <memory/allocator/PFMemoryArena.h>


// pf_get_program_window -------------------------------------------------------------------------------------
START_TEST(fn_pf_get_program_window__is_defined) {
    SDL_Window*(*fptr)(void) = &pf_get_program_window;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_get_program_window__returns_null_if_sdl_is_not_initialized) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_get_program_window());
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_get_program_window__sets_correct_error_message__if_sdl_is_not_initialized) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_get_program_window());
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Someone called pf_get_program_window before initializing SDL_VIDEO!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

// pf_get_sdl_renderer ---------------------------------------------------------------------------------------
START_TEST(fn_pf_get_sdl_renderer__is_defined) {
    SDL_Renderer*(*fptr)(void) = &pf_get_sdl_renderer;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_get_sdl_renderer__returns_null_if_sdl_is_not_initialized) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_get_sdl_renderer());
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_get_sdl_renderer__sets_correct_error_message__if_sdl_is_not_initialized) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_get_sdl_renderer());
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Someone called pf_get_sdl_renderer before initializing SDL_VIDEO!";
    ck_assert_in_error_buffer(expected);
}
END_TEST



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

START_TEST(fn_pf_try_create_engine_state_struct__allocates_string_internment_singleton_as_well__when_called) {
    size_t const size = ENGINE_MEMORY_TOTAL_ALLOCATION_SIZE;
    void* memory = malloc(size);
    ck_assert_ptr_nonnull(memory);
    
    PFAllocator_MemoryArena_t* arena = pf_allocator_memory_arena_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(arena);

    PFEngineState_t* engine_state = NULL;
    ck_assert_int_eq(TRUE, pf_try_create_engine_state_struct(arena, &engine_state));
    ck_assert_ptr_nonnull(engine_state);

    ck_assert_ptr_nonnull(engine_state->p_lifetime_string_internment);
    ck_assert_int_eq(engine_state->p_lifetime_string_internment->owned_memory_size, ENGINE_SYSTEM_SIZE__STRING_INTERNMENT_MEMORY);
    
    free(memory);
}
END_TEST

START_TEST(fn_pf_try_create_engine_state_Struct__allocates_recoverable_allocator_as_well__when_called) {
    size_t const size = ENGINE_MEMORY_TOTAL_ALLOCATION_SIZE;
    void* memory = malloc(size);
    ck_assert_ptr_nonnull(memory);

    PFAllocator_MemoryArena_t* arena = pf_allocator_memory_arena_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(arena);

    PFEngineState_t* engine_state = NULL;
    ck_assert_int_eq(TRUE, pf_try_create_engine_state_struct(arena, &engine_state));
    ck_assert_ptr_nonnull(engine_state);

    ck_assert_ptr_nonnull(engine_state->p_recoverable_memory_allocator);
    ck_assert_int_eq(engine_state->p_recoverable_memory_allocator->base_memory_size, ENGINE_ALLOCATOR_SIZE__RECOVERABLE_MEMORY);
    
    free(memory);
}
END_TEST



// pf_try_read_engine_configuration --------------------------------------------------------------------------
START_TEST(fn_pf_try_read_engine_configuration__is_defined) {
    int32_t(*fptr)(int, char*[], PFEngineState_t const *, PFEngineConfiguration_t**) = &pf_try_read_engine_configuration;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__returns_false__for_null_ptr_to_engine_state_param) {
    int const argc = 1;
    char* argv[] = { "" };
    
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__sets_correct_error_message__for_null_ptr_to_engine_state_param) {
    int const argc = 1;
    char* argv[] = { "" };

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got null ptr to PFEngineState_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_try_read_engine_configuration__returns_false__for_null_ptr_to_out_engine_configuration_param) {
    int const argc = 1;
    char* argv[] = { "" };
    PFEngineState_t const engine_state = {0};
    
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__sets_correct_error_message__for_null_ptr_to_out_engine_configuration_param) {
    int const argc = 1;
    char* argv[] = { "" };
    PFEngineState_t const engine_state = {0};

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got null ptr to out_engine_configuration param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__returns_false__for_null_ptr_to_engine_state_param_lifetime_memory_allocator_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFEngineState_t const engine_state = { 0 };
    PFEngineConfiguration_t* engine_configuration = NULL;

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__sets_correct_error_message__for_null_ptr_to_engine_state_param_lifetime_memory_allocator_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFEngineState_t const engine_state = { 0 };
    PFEngineConfiguration_t* engine_configuration = NULL;

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFEngineState_t had null ptr to lifetime_memory_allocator!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_try_read_engine_configuration__returns_false__for_null_ptr_to_engine_state_param_recoverable_memory_allocator_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFAllocator_MemoryArena_t arena = {0};
    PFEngineState_t const engine_state = { .p_lifetime_memory_allocator = &arena };
    PFEngineConfiguration_t* engine_configuration = NULL;

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__sets_correct_error_message__for_null_ptr_to_engine_state_param_recoverable_memory_allocator_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFAllocator_MemoryArena_t arena = {0};
    PFEngineState_t const engine_state = { .p_lifetime_memory_allocator = &arena };
    PFEngineConfiguration_t* engine_configuration = NULL;

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFEngineState_t had null ptr to recoverable_memory_allocator!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__returns_false__for_null_ptr_to_engine_state_param_lifetime_string_internment_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFAllocator_MemoryArena_t arena = {0};
    PFAllocator_FreeList_t recoverable = {0};
    PFEngineState_t const engine_state = { .p_lifetime_memory_allocator = &arena, .p_recoverable_memory_allocator = &recoverable };
    PFEngineConfiguration_t* engine_configuration = NULL;

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_try_read_engine_configuration__sets_correct_error_message__for_null_ptr_to_engine_state_param_lifetime_string_internment_member) {
    int const argc = 1;
    char* argv[] = { "" };
    PFAllocator_MemoryArena_t arena = {0};
    PFAllocator_FreeList_t recoverable = {0};
    PFEngineState_t const engine_state = { .p_lifetime_memory_allocator = &arena, .p_recoverable_memory_allocator = &recoverable };
    PFEngineConfiguration_t* engine_configuration = NULL;

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFEngineState_t had null ptr to lifetime_string_internment!";
    ck_assert_in_error_buffer(expected);
}
END_TEST



START_TEST(fn_pf_try_read_engine_configuration__returns_true__for_successful_use) {
    int const argc = 1;
    char* argv[] = { "Example/Execution/Path/String" };

    /**
     * 20250923 - EBarrett
     *
     *  This test mimics the actual calling of this fn during engine startup.
     *  I'm hardcoding the values at the current size, hoping they'll stay valid
     *  for the duration of the project.
     *
     *  Total memory is split into two groups (halves, at time of writing)
     *
     *      "Lifetime" group    - memory which can be allocated, but not deallocated
     *      "Recoverable" group - memory which can be allocated and deallocated
     *
     *  The string_internment struct lives in the lifetime group.  Once all of this
     *  is setup, we can call the fn, and it should actually do work using this memory,
     *  and then return true
     */
    size_t const size = Kibibytes(256);
    void* memory = malloc(size);

    PFAllocator_MemoryArena_t* scope = pf_allocator_memory_arena_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(scope);

    // lifetime
    size_t const lifetime_size = Kibibytes(128);
    void* lifetime_memory = pf_allocator_memory_arena_push_size(scope, lifetime_size);
    PFAllocator_MemoryArena_t* lifetime = pf_allocator_memory_arena_create_with_memory(lifetime_memory, lifetime_size);
    ck_assert_ptr_nonnull(lifetime);

    // recoverable
    size_t const recoverable_size = Kibibytes(127);
    void* recoverable_memory = pf_allocator_memory_arena_push_size(scope, recoverable_size);
    PFAllocator_FreeList_t* recoverable = pf_allocator_free_list_create_with_memory(recoverable_memory, recoverable_size);
    ck_assert_ptr_nonnull(recoverable);

    // string_internment lives in lifetime
    size_t const string_internment_size = Kibibytes(32);
    void* string_internment_memory = pf_allocator_memory_arena_push_size(lifetime, string_internment_size);
    PFStringLifetimeInternmentSingleton_t* string_internment = pf_string_lifetime_internment_create_with_memory(
        string_internment_memory, string_internment_size);
    ck_assert_ptr_nonnull(string_internment);

    // a "real" engine state
    PFEngineState_t const engine_state = {
        .p_lifetime_memory_allocator = lifetime,
        .p_recoverable_memory_allocator = recoverable,
        .p_lifetime_string_internment = string_internment
    };
    PFEngineConfiguration_t* engine_configuration = NULL;

    // fn call and result measurement
    ck_assert_int_eq(TRUE, pf_try_read_engine_configuration(argc, argv, &engine_state, &engine_configuration));
    ck_assert_ptr_nonnull(engine_configuration);

    free(memory);
}
END_TEST
