// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include "../pftest_utilities.h"
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

// EAllocationPolicy_FreeList --------------------------------------------------------------------------------

START_TEST(enum_EAllocationPolicy_FreeList__is_defined) {
    EAllocationPolicy_FreeList policy;
    ck_assert_ptr_nonnull(&policy);
}
END_TEST

START_TEST(enum_EAllocationPolicy_FreeList__has_expected_values) {
    ck_assert_int_eq(0, EAPFL_POLICY_FIND_FIRST);
    ck_assert_int_eq(1, EAPFL_POLICY_FIND_BEST);
}
END_TEST

// struct PFAllocator_FreeListAllocationHeader_t -------------------------------------------------------------

START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__is_defined) {
    PFAllocator_FreeListAllocationHeader_t header;
    ck_assert_ptr_nonnull(&header);
}
END_TEST

START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_size) {
    ck_assert_int_eq(24, sizeof(PFAllocator_FreeListAllocationHeader_t));
}
END_TEST

START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_members) {
    PFAllocator_FreeListAllocationHeader_t const header = {0};
    ck_assert_int_eq(0, header.block_size);
    ck_assert_int_eq(0, header.padding);
}
END_TEST

// struct PFAllocator_FreeListNode_t -------------------------------------------------------------------------

START_TEST(struct_PFAllocator_FreeListNode_t__is_defined) {
    PFAllocator_FreeListNode_t const node;
    ck_assert_ptr_nonnull(&node);
}
END_TEST

START_TEST(struct_PFAllocator_FreeListNode_t__has_expected_size) {
   ck_assert_int_eq(16, sizeof(PFAllocator_FreeListNode_t)); 
}
END_TEST

START_TEST(struct_PFAllocator_FreeListNode_t__has_expected_members) {
    PFAllocator_FreeListNode_t const node = {0};
    ck_assert_ptr_null(node.next);
    ck_assert_int_eq(0, node.metadata);
}
END_TEST

// fn pf_allocator_free_list_node_is_allocated ---------------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_node_is_allocated__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeListNode_t const *) = &pf_allocator_free_list_node_is_allocated;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_error_code__for_null_ptr_to_node_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__sets_correct_error_message__for_null_ptr_to_node_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_allocated_node) {
    PFAllocator_FreeListNode_t node = {0};
    node.metadata |= (size_t) 1 << 63;
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), TRUE);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_unallocated_node) {
    PFAllocator_FreeListNode_t const node = {0};
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), FALSE);
}
END_TEST

// fn pf_allocator_free_list_node_set_is_allocated -----------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_node_set_is_allocated__is_defined) {
    void(*fptr)(PFAllocator_FreeListNode_t*) = &pf_allocator_free_list_node_set_is_allocated;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_is_allocated__sets_correct_error_message__for_null_ptr_to_node_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    pf_allocator_free_list_node_set_is_allocated(NULL);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_is_allocated__sets_value_correctly__for_successful_use) {
    PFAllocator_FreeListNode_t node = {0};
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), FALSE);
    pf_allocator_free_list_node_set_is_allocated(&node);
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), TRUE);
}
END_TEST

// fn pf_allocator_free_list_node_set_is_not_allocated ------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__is_defined) {
    void(*fptr)(PFAllocator_FreeListNode_t*) = &pf_allocator_free_list_node_set_is_not_allocated;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__sets_correct_error_message__for_null_ptr_to_node) {
    PF_SUPPRESS_ERRORS
    pf_allocator_free_list_node_set_is_allocated(NULL);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_Free_list_node_set_is_not_allocated__sets_value_correctly__for_successful_use) {
    PFAllocator_FreeListNode_t node = {0};
    node.metadata |= (size_t) 1 << 63;
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), TRUE);
    pf_allocator_free_list_node_set_is_not_allocated(&node);
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), FALSE);
}
END_TEST

// fn pf_allocator_free_list_node_get_block_size -------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_node_get_block_size__is_defined) {
   size_t(*fptr)(PFAllocator_FreeListNode_t const *) = &pf_allocator_free_list_node_get_block_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_block_size__returns_correct_error_code__for_null_ptr_to_node) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_block_size(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_block_size__sets_correct_error_message__for_null_ptr_to_node) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_block_size(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_block_size__returns_correct_block_size__without_disturbing_other_metadata) {
    PFAllocator_FreeListNode_t node = {0};
    // set block size as 64
    size_t const block_size = 64;
    node.metadata = block_size;
    pf_allocator_free_list_node_set_is_allocated(&node);
    ck_assert_int_ne(node.metadata, 64);
    size_t const ret = pf_allocator_free_list_node_get_block_size(&node);
    ck_assert_int_eq(ret, block_size);
    ck_assert_int_eq(pf_allocator_free_list_node_is_allocated(&node), TRUE);
}
END_TEST

// fn pf_allocator_free_list_node_set_block_size -------------------------------------------------------------




