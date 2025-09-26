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

   PF_UNSUPPRESS_ERRORS
}
END_TEST

// fn pf_os_path_exists_pstr_linux ---------------------------------------------------------------------------


// fn pf_os_file_exists_cstr_linux ---------------------------------------------------------------------------
// fn pf_os_file_exists_pstr_linux ---------------------------------------------------------------------------


// fn pf_os_directory_exists_cstr_linux ---------------------------------------------------------------------------
// fn pf_os_directory_exists_pstr_linux ---------------------------------------------------------------------------

// fn pf_os_file_size_cstr_linux ---------------------------------------------------------------------------
// fn pf_os_file_size_pstr_linux ---------------------------------------------------------------------------



