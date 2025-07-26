// paciFIST studios. 2025. MIT License

// includes
#include <check.h>
#include "../../src/date/datetime.h"

// stdlib
// framework
// engine
#include "../../src/core/error.h"



START_TEST(fn_get_datetime_string__is_defined) {
    void(*fptr)(char*, size_t const) = &get_datetime_string;
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



START_TEST(fn_get_datetime_string__can_return_valid_string) {
    char buffer[22];
    for (size_t i = 0; i < 22; i++) {
        buffer[i] = 0;
    }

    PF_SUPPRESS_ERRORS
    get_datetime_string(buffer, 22);
    PF_UNSUPPRESS_ERRORS

    // note: the last character is going to be a null character,
    // so don't check it b/c that *is* a zero
    for (size_t i = 0; i < 21; i++) {
        ck_assert_int_ne((unsigned char)buffer[i], 0);
    }
}
END_TEST




