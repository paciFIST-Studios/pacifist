// paciFIST studios. 2025. MIT License

// include
#include <stdlib.h>
#include <sys/socket.h>

#include "check.h"
#include "../../src/core/error.h"

// stdlib
// framework
// engine
// game

// macro BUILD_AND_SET_ERROR_MESSAGE --------------------------------------------------------------

//START_TEST(macro_build_and_set_error_message__is_defined) {
//# ifdef BUILD_AND_SET_ERROR_MESSAGE
//    ck_assert_int_eq(1, 1);
//#else
//    ck_assert_int_eq(1, 2);
//#endif
//}
//END_TEST

// fn get_error_buffer_size -----------------------------------------------------------------------

START_TEST(fn_get_error_buffer_size__is_defined) {
    size_t(*fptr)() = &pf_get_error_buffer_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST;

START_TEST(fn_get_error_buffer_size__has_expected_value) {
    ck_assert_int_eq(pf_get_error_buffer_size(), 4096);
}
END_TEST




// fn get_error -----------------------------------------------------------------------------------

START_TEST(fn_get_error__is_defined) {
    char* (*fptr)() = &pf_get_error;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_get_error__returns_ptr_to_error_buffer) {
    char* buffer = pf_get_error();
    ck_assert_ptr_nonnull(buffer);
}
END_TEST;


START_TEST(fn_get_error__returns_nullptr__if_errors_are_suppressed) {
    pf_set_error("a", 1);
    dnc__pf_set_error_suppressed();
    ck_assert_ptr_null(pf_get_error());
    dnc__pf_set_error_not_suppressed();
}
END_TEST


// fn set_error -----------------------------------------------------------------------------------

START_TEST(fn_set_error__is_defined) {
    void (*fptr)(char const * , size_t const) = &pf_set_error;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST;

START_TEST(fn_set_error__copies_at_most__error_buffer_size_characters){
    // setup test message buffer
    size_t const test_error_message_length = 8192;
    char * test_error_message = malloc(test_error_message_length);
    for (size_t i = 0; i < test_error_message_length; i++) {
       test_error_message[i] = '1'; 
    }
    test_error_message[test_error_message_length] = '\0';
    ck_assert_int_eq(strlen(test_error_message), test_error_message_length);

    // try to set this whole thing
    pf_set_error(test_error_message, test_error_message_length);

    // we should have only gotten half of it
    char const * error_message = pf_get_error();

    ck_assert_ptr_nonnull(error_message);
    ck_assert_int_eq(strlen(error_message), pf_get_error_buffer_size());
    // the message we got is smaller
    ck_assert_int_lt(strlen(error_message), strlen(test_error_message));

    // and all the characters were copied over
    size_t const copied_characters = pf_get_error_buffer_size() - 1;
    for (size_t i = 0; i < copied_characters; i++) {
        ck_assert(test_error_message[i] == error_message[i]);
    }
    
    free(test_error_message);
    dnc__pf_set_error_not_suppressed();
}
END_TEST


START_TEST(fn_set_error__does_not_automatically_unspresses_errors) {
    dnc__pf_set_error_suppressed();
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    pf_set_error("a", 1);
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
}
END_TEST


// fn clear_error ---------------------------------------------------------------------------------

START_TEST(fn_clear_error__is_defined) {
    void(*fptr)() = &pf_clear_error;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_clear_error__fills_buffer_with_zeroes) {
    size_t const test_length = pf_get_error_buffer_size();
    char* test_buffer = malloc(test_length);
    for (size_t i = 0; i < test_length; i++) {
        test_buffer[i] = 'A';    
    }
    test_buffer[test_length] = '\0';

    dnc__pf_set_error_not_suppressed();
    pf_set_error(test_buffer, test_length);

    char* error = pf_get_error();
    ck_assert_ptr_nonnull(error);

    for (size_t i = 0; i < test_length-1; i++) {
        ck_assert(error[i] == 'A');
    }

    pf_clear_error();

    for (size_t i = 0; i < test_length-1; i++) {
        ck_assert(error[i] == 0);
    }
    free(test_buffer);
    dnc__pf_set_error_not_suppressed();
}
END_TEST

// fn pf_set_error_suppressed ---------------------------------------------------------------------
START_TEST(fn_pf_set_error_suppressed__is_defined) {
    void(*fptr)() = &dnc__pf_set_error_suppressed;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_set_error_suppressed__turns_suppression_on) {
    dnc__pf_set_error_not_suppressed();
    ck_assert_int_eq(0, pf_get_is_error_suppressed());
    dnc__pf_set_error_suppressed();
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
}
END_TEST


// fn pf_set_error_not_suppressed -----------------------------------------------------------------
START_TEST(fn_pf_set_error_not_suppressed__is_defined) {
    void(*fptr)() = &dnc__pf_set_error_not_suppressed;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_set_error_not_suppressed__turns_suppression_off) {
    dnc__pf_set_error_suppressed();
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
    ck_assert_int_eq(0, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
}
END_TEST

// fn pf_get_is_error_suppressed ------------------------------------------------------------------
START_TEST(fn_pf_get_is_error_suppressed__is_defined) {
    int32_t(*fptr)() = &pf_get_is_error_suppressed;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pg_get_is_error_suppressed__returns_current_state_of_suppression) {
    dnc__pf_set_error_suppressed();
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
    ck_assert_int_eq(0, pf_get_is_error_suppressed());
    dnc__pf_set_error_suppressed();
    ck_assert_int_eq(1, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
    ck_assert_int_eq(0, pf_get_is_error_suppressed());
    dnc__pf_set_error_not_suppressed();
}
END_TEST

