// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../../src/memory/allocator/PFAllocator.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../src/core/define.h"
#include "../../src/core/error.h"
#include "../../src/string/pstring.h"

// -----------------------------------------------------------------------------------------------------------
// PFAllocator FreeList 
// -----------------------------------------------------------------------------------------------------------

START_TEST(enum_EAllocationPolicy_FreeList__is_defined) {
    EAllocationPolicy_FreeList policy;
    ck_assert_ptr_nonnull(&policy);
}
END_TEST



// -----------------------------------------------------------------------------------------------------------
// PFAllocator RedBlackTree 
// -----------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------
// PFAllocator Interface
// -----------------------------------------------------------------------------------------------------------




// fn static fn ptrs -----------------------------------------------------------------------------------------


// pf_malloc
START_TEST(static_fn_ptr_pf_malloc__is_defined) {
    pf_malloc = &malloc;
    ck_assert_ptr_nonnull(pf_malloc);
}
END_TEST

// pf_realloc
START_TEST(static_fn_ptr_pf_realloc__is_defined) {
    pf_realloc = &realloc;
    ck_assert_ptr_nonnull(pf_realloc);
}
END_TEST

// pf_free
START_TEST(static_fn_ptr_pf_free_is_defined) {
    pf_free = &free;
    ck_assert_ptr_nonnull(pf_free);
}
END_TEST


START_TEST(static_fn_ptrs__can_be_used__if_they_are_set_with_stdlib_defaults) {
    pf_malloc = &malloc;
    pf_realloc = &realloc;
    pf_free = &free;

    uint8_t * memory = pf_malloc(32);
    ck_assert_ptr_nonnull(memory);
    memory = pf_realloc(memory, 64);
    ck_assert_ptr_nonnull(memory);
    pf_free(memory);
    memory = NULL;
    ck_assert_ptr_null(memory);
}
END_TEST






// fn pf_allocator_initialize --------------------------------------------------------------------------------

START_TEST(fn_pf_allocator_initialize__is_defined) {
    int32_t(*fptr)(void*, size_t const) = &pf_allocator_initialize;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_null_ptr_to_base_memory) {
    PF_SUPPRESS_ERRORS
    int32_t const ret = pf_allocator_initialize(NULL, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_NULL_PTR);
}
END_TEST

START_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_null_ptr_to_base_memory) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_allocator_initialize(NULL, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Null ptr to base memory";
    size_t const expected_length = pf_strlen(expected_error);

    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST

START_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_invalid_memory_size) {
    size_t const size = 64;
    uint8_t memory[size];
    PF_SUPPRESS_ERRORS
    int32_t const ret = pf_allocator_initialize(memory, 0);
    PF_UNSUPPRESS_ERRORS
    ck_assert_int_eq(ret, PFEC_ERROR_INVALID_LENGTH);
}
END_TEST


START_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_invalid_memory_size) {
    uint8_t memory[64];
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_allocator_initialize(memory, 0);
    PF_UNSUPPRESS_ERRORS

    char const * error_message = pf_get_error();
    size_t const error_length = pf_strlen(error_message);
    PString_t const error = { .string = (char*)error_message, .length = error_length };

    char const expected_error[] = "Invalid memory size";
    size_t const expected_length = pf_strlen(expected_error);

    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected_error, expected_length));
}
END_TEST







