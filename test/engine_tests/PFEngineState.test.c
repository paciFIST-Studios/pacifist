// paciFIST studios. 2025. MIT License

// includes
#include <check.h>
#include "../../src/engine/PFEngineState.h"

// stdlib
// framework
// engine

// struct PFEngineState_t ------------------------------------------------------------------------------------

START_TEST(fn_struct_PFEngineState_t__is_defined) {
    PFEngineState_t const engine_state;
    ck_assert_ptr_nonnull(&engine_state);
}

START_TEST(fn_struct_PFEngineState_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PFEngineState_t), 2048);
}
END_TEST

START_TEST(fn_struct_PFEngineState_t__has_expected_members) {
    PFEngineState_t const engine_state = {0};
    ck_assert_ptr_nonnull(&engine_state.m_string_internment);
}
END_TEST


// fn pf_engine_state_initialize -----------------------------------------------------------------------------

START_TEST(fn_pf_engine_state_initialize__is_defined) {
    int32_t(*fptr)(PFEngineState_t *) = &pf_engine_state_initialize;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_engine_state_initialize__correctly_initializes_engine_state) {
    PFEngineState_t engine_state = {0};
    int32_t const result = pf_engine_state_initialize(&engine_state);
    ck_assert_int_eq(result, PFEC_NO_ERROR);
}
END_TEST



// fn pf_engine_state_destroy --------------------------------------------------------------------------------
START_TEST(fn_pf_engine_state_destroy__is_defined) {
    int32_t(*fptr)(PFEngineState_t *) = &pf_engine_state_destroy;
    ck_assert_ptr_nonnull(fptr);
}


// fn pf_engine_state_save -----------------------------------------------------------------------------------
START_TEST(fn_pf_engine_state_save__is_defined) {
    int32_t(*fptr)(PFEngineState_t const *) = &pf_engine_state_save;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// fn pf_engine_state_load -----------------------------------------------------------------------------------
START_TEST(fn_pf_engine_state_load__is_defined) {
    int32_t(*fptr)(PFEngineState_t *) = &pf_engine_state_load;
    ck_assert_ptr_nonnull(fptr);
}












