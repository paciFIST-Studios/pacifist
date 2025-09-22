// paciFIST studios. 2025. MIT License

#include <check.h>
#include "engine/entry_point/PFEngineInitialization.h"

#include "engine/PFEngineConfiguration.h"
#include "memory/allocator/PFMemoryArena.h"





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

// pf_try_create_engine_lifetime_allocator -------------------------------------------------------------------
START_TEST(fn_pf_try_create_engine_lifetime_allcoator__is_defined) {
    int32_t(*fptr)(void*, size_t, PFAllocator_MemoryArena_t**) = &pf_try_create_engine_lifetime_allocator;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_read_engine_configuration --------------------------------------------------------------------------
START_TEST(fn_pf_try_read_engine_configuration__is_defined) {
    int32_t(*fptr)(int, char*[], PFAllocator_MemoryArena_t*, PFEngineConfiguration_t**) = &pf_try_read_engine_configuration;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_try_create_engine_state_struct -------------------------------------------------------------------------
START_TEST(fn_pf_try_create_engine_state_struct__is_defined) {
    int32_t(*fptr)(void*, PFEngineState_t**) = &pf_try_create_engine_state_struct;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST



