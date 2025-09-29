// paciFIST Studios. 2025. MIT License.

// include
#include <check.h>
#include <pftest_utilities.h>

#include <parse/xml/xml.h>

// stdlib
// framework
// engine
#include <string/pstring.h>
#include <memory/allocator/PFAllocator.h>

// fn pf_load_xml --------------------------------------------------------------------------------------------

START_TEST(fn_pf_load_xml__is_defined) {
    int32_t(*fptr)(PString_t, PFAllocator_FreeList_t const *, PFXmlDocument_t*) = pf_load_xml;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_load_xml__returns_correct_error_code__for_null_string_in_pstring_param) {
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_string_in_pstring_param) {
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path.string!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;

START_TEST(fn_pf_load_xml__returns_correct_error_code__for_zero_length_in_pstring_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = 0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_zero_length_in_pstring_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = 0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Zero length path.length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;


START_TEST(fn_pf_load_xml__returns_correct_error_code__for_null_allocator_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_allocator_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeList_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;

START_TEST(fn_pf_load_xml__returns_correct_error_code__for_null_xml_document_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, &allocator, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_xml_document_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_load_xml(ps, &allocator, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFXmlDocument_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;



START_TEST(fn_pf_load_xml__returns_correct_error_code__for_successful_use) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PFXmlDocument_t doc = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_NO_ERROR, pf_load_xml(ps, &allocator, &doc));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;
