// paciFIST studios. 2025. MIT License

#ifndef PFTEST_UTILITIES_H
#define PFTEST_UTILITIES_H

// include
// stdlib
// framework
#include <check.h>
// engine
#include "../src/core/define.h"
#include "../src/core/error.h"
#include "../src/string/pstring.h"



static inline void ck_assert_in_error_buffer(char const * expected){
    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length};

    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected, expected_length));
}




#endif //PFTEST_UTILITIES_H
