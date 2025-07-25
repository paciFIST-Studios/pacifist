// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/string/pstring.h"
// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../src/core/define.h"
#include "../../src/core/error.h"

// struct PString_t ------------------------------------------------------------------------------------------

START_TEST(struct_PString_t__is_defined) {
    PString_t pstr = { .string = NULL, .length = 0 };
    ck_assert_ptr_nonnull(&pstr);
    // also checks members
    ck_assert_ptr_null(pstr.string);
    ck_assert_int_eq(pstr.length, 0);
}
END_TEST

START_TEST(struct_PString_t__has_expected_size) {
    ck_assert_int_eq(sizeof(PString_t), 16);
}


// fn pstring_contains_substring -----------------------------------------------------------------------------

START_TEST(fn_pstring_contains_pstr_sub__is_defined) {
    int32_t(*fptr)(PString_t const, PString_t const) = & pstring_contains_pstr_sub;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pstring_contains_pstr_sub__returns_false__if_substring_is_longer_that_first_string) {
    PString_t pstr1 = { .string = "a", .length = 1 };
    PString_t pstr2 = { .string = "ab", .length = 2 };
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pstring_contains_pstr_sub(pstr1, pstr2));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pstring_contains_pstr_sub__logs_correct_error__if_substring_is_longer_than_first_string) {
    PString_t pstr1 = { .string = "a", .length = 1 }; 
    PString_t pstr2 = { .string = "ab", .length = 2 };
    pf_clear_error();
    
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pstring_contains_pstr_sub(pstr1, pstr2));
    PF_UNSUPPRESS_ERRORS

    char const expected_error[] = "Searching for substring whose length is longer than the base string!";
    size_t const expected_length = pf_strlen(expected_error);
    char * found_error = pf_get_error();

    PString_t pstr_found_error = { .string = found_error, .length = pf_strlen(found_error) };
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(pstr_found_error, expected_error, expected_length));
}
END_TEST


START_TEST(fn_pstring_contains_pstr_sub__can_find_a_substring) {
    PString_t a = { .string = "ab", .length = 2 };
    PString_t b = { .string =  "a", .length = 1 };
    
    //pstring_contains_substring(a, b);  
    ck_assert_int_eq(pstring_contains_pstr_sub(a, b), 1);  
}
END_TEST

START_TEST(fn_pstring_contains_pstr_sub__stress_testing) {
    char * test_string_a = "gold old ld d";
    char * test_substring_a = "gold";
    PString_t ta = { .string = test_string_a, .length = strlen(test_string_a) };
    PString_t tsa = { .string = test_substring_a, .length = strlen(test_substring_a) };

    ck_assert_int_eq(1, pstring_contains_pstr_sub(ta, tsa));
    
    char * test_string = "There are strange things done in the midnight sun by the men who moil for gold";
    char * test_substring1 = "range";
    char * test_substring2 = " in the ";
    char * test_substring3 = "moil";
    char * test_substring4 = "12moil345";
    PString_t a = { .string = test_string, .length = strlen(test_string) };
    PString_t b = { .string = test_substring1, .length = strlen(test_substring1) };
    PString_t c = { .string = test_substring2, .length = strlen(test_substring2) };
    PString_t d = { .string = test_substring3, .length = strlen(test_substring3) };
    PString_t e = { .string = test_substring4, .length = strlen(test_substring4) };

    // matches substring which is part of a word
    ck_assert_int_eq(1, pstring_contains_pstr_sub(a, b));
    // matches substring which includes whitespace
    ck_assert_int_eq(1,  pstring_contains_pstr_sub(a, c));
    // matches substring which is a word
    ck_assert_int_eq(1,  pstring_contains_pstr_sub(a, d));
    // does not match substring which doesn't appear,
    // even if part of the substring does appear
    ck_assert_int_eq(0,  pstring_contains_pstr_sub(a, e));
}
END_TEST


// fn pstring_contains_char_sub ------------------------------------------------------------------------------

START_TEST(fn_pstring_constains_char_sub__is_defined) {
    int32_t(*fptr)(PString_t const, char const *, size_t const) = &pstring_contains_char_sub;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pstring_slice ------------------------------------------------------------------------------------------

START_TEST(fn_pstring_slice__is_defined) {
    PString_t(*fptr)(PString_t const, int32_t const, int32_t const) = &pstring_slice;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pstirng_slice__returns_null_result__of_correct_form) {
    PString_t pstr = { .string = NULL, .length = 0 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 1);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_nonnull(&result);
    ck_assert_int_eq(result.length, 0);
    ck_assert_ptr_null(result.string);
}
END_TEST

START_TEST(fn_pstring_slice__returns_null_result__for_pstr_arg_with_null_string) {
    PString_t pstr = { .string = NULL, .length = 1 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 1);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(result.string);
    ck_assert_int_eq(result.length, 0);
}
END_TEST

START_TEST(fn_pstring_slice__sets_correct_error_message__for_pstring_arg_with_null_string) {
    PString_t const pstr = { .string = NULL, .length = 1 };
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 1);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Null pointer passed to base string!";
    size_t const expected_length = pf_strlen(expected_error);
    ck_assert_int_eq(TRUE,  pstring_contains_char_sub(error, expected_error, expected_length)); 
}
END_TEST