// struct PFAllocator_FreeList_t -----------------------------------------------------------------------------

START_TEST(struct_PFAllocator_FreeList_t__is_defined) {
    PFAllocator_FreeList_t const allocator = {0};
    ck_assert_ptr_nonnull(&allocator);
}
END_TEST

START_TEST(struct_PFAllocator_FreeList_t__has_expected_size) {
    ck_assert_int_eq(64, sizeof(PFAllocator_FreeList_t));
}
END_TEST

START_TEST(struct_PFAllocator_FreeList_t__has_expected_members) {
    PFAllocator_FreeList_t const allocator = {0};
    ck_assert_ptr_null(allocator.base_memory);
    ck_assert_ptr_null(allocator.head);
    ck_assert_int_eq(allocator.base_memory_size, 0);
    ck_assert_int_eq(allocator.used_memory, 0);
    ck_assert_int_eq(allocator.policy, 0);
}
END_TEST


// fn pf_allocator_free_list_initialize ----------------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_initialize__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeList_t*, void*, size_t const) = &pf_allocator_free_list_initialize;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_null_free_list_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_initialize(NULL, NULL, 0), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_null_free_list_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_initialize(NULL, NULL, 0), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot initialize PFAllocator_FreeList_t without valid ptr to free list allocator!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_null_base_memory_param) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, NULL, 0), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_null_base_memory_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, NULL, 0), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot initialize PFAllocator_FreeList_t without valid base_memory ptr!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__returns_correct_error_code__for_zero_size_param) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    char memory[10];
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, memory, 0), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__sets_correct_error_message__for_zero_size_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    char memory[10];
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, memory, 0), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot initialize PFAllocator_FreeList_t to zero size!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


START_TEST(fn_pf_allocator_free_list_initialize__sets_pf_memory_functions__when_used) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 64;
    char memory[size];
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, memory, size), 0);

    // these are getting set in the header, but are null when they get back here
    ck_assert_ptr_nonnull(allocator.pf_malloc); 
    ck_assert_ptr_nonnull(allocator.pf_realloc); 
    ck_assert_ptr_nonnull(allocator.pf_free); 
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__sets_memory_values_correctly) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 64;
    char memory[size];
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, memory, size), 0);

    ck_assert_ptr_eq(allocator.base_memory, memory);
    ck_assert_int_eq(allocator.base_memory_size, size);
    ck_assert_int_eq(allocator.used_memory, 0);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_initialize__after_initialization_allocator_memory_fns_are_usable) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 64;
    char memory[size];
    ck_assert_int_eq(pf_allocator_free_list_initialize(&allocator, memory, size), 0);

    size_t const allocated_size = 128;
    size_t const allocated_resize = 256;
    char* allocated_memory = allocator.pf_malloc(allocated_size);
    ck_assert_ptr_nonnull(allocated_memory);
    char* reallocated_memory = allocator.pf_realloc(allocated_memory, allocated_resize);
    allocated_memory = NULL;
    ck_assert_ptr_nonnull(reallocated_memory);
    allocator.pf_free(reallocated_memory);
    reallocated_memory = NULL;
}
END_TEST


// fn pf_allocator_free_list_free_all ------------------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_free_all__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeList_t*) = pf_allocator_free_list_free_all;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t* pointer is null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_base_memory_ptr_in_free_list) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST
START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_base_memory_ptr_in_free_list) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t allocator = {0};
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t has no initialized base memory!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_zero_owned_memory_in_free_list) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.base_memory_size = 0;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_zero_owned_memory_in_free_list) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.base_memory_size = 0;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t has invalid owned memory size!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_malloc) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_malloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_malloc) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_malloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_malloc is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_realloc) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_realloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_realloc) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_realloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_realloc is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_free) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_free = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_free) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    allocator.pf_free = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_free is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__when_free_list_allocator_lives_inside_its_own_base_memory) {
    size_t const size = 128;
    uint8_t memory[size];
    PFAllocator_FreeList_t* allocator = (PFAllocator_FreeList_t*)memory;
    ck_assert_ptr_nonnull(allocator);
    pf_allocator_free_list_initialize(allocator, memory, size);
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_NO_ERROR);
    // 128 - 64 (allocator) - 16 (node header) = 48
    ck_assert_int_eq(allocator->head->metadata, 48);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__when_free_list_allocator_lives_outside_its_own_base_memory) {
    PFAllocator_FreeList_t allocator = {0};
    size_t const size = 128;
    char memory[size];
    pf_allocator_free_list_initialize(&allocator, memory, size);
    ck_assert_int_eq(pf_allocator_free_list_free_all(&allocator), PFEC_NO_ERROR);
    // 128 - 16 (node header) = 112
    ck_assert_int_eq(allocator.head->metadata, 112);
}
END_TEST


// fn pf_allocator_is_power_of_two ---------------------------------------------------------------------------

