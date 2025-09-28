// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include <pftest_utilities.h>
#include <os/path.h>

// stdlib
#include <stdio.h>
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


// fn pf_os_is_directory_cstr_linux ---------------------------------------------------------------------------
START_TEST(fn_pf_os_path_is_directory__is_defined) {
    int32_t (*fptr)(char const *, size_t) = &pf_os_path_is_directory_cstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__returns_correct_error_code__for_null_path_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_is_directory_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__sets_correct_error_message__for_null_path_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_is_directory_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__returns_correct_error_code__for_zero_length_param) {
    char path[] = "some/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_is_directory_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__sets_correct_error_message__for_zero_length_param) {
    char path[] = "some/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_is_directory_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Invalid path length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_os_path_is_directory_cstr_linux__returns_false__for_file) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    ck_assert_int_eq(FALSE, pf_os_path_is_directory_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__returns_true__for_directory) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests";
    ck_assert_int_eq(TRUE, pf_os_path_is_directory_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_cstr_linux__returns_false__for_invalid_path) {
    char const path[] = "fake/path/lol/get/wrekd";
    ck_assert_int_eq(FALSE, pf_os_path_is_directory_cstr_linux(path, strlen(path)));
}
END_TEST

// fn pf_os_directory_exists_pstr_linux ---------------------------------------------------------------------------

START_TEST(fn_pf_os_path_is_directory_pstr_linux__is_defined) {
    int32_t (*fptr)(PString_t const) = &pf_os_path_is_directory_pstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_os_path_is_directory_pstr_linux__returns_false__for_file) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests/path.test.c";
    PString_t pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(FALSE, pf_os_path_is_directory_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_pstr_linux__returns_true__for_directory) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PString_t pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(TRUE, pf_os_path_is_directory_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_path_is_directory_pstr_linux__returns_false__for_invalid_path) {
    char path[] = "fake/path/lol/get/wrekd";
    PString_t pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(FALSE, pf_os_path_is_directory_pstr_linux(pstr));
}
END_TEST



// fn pf_os_file_size_cstr_linux ---------------------------------------------------------------------------
START_TEST(fn_pf_os_file_size_cstr_linux__is_defined) {
    size_t(*fptr)(char const *, size_t) = &pf_os_file_size_cstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__returns_correct_error_code__for_null_path_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_file_size_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__sets_correct_error_message__for_null_path_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_file_size_cstr_linux(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to path!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__returns_correct_error_code__for_zero_length_param) {
    char path[] = "some/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_file_size_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__sets_correct_error_message__for_zero_length_param) {
    char path[] = "some/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_file_size_cstr_linux(path, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Invalid path length!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_os_file_size_cstr_linux__returns_file_size__for_file) {
    char const path[] = "/home/ellie/git/paciFIST/src/splash.bmp";
    ck_assert_int_eq(2454330, pf_os_file_size_cstr_linux(path, strlen(path)));
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__returns_error_size__for_directory) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)-1, pf_os_file_size_cstr_linux(path, strlen(path)));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__sets_correct_error_message__for_directory) {
    char const path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)-1, pf_os_file_size_cstr_linux(path, strlen(path)));
    PF_UNSUPPRESS_ERRORS

    char error_message[256] = {0};
    sprintf(error_message, "Got path to a directory! Cannot give size in bytes!  Path=\"%s\"", path);
    ck_assert_in_error_buffer(error_message);
}
END_TEST

START_TEST(fn_pf_os_file_size_cstr_linux__returns_error_size__for_invalid_path) {
    char const path[] = "fake/path/lol/get/wrekd";
    ck_assert_int_eq((size_t)-1, pf_os_file_size_cstr_linux(path, strlen(path)));
}
END_TEST



// fn pf_os_file_size_pstr_linux ---------------------------------------------------------------------------

START_TEST(fn_pf_os_file_size_pstr_linux__is_defined) {
    size_t(*fptr)(PString_t const) = &pf_os_file_size_pstr_linux;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_file_size_pstr_linux__returns_file_size__for_file) {
    char path[] = "/home/ellie/git/paciFIST/src/splash.bmp";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq(2454330, pf_os_file_size_pstr_linux(pstr));
}
END_TEST

START_TEST(fn_pf_os_file_size_pstr_linux__returns_error_size__for_directory) {
    char path[] = "/home/ellie/git/paciFIST/test/os_tests";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)-1, pf_os_file_size_pstr_linux(pstr));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_file_size_pstr_linux__returns_error_size__for_invalid_path) {
    char path[] = "fake/path/lol/get/wrekd";
    PString_t const pstr = { .string = path, .length = strlen(path) };
    ck_assert_int_eq((size_t)-1, pf_os_file_size_pstr_linux(pstr));
}
END_TEST



// fn pf_os_path_join ----------------------------------------------------------------------------------------

START_TEST(fn_pf_os_path_join_cstr__is_defined) {
    size_t(*fptr)(char*, size_t, char const *, size_t, char const *, size_t) = pf_os_path_join_cstr;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_null_path_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(NULL, 0, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_null_path_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(NULL, 0, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to out_path_buffer!";
    ck_assert_in_error_buffer(expected);
}

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_zero_length_buffer_path_param) {
    char buffer[32] = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, 0, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_zero_length_buffer_path_param) {
    char buffer[32] = {0};
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, 0, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Buffer size has zero length!";
    ck_assert_in_error_buffer(expected);
}

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_null_first_path_cstr_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(buffer, buffer_size, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_null_first_path_cstr_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(buffer, buffer_size, NULL, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to first_path param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_zero_first_length_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, buffer_size, first_path, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_zero_first_length_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, buffer_size, first_path, 0, NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Zero length for first_length param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_null_second_path_cstr_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_null_second_path_cstr_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to second_path param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_zero_second_length_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    char const second_path[16] = "moar/test/path";
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_zero_second_length_param) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    char const second_path[16] = "moar/test/path";
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_INVALID_LENGTH, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Zero length for second_length param!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__returns_correct_error_code__for_buffer_size_too_small) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    char const second_path[16] = "moar/test/path";
    size_t const second_length = 16;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NOT_ENOUGH_MEMORY, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__sets_correct_error_message__for_buffer_size_to_small) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = 16;
    char const second_path[16] = "moar/test/path";
    size_t const second_length = 16;
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NOT_ENOUGH_MEMORY, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Out Buffer is not big enough to create joined path!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__returns_correct_length__for_successfully_joined_path) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = strlen(first_path);
    char const second_path[16] = "moar/test/path";
    size_t const second_length = strlen(second_path);
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(first_length+second_length+2, pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__correctly_fills_out_buffer__for_successfully_joined_path) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = strlen(first_path);
    char const second_path[16] = "moar/test/path";
    size_t const second_length = strlen(second_path);
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    size_t const copied_length = pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length);
    size_t const expected_length = first_length + second_length + /*delimiter*/ 1 + /*nul-terminator*/ 1;
    ck_assert_int_eq(expected_length, copied_length);
    PF_UNSUPPRESS_ERRORS

    for (size_t i = 0; i < copied_length; i++) {
        if (i < first_length) {
            ck_assert_int_eq((unsigned)buffer[i], (unsigned)first_path[i]);
        }
        if (i == first_length) {
            ck_assert_int_eq((unsigned)buffer[i], (unsigned)'/');
        }
        if (i > first_length) {
            // i - first_length gets us the character number, and then -1 takes it back to index form
            size_t const idx = i - first_length - 1;
            ck_assert_int_eq((unsigned)buffer[i], (unsigned)second_path[idx]);
        }
    }
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__uses_forward_slash_for_forward_slash_in_first_path__for_successfully_joined_path) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some/test/path";
    size_t const first_length = strlen(first_path);
    char const second_path[16] = "moar/test/path";
    size_t const second_length = strlen(second_path);
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    size_t const copied_length = pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length);
    ck_assert_int_eq(first_length+second_length+2, copied_length);
    PF_UNSUPPRESS_ERRORS

    ck_assert_int_eq((unsigned)buffer[first_length], (unsigned)'/');
}
END_TEST

START_TEST(fn_pf_os_path_join_cstr__uses_back_slash_for_back_slash_in_first_path__for_successfully_joined_path) {
    char buffer[32] = {0};
    size_t const buffer_size = 32;
    char const first_path[16] = "some\\test\\path";
    size_t const first_length = strlen(first_path);
    char const second_path[16] = "moar\\test\\path";
    size_t const second_length = strlen(second_path);
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    size_t const copied_length = pf_os_path_join_cstr(buffer, buffer_size, first_path, first_length, second_path, second_length);
    ck_assert_int_eq(first_length+second_length+2, copied_length);
    PF_UNSUPPRESS_ERRORS

    ck_assert_int_eq((unsigned)buffer[first_length], (unsigned)'\\');
}
END_TEST


