// paciFIST studios. 2025. MIT License

#ifndef PFTEST_UTILITIES_H
#define PFTEST_UTILITIES_H

// include
// stdlib
// framework
#include <check.h>
// engine
#include <core/define.h>
#include <core/error.h>
#include <string/pstring.h>

#define ck_assert_in_error_buffer(expected) do {pf_assert_in_error_buffer_inner(expected);} while(0);

// asserts that the supplied string is a substring of the error currently located in the engine error buffer
static inline void pf_assert_in_error_buffer_inner(char const * expected){
    char const * error_message = pf_get_error();
    size_t const error_length = strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length};

    size_t const expected_length = strlen(expected);

    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected, expected_length));
}





#endif //PFTEST_UTILITIES_H
