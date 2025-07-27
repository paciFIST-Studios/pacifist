// paciFIST studios. 2025. MIT License

// includes
#include <check.h>
#include "../../src/date/datetime.h"

// stdlib
// framework
// engine
#include "../../src/core/define.h"
#include "../../src/core/error.h"
#include "../../src/string/pstring.h"


// fn get_datetime_string ------------------------------------------------------------------------------------

START_TEST(fn_get_datetime_string__is_defined) {
    int32_t(*fptr)(char*, size_t const) = &get_datetime_string;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_get_datetime_string__requires_buffer_len_greater_or_equal_to_22) {
    char buffer[21];
    for (size_t i = 0; i < 21; i++) {
        buffer[i] = 1;
    }

    PF_SUPPRESS_ERRORS
    get_datetime_string(buffer, 21);
    PF_UNSUPPRESS_ERRORS

    // none of these have been set, b/c of early out, due to too-small-buffer
    for (size_t i = 0; i < 21; i++) {
        ck_assert_int_eq((unsigned char)buffer[i], 1);
    }
}
END_TEST

START_TEST(fn_get_datetime_string__returns_correct_error_code__for_null_out_buffer) {
    PF_SUPPRESS_ERRORS
    int32_t const ret = get_datetime_string(NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_get_date_time_string__sets_correct_error_message__for_null_out_buffer) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    get_datetime_string(NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Requires valid ptr to out buffer";
    size_t const expected_length = pf_strlen(expected);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST

START_TEST(fn_get_date_time_string__returns_correct_error_code__for_invalid_out_buffer_length) {
    PF_SUPPRESS_ERRORS
    char out_buffer[22];
    int32_t const ret = get_datetime_string(out_buffer, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_INVALID_LENGTH);
}
END_TEST


START_TEST(fn_get_date_time_string__sets_correct_error_message__for_invalid_out_buffer_length) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    char out_buffer[22];
    get_datetime_string(out_buffer, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Output buffer too small";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST

START_TEST(fn_get_datetime_string__can_return_valid_string) {
    char buffer[22];
    for (size_t i = 0; i < 22; i++) {
        buffer[i] = 0;
    }

    get_datetime_string(buffer, 22);

    // note: the last character is going to be a null character,
    // so don't check it b/c that *is* a zero
    for (size_t i = 0; i < 21; i++) {
        ck_assert_int_ne((unsigned char)buffer[i], 0);
    }
}
END_TEST

START_TEST(fn_get_datetime_string__returns_correct_error_code__for_successful_use) {
    char buffer[22];
    int32_t const ret = get_datetime_string(buffer, 22);
    ck_assert_int_eq(ret, PFEC_NO_ERROR);
}
END_TEST


