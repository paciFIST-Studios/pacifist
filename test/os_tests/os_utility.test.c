// paciFIST studios. 2025. MIT License.

// include
#include <check.h>
#include "../../src/os/os_utility.h"

// stdlib
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
// framework
// engine
#include "../../src/core/define.h"
#include "../../src/core/error.h"
#include "../../src/string/pstring.h"

// fn file_size --------------------------------------------------------------------------------------------------------

START_TEST(fn_file_size__is_defined) {
    int64_t(*fptr)(const char *, size_t const) = &file_size;    
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_file_size__returns_minus_one__for_null_path) {
    //pf_set_error_suppressed();
    ck_assert(file_size(NULL, 0) == -1);
    //pf_set_error_not_suppressed();
}

START_TEST(fn_file_size__returns_minus_one__for_zero_path_length) {
    //pf_set_error_suppressed();
    char * path = "";
    ck_assert(file_size(path, 0) == -1);
    //pf_set_error_not_suppressed();
}
END_TEST

START_TEST(fn_file_size__returns_minus_one__if_file_does_not_exist) {
    //pf_set_error_suppressed();
    char path[200];
    for (size_t i = 0; i < 200; i++) { path[i] = 0; }
    sprintf(path, "%s.get_wrekd", __FILE__);
    ck_assert(file_size(path, pf_strlen(path)) == -1);
    //pf_set_error_not_suppressed();
    
}
END_TEST

START_TEST(fn_file_size__writes_correct_error__for_null_path) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    file_size(NULL, 0);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid ptr to file path";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST

START_TEST(fn_file_size__writes_correct_error__for_zero_path_length) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    char * file_path = "";
    file_size(file_path, 0);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid file path length";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST

START_TEST(fn_file_size__writes_correct_error__if_file_does_not_exist) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    char path[200];
    for (size_t i = 0; i < 200; i++) { path[i] = 0; }
    sprintf(path, "%s.get_wrekd", __FILE__);
    file_size(path, pf_strlen(path));

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "File does not exist";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST


// fn is_file ----------------------------------------------------------------------------------------------------------

START_TEST(fn_is_file__is_defined) {
    int32_t (*fptr)(const char *, size_t const) = &is_file; 
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_is_file__returns_minus_one__for_null_path) {
    //pf_set_error_suppressed();
    ck_assert_int_eq(-1, is_file(NULL, 0));
    //pf_set_error_not_suppressed();
}
END_TEST

START_TEST(fn_is_file__returns_minus_one__for_zero_path_length) {
    //pf_set_error_suppressed();
    char * path = "";
    ck_assert_int_eq(-1, is_file(path, 0));
    //pf_set_error_not_suppressed();
}
END_TEST

START_TEST(fn_is_file__returns_zero__if_file_does_not_exist) {
    char path[200];
    for (size_t i = 0; i < 200; i++) { path[i] = 0; }
    sprintf(path, "%s.get_wrekd", __FILE__);
    size_t const len = pf_strlen(path);
    ck_assert_int_eq(FALSE, is_file(path, len)); 
}
END_TEST

START_TEST(fn_is_file__returns_one__if_file_does_exist) {
    char const * path = __FILE__;
    size_t const len = pf_strlen(path);
    ck_assert_int_eq(TRUE, is_file(path, len)); 
}
END_TEST

START_TEST(fn_is_file__writes_correct_error__for_null_path) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    is_file(NULL, 1);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid ptr to file path";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST

START_TEST(fn_is_file__writes_correct_error__for_zero_path_length) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    char * path = "";
    is_file(path, 0);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid file path length";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST


// fn is_directory -----------------------------------------------------------------------------------------------------

START_TEST(fn_is_directory__is_defined) {
    int32_t (*fptr)(const char *, size_t const) = &is_directory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_is_directory__returns_minus_one__for_null_path) {
    //pf_set_error_suppressed();
    ck_assert_int_eq(-1, is_directory(NULL, 0));
    //pf_set_error_not_suppressed();
}
END_TEST

START_TEST(fn_if_directory__returns_minus_one__for_zero_path_length) {
    //pf_set_error_suppressed();
    char * path = "";
    ck_assert_int_eq(-1, is_directory(path, 0));
    //pf_set_error_not_suppressed();
}
END_TEST

START_TEST(fn_is_directory__returns_zero__if_path_is_not_a_directory) {
    char* path = __FILE__;
    size_t const len = pf_strlen(path);
    ck_assert_int_eq(FALSE, is_directory(path, len));
}
END_TEST

START_TEST(fn_is_directory__returns_one__if_path_is_a_directory) {
    char path[PATH_MAX];
    for (size_t i = 0; i < PATH_MAX; i++) { path[i] = 0; }
    ck_assert_ptr_nonnull(getcwd(path, PATH_MAX)); 
    size_t const len = pf_strlen(path);
    ck_assert_int_eq(TRUE, is_directory(path, len)); 
}
END_TEST

START_TEST(fn_is_directory__writes_correct_error__for_null_path) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    is_directory(NULL, 1);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid ptr to file path";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
}
END_TEST

START_TEST(fn_is_directory__writes_correct_error__for_zero_path_length) {
    // turn on error suppression, so it won't write to stdout
    //pf_set_error_suppressed();

    // call the fn in a condition where it generates this error
    char * path = "";
    is_directory(path, 0);

    // turn off error suppression, so we can get the error
    //pf_set_error_not_suppressed();
    
    // retrieve the error and put it in a pstring
    char * error = pf_get_error();
    size_t const error_length = pf_strlen(error);
    PString_t const msg = { .string = error, .length = error_length };

    // set up the expected result
    char const * expected = "Invalid file path length";  
    size_t const expected_length = pf_strlen(expected);

    // did we get the expected result?
    ck_assert_int_eq(1, pstring_contains_char_sub(msg, expected, expected_length));
    
}
END_TEST
