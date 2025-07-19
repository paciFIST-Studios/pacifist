// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/string/pstring.h"
// stdlib
// framework
// engine


// struct PString_t --------------------------------------------------------------------------------------------------

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


// fn pstring_contains_substring ---------------------------------------------------------------------------------------

START_TEST(fn_pstring_contains_pstr_sub__is_defined) {
    int32_t(*fptr)(PString_t const, PString_t const) = & pstring_contains_pstr_sub;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pstring_contains_pstr_sub__returns_false_if_substring_is_longer_that_first_string) {
    PString_t pstr1 = { .string = "a", .length = 1 };
    PString_t pstr2 = { .string = "ab", .length = 2 };
    ck_assert_int_eq(pstring_contains_pstr_sub(pstr1, pstr2), 0);
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


// fn pstring_contains_char_sub ----------------------------------------------------------------------------------------

START_TEST(fn_pstring_constains_char_sub__is_defined) {
    int32_t(*fptr)(PString_t const, char const *, size_t const) = &pstring_contains_char_sub;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


