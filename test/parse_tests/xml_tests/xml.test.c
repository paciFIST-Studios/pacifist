// paciFIST Studios. 2025. MIT License.

// include
#include <check.h>
#include <pftest_utilities.h>

#include <parse/xml/xml.h>

// stdlib
#include <stdio.h>
#include <sys/file.h>
// framework
// engine
#include <string/pstring.h>
#include <memory/allocator/PFAllocator_FreeList.h>

// fn pf_load_xml --------------------------------------------------------------------------------------------

START_TEST(fn_pf_load_xml__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeList_t*, PString_t, PFXmlDocument_t*) = pf_load_xml;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_load_xml__returns_false__for_null_string_in_pstring_param) {
    PFAllocator_FreeList_t allocator = {0};
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_string_in_pstring_param) {
    PFAllocator_FreeList_t allocator = {0};
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path.string!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;

START_TEST(fn_pf_load_xml__returns_false__for_zero_length_in_pstring_param) {
    PFAllocator_FreeList_t allocator = {0};
    char string[] = "string";
    PString_t const ps = { .string = string, .length = 0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_zero_length_in_pstring_param) {
    PFAllocator_FreeList_t allocator = {0};
    char string[] = "string";
    PString_t const ps = { .string = string, .length = 0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Zero length path.length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;


START_TEST(fn_pf_load_xml__returns_false__for_null_allocator_param) {
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(NULL, ps, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_allocator_param) {
    PString_t const ps = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(NULL, ps, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeList_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;

START_TEST(fn_pf_load_xml__returns_false__for_null_xml_document_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_null_xml_document_param) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFXmlDocument_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;

START_TEST(fn_pf_load_xml__returns_false__for_invalid_path_to_xml_file) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PFXmlDocument_t xml_doc = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, &xml_doc));
    PF_UNSUPPRESS_ERRORS
}
END_TEST;

START_TEST(fn_pf_load_xml__sets_correct_error_message__for_invalid_path_to_xml_file) {
    char string[] = "string";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PFXmlDocument_t xml_doc = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, &xml_doc));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got invalid path to xml file!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;


START_TEST(fn_pf_load_xml__returns_false__if_supplied_file_cannot_be_opened) {
    char string[] = "/home/ellie/git/paciFIST/test/parse_tests/xml_tests/test_file_root.xml";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PFXmlDocument_t xml_doc = {0};
    PF_SUPPRESS_ERRORS
    FILE* file = fopen(ps.string, "r");
    flockfile(file);
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, &xml_doc));
    fclose(file);
    PF_UNSUPPRESS_ERRORS
}
END_TEST;


START_TEST(fn_pf_load_xml__sets_correct_error_message__if_supplied_file_cannot_be_opened) {
    char string[] = "/home/ellie/git/paciFIST/test/parse_tests/xml_tests/test_file_root.xml";
    PString_t const ps = { .string = string, .length = strlen(string)};
    PFAllocator_FreeList_t allocator = {0};
    PFXmlDocument_t xml_doc = {0};
    PF_SUPPRESS_ERRORS
    FILE* file = fopen(ps.string, "w");
    ck_assert_int_eq(FALSE, pf_load_xml(&allocator, ps, &xml_doc));
    fclose(file);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got invalid path to xml file!";
    ck_assert_in_error_buffer(expected);
}
END_TEST;






//START_TEST(fn_pf_load_xml__returns_correct_error_code__for_successful_use) {
//    char string[] = "/home/ellie/git/paciFIST/test/parse_tests/xml_tests/test_file_root.xml";
//    PString_t const ps = { .string = string, .length = strlen(string)};
//    PFAllocator_FreeList_t allocator = {0};
//    PFXmlDocument_t doc = {0};
//    PF_SUPPRESS_ERRORS
//    ck_assert_int_eq(TRUE, pf_load_xml(&allocator, ps, &doc));
//    PF_UNSUPPRESS_ERRORS
//}
//END_TEST;


// fn pf_free_xml_doc ----------------------------------------------------------------------------------------
START_TEST(fn_pf_free_xml_doc__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeList_t*, PFXmlDocument_t*) = &pf_free_xml_doc;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// fn pf_xml_node_create_with_memory -------------------------------------------------------------------------
START_TEST(fn_pf_xml_node_create_with_memory__is_defined) {
    PFXmlNode_t*(*fptr)(PFAllocator_FreeList_t*, PFXmlNode_t*) = &pf_xml_node_create_with_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_xml_node_free ---------------------------------------------------------------------------------------
START_TEST(fn_pf_xml_node_free__is_defined) {
    void(*fptr)(PFAllocator_FreeList_t*, PFXmlNode_t*) = pf_xml_node_free;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST;


