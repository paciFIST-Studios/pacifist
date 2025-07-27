// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/string/PFStringInternmentSingleton.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../src/core/define.h"
#include "../../src/core/error.h"
#include "../../src//string/pstring.h"

// Constants -------------------------------------------------------------------------------------------------

START_TEST(constant_PFSI_MAX_STRINGS__is_expected_value) {
    ck_assert_int_eq(PFSI_MAX_STRINGS, 126);
}
END_TEST



// struct PFStringInternmentSingleton_t ----------------------------------------------------------------------

START_TEST(struct_PFStringInternmentSingleton_t__is_defined) {
    PFStringInternmentSingleton_t const strings = {0};
    ck_assert_ptr_nonnull(&strings);
}
END_TEST

START_TEST(struct_PFStringInternmentSingleton_t__has_correct_size) {
    ck_assert_int_eq(sizeof(PFStringInternmentSingleton_t), 2048);
}

START_TEST(struct_PFStringInternmentSingleton_t__has_expected_members) {
    PFStringInternmentSingleton_t const strings = {0};
    ck_assert_ptr_null(strings.usable_memory_base);
    ck_assert_int_eq(strings.owned_memory_size, 0);
    ck_assert_int_eq(strings.used_memory_size, 0);
    ck_assert_int_eq(strings.next_unused_idx, 0);
    ck_assert_ptr_nonnull(strings.strings);
}
END_TEST

// fn pf_string_internment_initialize ------------------------------------------------------------------------
START_TEST(fn_pf_string_internment_initialize__is_defined) {
    int32_t(*fptr)(PFStringInternmentSingleton_t *, void *, size_t const) = &pf_string_internment_initialize;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_string_internment_initialize__returns_correct_error_code__for_null_string_internment_param) {
    PF_SUPPRESS_ERRORS
    int32_t ret = pf_string_internment_initialize(NULL, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_pf_string_internment_initialize__sets_correct_error_message__for_null_string_internment_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_initialize(NULL, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Null ptr to PFStringInternmentSingleton";
    size_t const expected_length = strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST

START_TEST(pf_string_internment_initialize__returns_correct_error_code__for_null_memory_base_param) {
    PFStringInternmentSingleton_t string_internment = {0};
    
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    int32_t const ret = pf_string_internment_initialize(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_NULL_PTR);
}

START_TEST(pf_string_internment_initialize__sets_correct_error_message__for_null_memory_base_param) {
    PFStringInternmentSingleton_t string_internment = {0};

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_initialize(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Null ptr to PFStringInternmentSingleton memory base";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST

START_TEST(pf_string_internment_initialize__returns_correct_error_code__for_invalid_memory_size) {
    PFStringInternmentSingleton_t string_internment = {0};
    void* memory = malloc(128);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    int32_t const ret = pf_string_internment_initialize(&string_internment, memory, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_INVALID_LENGTH);

    free(memory);
}
END_TEST

START_TEST(pf_string_internment_initialize__sets_correct_error_message__for_invalid_memory_size) {
    PFStringInternmentSingleton_t string_internment = {0};
    void* memory = malloc(128);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_initialize(&string_internment, memory, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Invalid memory size for PFStringInternmentSingleton";
    size_t const expected_length = strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));

    free(memory);
}
END_TEST

START_TEST(pf_string_internment_initialize__correctly_initializes_string_internment__for_expected_arguments) {
    PFStringInternmentSingleton_t string_internment = {0};

    size_t const memory_size = 16 * PFSI_MAX_STRINGS;
    void* memory = malloc(memory_size);

    int32_t const ret = pf_string_internment_initialize(&string_internment, memory, memory_size);
    ck_assert_int_eq(ret, PFEC_NO_ERROR);

    ck_assert_ptr_eq(string_internment.usable_memory_base, memory);
    ck_assert_int_eq(string_internment.owned_memory_size, memory_size);
    ck_assert_int_eq(string_internment.used_memory_size, 0);
    ck_assert_int_eq(string_internment.next_unused_idx, 0);

    for (size_t i = 0; i < PFSI_MAX_STRINGS; i++) {
        ck_assert_ptr_null(string_internment.strings[i].string);
        ck_assert_int_eq(string_internment.strings[i].length, 0);
    }

    free(memory);
}
END_TEST


// fn pf_string_internment_emplace_cstring -------------------------------------------------------------------
START_TEST(fn_pf_string_internment_emplace_cstring__is_defined) {
    PString_t (*fptr)(PFStringInternmentSingleton_t*, char *, size_t const) = &pf_string_internment_emplace_cstr;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// PFStringInternmentSingleton_t* param

START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_PFStringInternmentSingleton_t) {
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(NULL, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_PFStirngInternmentSingleton_t) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(NULL, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Null ptr to PFStringInternmentSingleton";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_memory_base_in_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_memory_base_in_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Null ptr to PFStringInternmentSingleton usable memory base";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_invalid_owned_memory_size_in_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = 0;
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_INVALID_LENGTH);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_invalid_owned_memory_size_in_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = 0;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "PFStringInternmentSingleton invalid owned memory size";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_used_memory_size_greather_than_owned_memory_size_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.used_memory_size = size + 1;
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_OUT_OF_BOUNDS_MEMORY_USE);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_used_memory_sie_greater_than_owned_memory_size_in_PFStringInternmentSingleton_t) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.used_memory_size = size + 1;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "PFStringInternmentSingleton is using memory it doesn't own";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


// char * cstring param

START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_null_ptr_to_cstring) {
    PFStringInternmentSingleton_t string_internment = {0};
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_null_ptr_to_cstring) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Null ptr to cstring";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST

// size_t const length param

START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_invalid_length) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    char test_string[] = "test string";
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, test_string, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_INVALID_LENGTH);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_invalid_length) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    char test_string[] = "test string";

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, test_string, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Invalid string length";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


// fn internal checks

START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_out_of_memory_error) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.used_memory_size = size;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_OUT_OF_MEMORY);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_out_of_memory_error) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.used_memory_size = size;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Ran out of memory for PFStringInternmentSingleton";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_error_code__for_out_of_tracking_indices) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.next_unused_idx = PFSI_MAX_STRINGS;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_null(ret.string);
    ck_assert_int_eq(ret.length, PFEC_ERROR_OUT_OF_MEMORY);
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_correct_error_message__for_out_of_tracking_indices) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    string_internment.next_unused_idx = PFSI_MAX_STRINGS;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected[] = "Ran out of tracking indices for PFStringInternmentSingleton";
    size_t const expected_length = pf_strlen(expected);

    ck_assert_int_eq(TRUE, pstring_contains_char_sub(error, expected, expected_length));
}
END_TEST


START_TEST(fn_pf_string_internment_emplace_cstr__returns_correct_pstr__for_valid_args) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    PString_t mem = { .string = test_memory, .length = size };
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_nonnull(ret.string);
    ck_assert_int_eq(ret.length, test_length);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(ret, test_string, test_length));
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__copies_string_to_memory__for_valid_args) {
    PFStringInternmentSingleton_t string_internment = {0};
    size_t const size = 32;
    char test_memory[size];
    PString_t mem = { .string = test_memory, .length = size };
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;
    char test_string[] = "test string";
    size_t const test_length = pf_strlen(test_string);
    PF_SUPPRESS_ERRORS
    PString_t const ret = pf_string_internment_emplace_cstr(&string_internment, test_string, test_length);
    PF_UNSUPPRESS_ERRORS
    ck_assert_ptr_nonnull(ret.string);
    ck_assert_int_eq(ret.length, test_length);
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(mem, test_string, test_length));
}
END_TEST

START_TEST(fn_pf_string_internment_emplace_cstr__sets_memory_used_correctly_after_copy) {
    // setup memory
    size_t const size = 32;
    char test_memory[size];
    for (size_t i = 0; i < size; i++) { test_memory[i] = 0; }
    PString_t const mem = { .string = test_memory, .length = size };

    // setup string internment
    PFStringInternmentSingleton_t string_internment = {0};
    string_internment.usable_memory_base = test_memory;
    string_internment.owned_memory_size = size;

    char test_string1[] = "test string";
    size_t const test_length1 = pf_strlen(test_string1);
    PString_t const ret1 = pf_string_internment_emplace_cstr(&string_internment, test_string1, test_length1);
    // valid return value?
    ck_assert_ptr_nonnull(ret1.string);
    ck_assert_int_eq(ret1.length, test_length1);
    // return value has correct information?
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(ret1, test_string1, test_length1));
    // information was copied to provided memory?
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(mem, test_string1, test_length1));

    char test_string2[] = "another test string";
    size_t const test_length2 = pf_strlen(test_string2);
    PString_t const ret2 = pf_string_internment_emplace_cstr(&string_internment, test_string2, test_length2);
    // valid return value?
    ck_assert_ptr_nonnull(ret2.string);
    ck_assert_int_eq(ret2.length, test_length2);
    // return value has correct information?
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(ret2, test_string2, test_length2));
    // information was copied to provided memory?
    ck_assert_int_eq(TRUE, pstring_contains_char_sub(mem, test_string2, test_length2));

    // expected amount of memory is used?
    size_t const expected_memory_used = test_length1 + 1 + test_length2 + 1;
    ck_assert_int_eq(expected_memory_used, string_internment.used_memory_size);
}
END_TEST


//// fn pf_string_internment_emplace_pstring -------------------------------------------------------------------
//START_TEST(fn_pf_string_internment_emplace_pstring__is_defined) {
//    int32_t(*fptr)(PString_t const) = &pf_string_internment_emplace_pstring;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST

