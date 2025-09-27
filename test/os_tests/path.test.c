// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include <pftest_utilities.h>
#include <os/path.h>

// stdlib
// framework
// engine


// fn pf_os_path_exists_cstr_linux ---------------------------------------------------------------------------
START_TEST(fn_pf_os_path_exists_cstr_linux__is_defined ) {
    int32_t(*fptr)(char const *, size_t const) = &pf_os_path_exists_cstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__returns_correct_error_code__for_null_path_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_exists_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__sets_correct_error_message__for_null_path_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_exists_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__returns_correct_error_code__for_zero_length_param) {
    char path[] = "some/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_exists_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__sets_correct_error_message__for_zero_length_param) {
    char path[] = "some/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_exists_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Invalid path length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_os_path_exists_cstr_linux__returns_true__if_path_is_a_file_on_disk) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    ck_assert_int_eq(TRUE, pf_os_path_exists_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__returns_true__if_path_is_a_directory_on_disk) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests";
    ck_assert_int_eq(TRUE, pf_os_path_exists_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_exists_cstr_linux__returns_false__if_path_is_not_found) {
    char path[] = "fake_path/get_wrekd/lol";
    ck_assert_int_eq(FALSE, pf_os_path_exists_cstr_linux(path, strlen(path)));
}
END_TEST


// fn pf_os_path_exists_pstr_linux ---------------------------------------------------------------------------

START_TEST(fn_pf_os_path_exists_pstr_linux__is_defined) {
    int32_t(*fptr)(PString_t) = &pf_os_path_exists_pstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_exists_pstr_linux__returns_true__if_path_is_a_file_on_disk) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(TRUE, pf_os_path_exists_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_exists_pstr_linux__returns_true__if_path_is_a_directory_on_disk) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(TRUE, pf_os_path_exists_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_exists_pstr_linux__returns_false__if_path_is_not_found) {
    char path[] = "fake_path/get_wrekd/lol";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(FALSE, pf_os_path_exists_pstr_linux(pstr));
}
END_TEST


// fn pf_os_path_is_file_cstr_linux ---------------------------------------------------------------------------

START_TEST(fn_pf_os_path_is_file_cstr_linux__is_defined) {
    int32_t(*fptr)(char const *, size_t) = &pf_os_path_is_file_cstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__returns_correct_error_code__for_null_path_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_is_file_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__sets_correct_error_message__for_null_path_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_is_file_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__returns_correct_error_code__for_zero_length_param) {
    char path[] = "some/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_is_file_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__sets_correct_error_message__for_zero_length_param) {
    char path[] = "some/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_is_file_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Invalid path length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_os_path_is_file_cstr_linux__returns_true__for_file) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    ck_assert_int_eq(TRUE, pf_os_path_is_file_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__returns_false__for_directory) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests";
    ck_assert_int_eq(FALSE, pf_os_path_is_file_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_is_file_cstr_linux__returns_false__for_invalid_path) {
    char const path[] = "fake/path/lol/get/wrekd";
    ck_assert_int_eq(FALSE, pf_os_path_is_file_cstr_linux(path, strlen(path)));
}
END_TEST

// fn pf_os_path_is_file_pstr_linux ---------------------------------------------------------------------------

START_TEST(fn_pf_os_path_is_file_pstr_linux__is_defined) {
    int32_t (*fptr)(PString_t const) = &pf_os_path_is_file_pstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_is_file_pstr_linux__returns_true__for_file) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(TRUE, pf_os_path_is_file_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_is_file_pstr_linux__returns_false__for_directory) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(FALSE, pf_os_path_is_file_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_is_file_pstr_linux__returns_false__for_invalid_path) {
    char path[] = "fake/path/lol/get/wrekd";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(FALSE, pf_os_path_is_file_pstr_linux(pstr));
}
END_TEST


// fn pf_os_directory_exists_cstr_linux ---------------------------------------------------------------------------
START_TEST(fn_pf_os_path_is_directory__is_defined) {
    int32_t (*fptr)(char const *, size_t) = &pf_os_is_directory_cstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_os_directory_exists_pstr_linux ---------------------------------------------------------------------------

// fn pf_os_file_size_cstr_linux ---------------------------------------------------------------------------
// fn pf_os_file_size_pstr_linux ---------------------------------------------------------------------------