START_TEST(fn_pf_allocator_is_power_of_two__is_defined) {
    int32_t(*fptr)(size_t const) = &pf_allocator_is_power_of_two;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_allocator_free_list_calculate_padding_and_header ----------------------------------------------------
START_TEST(fn_pf_allocator_free_list_calculate_padding_and_header__is_defined) {
    size_t(*fptr)(uintptr_t, uintptr_t, size_t const) = &pf_allocator_free_list_calculate_padding_and_header;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_allocator_free_list_find_first ----------------------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_find_first__is_defined) {
    PFAllocator_FreeListNode_t* (*fptr)(
        PFAllocator_FreeList_t const * free_list,
        size_t const list_size,
        size_t const alignment,
        size_t * padding,
        PFAllocator_FreeListNode_t** previous_node
    ) = &pf_allocator_free_list_find_first;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


// fn pf_allocator_free_list_find_best -----------------------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_find_best__is_defined) {
    PFAllocator_FreeListNode_t* (*fptr)(
        PFAllocator_FreeList_t const * free_list,
        size_t const list_size,
        size_t const alignment,
        size_t * padding,
        PFAllocator_FreeListNode_t** previous_node
        ) = &pf_allocator_free_list_find_best;
    ck_assert_ptr_nonnull(fptr);
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
//START_TEST(static_fn_ptr_pf_malloc__is_defined) {
//    pf_malloc = &malloc;
//    ck_assert_ptr_nonnull(pf_malloc);
//}
//END_TEST
//
//// pf_realloc
//START_TEST(static_fn_ptr_pf_realloc__is_defined) {
//    pf_realloc = &realloc;
//    ck_assert_ptr_nonnull(pf_realloc);
//}
//END_TEST
//
//// pf_free
//START_TEST(static_fn_ptr_pf_free_is_defined) {
//    pf_free = &free;
//    ck_assert_ptr_nonnull(pf_free);
//}
//END_TEST
//
//
//START_TEST(static_fn_ptrs__can_be_used__if_they_are_set_with_stdlib_defaults) {
//    pf_malloc = &malloc;
//    pf_realloc = &realloc;
//    pf_free = &free;
//
//    uint8_t * memory = pf_malloc(32);
//    ck_assert_ptr_nonnull(memory);
//    memory = pf_realloc(memory, 64);
//    ck_assert_ptr_nonnull(memory);
//    pf_free(memory);
//    memory = NULL;
//    ck_assert_ptr_null(memory);
//}
//END_TEST






//// fn pf_allocator_initialize --------------------------------------------------------------------------------
//
//START_TEST(fn_pf_allocator_initialize__is_defined) {
//    int32_t(*fptr)(void*, size_t const) = &pf_allocator_initialize;
//    ck_assert_ptr_nonnull(fptr);
//}
//END_TEST
//
//
//START_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_null_ptr_to_base_memory) {
//    PF_SUPPRESS_ERRORS
//    int32_t const ret = pf_allocator_initialize(NULL, 0);
//    PF_UNSUPPRESS_ERRORS
//    ck_assert_int_eq(ret, PFEC_ERROR_NULL_PTR);
//}
//END_TEST
//
//START_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_null_ptr_to_base_memory) {
//    pf_clear_error();
//    PF_SUPPRESS_ERRORS
//    pf_allocator_initialize(NULL, 0);
//    PF_UNSUPPRESS_ERRORS
//
//    char const * error_message = pf_get_error();
//    size_t const error_length = pf_strlen(error_message);
//    PString_t const error = { .string = (char*)error_message, .length = error_length };
//
//    char const expected_error[] = "Null ptr to base memory";
//    size_t const expected_length = pf_strlen(expected_error);
//
//    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected_error, expected_length));
//}
//END_TEST
//
//START_TEST(fn_pf_allocator_initialize__returns_correct_error_code__for_invalid_memory_size) {
//    size_t const size = 64;
//    uint8_t memory[size];
//    PF_SUPPRESS_ERRORS
//    int32_t const ret = pf_allocator_initialize(memory, 0);
//    PF_UNSUPPRESS_ERRORS
//    ck_assert_int_eq(ret, PFEC_ERROR_INVALID_LENGTH);
//}
//END_TEST
//
//
//START_TEST(fn_pf_allocator_initialize__sets_correct_error_message__for_invalid_memory_size) {
//    uint8_t memory[64];
//    pf_clear_error();
//    PF_SUPPRESS_ERRORS
//    pf_allocator_initialize(memory, 0);
//    PF_UNSUPPRESS_ERRORS
//
//    char const * error_message = pf_get_error();
//    size_t const error_length = pf_strlen(error_message);
//    PString_t const error = { .string = (char*)error_message, .length = error_length };
//
//    char const expected_error[] = "Invalid memory size";
//    size_t const expected_length = pf_strlen(expected_error);
//
//    ck_assert_int_eq(TRUE, pf_pstring_contains_char_sub(error, expected_error, expected_length));
//}
//END_TEST