START_TEST(fn_pstring_slice__returns_null_result__for_pstr_arg_with_zero_length) {
    PString_t pstr = { .string = "test", .length = 0 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 1);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(result.string);
    ck_assert_int_eq(result.length, 0);
}
END_TEST

START_TEST(fn_pstring_slive__sets_correct_error_message__for_pstr_arg_with_zero_length) {
    PString_t pstr = { .string = "test", .length = 0 };
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 1);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Invalid string length!";
    size_t const expected_length = pf_strlen(expected_error);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST

START_TEST(fn_pstring_slice__returns_null_result__for_request_of_zero_length_slice) {
    PString_t pstr = { .string = "words", .length = 5 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 2, 2);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(result.string);
    ck_assert_int_eq(result.length, 0);
}

START_TEST(fn_pstring_slice__sets_correct_error_message__for_request_of_zero_length_slice) {
    PString_t pstr = { .string = "words", .length = 5 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 2, 2);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Cannot create a zero-length slice!";
    size_t const expected_length = pf_strlen(expected_error);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST

START_TEST(fn_pstring_slice__returns_null_result__for_out_of_bounds_begin_idx) {
    PString_t pstr = { .string = "a", .length = 1 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 2, 1);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(result.string);
    ck_assert_int_eq(result.length, 0);
}

START_TEST(fn_pstring_slice__sets_correct_error_message__for_out_of_bounds_begin_idx) {
    PString_t pstr = { .string = "a", .length = 1 };
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 2, 1);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Begin parameter wants out of bounds access to string!";
    size_t const expected_length = pf_strlen(expected_error);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST

START_TEST(fn_pstring_slice__returns_null_result__for_out_of_bounds_end_idx) {
    PString_t pstr = { .string = "a", .length = 1 };
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 2);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(result.string);
    ck_assert_int_eq(result.length, 0);
}
END_TEST

START_TEST(fn_pstring_slice__sets_correct_error_message__for_out_of_bounds_end_idx) {
    PString_t const pstr = { .string = "a", .length = 1 };
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PString_t result = pstring_slice(pstr, 0, 2);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "End parameter wants out of bounds access to string!";
    size_t const expected_length = pf_strlen(expected_error);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST

START_TEST(fn_pstring_slice__returns_non_owning_pstring__for_valid_slice) {
    char const test_string[] = "This is a test, it is only a test";
    size_t const test_length = pf_strlen(test_string);
    PString_t const pstr = { .string = (char*)test_string, test_length };

    int32_t const begin = 10;
    int32_t const end = 14;
    PString_t result = pstring_slice(pstr, begin, end);
    ck_assert_ptr_nonnull(result.string);
    ck_assert_int_eq(result.length, end - begin);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(result, "test", 4));
}
END_TEST

START_TEST(fn_pstring_slice__returns_slice__when_using_negative_indicies) {
    char const test_string[] = "This is a string.  Oh there's an html tag in it <b>bold</b>";
    size_t const test_length = pf_strlen(test_string);
    PString_t const pstr = { .string = (char*)test_string, test_length };

    int32_t const begin = -12;
    int32_t const end = PSTR_LAST;
    size_t const len = abs(end - begin);
    PString_t result = pstring_slice(pstr, begin, end);
    ck_assert_ptr_nonnull(result.string);
    ck_assert_int_eq(result.length, len);
}
END_TEST

START_TEST(fn_pstring_slice__returns_slice__when_using_positive_then_negative_indicies) {
    char const test_string[] = "GameplayTag=JustALilGuy";
    size_t const test_length = pf_strlen(test_string);
    PString_t const pstr = { .string = (char*)test_string, test_length };

    int32_t const begin = 12;
    int32_t const end = PSTR_LAST;
    size_t const len = test_length - begin;
    PString_t result = pstring_slice(pstr, begin, end);
    ck_assert_ptr_nonnull(result.string);
    ck_assert_int_eq(result.length, len);
}
END_TEST

START_TEST(fn_pstring_slice__feels_okay_in_casual_usage) {
    char const test_string[] = "player.skill.fireball.level.1";
    size_t const test_length = pf_strlen(test_string);
    PString_t const pstr = { .string = (char*)test_string, test_length };

    PString_t result = pstring_slice(pstr, 14, PSTR_LAST);

    char * fireball = malloc(9);
    for (size_t i = 0; i < 9; i++) { fireball[i] = 0; }
    
    strncpy(fireball, result.string, 9);

    int32_t bContainsSubstring = pstring_contains_char_sub(result, fireball, 9);
    ck_assert_int_eq(bContainsSubstring, TRUE);
    
    free(fireball);
}















