// paciFIST studios. 2025. MIT License.

#include <check.h>
#include "engine/entry_point/pf_engine_entry_point.h"

// fn pf_app_init --------------------------------------------------------------------------------------------
START_TEST(fn_pf_app_init__is_defined) {
    SDL_AppResult (*fptr)(void**, int, char*[]) = &pf_app_init;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// correct error on pf_try_initialize_sdl_video_systems fail
// correct return value on pf_try_initialize_sdl_video_systems fail

// correct error on pf_try_initialize_sdl_image fail
// correct return value on pf_try_initialize_sdl_image fail

// correct error on pf_try_initialize_sdl_font fail
// correct return value on pf_try_initialize_sdl_font fail

// correct error on pf_try_initialize_sdl_audio fail
// correct return value on pf_try_initialize_sdl_audio fail

// correct error on pf_try_create_engine_state_struct fail
// correct return value on pf_try_create_engine_state_struct fail

// correct error on pf_try_allocate_engine_memory_from_os fail
// correct return value on pf_try_allocate_engine_memory_from_os fail

// correct error on pf_try_create_engine_lifetime_allocator fail
// correct return value on pf_try_create_engine_lifetime_allocator fail

// correct error on pf_try_read_engine_configuration fail
// correct return value on pf_try_read_engine_configuration fail

// correct return value if there are no errors


// fn pf_app_event -------------------------------------------------------------------------------------------
START_TEST(fn_pf_app_event__is_defined) {
    SDL_AppResult (*fptr)(void*, SDL_Event*) = &pf_app_event;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_app_iterate -----------------------------------------------------------------------------------------
START_TEST(fn_pf_app_iterate__is_defined) {
    SDL_AppResult (*fptr)(void*) = &pf_app_iterate;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_app_quit --------------------------------------------------------------------------------------------
START_TEST(fn_pf_app_quit__is_defined) {
    void(*fptr)(void*, SDL_AppResult) = &pf_app_quit;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST




