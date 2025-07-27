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
    int32_t (*fptr)(char const *, size_t const) = &pf_string_internment_emplace_cstring;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


//START_TEST(fn_pf_string_internment_emplace_cstring__is_defined) {
//    int32_t (*fptr)(char const *, size_t const) = &pf_string_internment_emplace_cstring;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST


//// fn pf_string_internment_emplace_pstring -------------------------------------------------------------------
//START_TEST(fn_pf_string_internment_emplace_pstring__is_defined) {
//    int32_t(*fptr)(PString_t const) = &pf_string_internment_emplace_pstring;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST

