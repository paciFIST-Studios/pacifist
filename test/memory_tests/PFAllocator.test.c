// paciFIST studios. 2025. MIT License

// include
#include <check.h>
#include <pftest_utilities.h>
#include <memory/allocator/PFAllocator.h>

// stdlib
#include <stdlib.h>
#include <stdio.h>
// framework
// engine
#include "core/define.h"
#include "core/error.h"

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
    ck_assert_int_eq(0, EAPFL_POLICY_FIND_BEST);
    ck_assert_int_eq(1, EAPFL_POLICY_FIND_FIRST);
}
END_TEST

// struct PFAllocator_FreeListAllocationHeader_t -------------------------------------------------------------

//START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__is_defined) {
//    PFAllocator_FreeListAllocationHeader_t const header = {0};
//    ck_assert_ptr_nonnull(&header);
//}
//END_TEST
//
//START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_size) {
//    ck_assert_int_eq(24, sizeof(PFAllocator_FreeListAllocationHeader_t));
//}
//END_TEST
//
//START_TEST(struct_PFAllocator_FreeListAllocationHeader_t__has_expected_members) {
//    PFAllocator_FreeListAllocationHeader_t const header = {0};
//    ck_assert_int_eq(0, header.block_size);
//    ck_assert_int_eq(0, header.padding);
//}
//END_TEST

// struct PFAllocator_FreeListNode_t -------------------------------------------------------------------------

START_TEST(struct_PFAllocator_FreeListNode_t__is_defined) {
    PFAllocator_FreeListNode_t const node = {0};
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
    int32_t(*fptr)(PFAllocator_FreeListNode_t const *) = &pf_allocator_free_list_node_get_is_allocated;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_error_code__for_null_ptr_to_node_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__sets_correct_error_message__for_null_ptr_to_node_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_allocated_node) {
    PFAllocator_FreeListNode_t node = {0};
    pf_allocator_free_list_node_set_is_allocated(&node);
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), TRUE);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_is_allocated__returns_correct_value_for_unallocated_node) {
    PFAllocator_FreeListNode_t const node = {0};
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), FALSE);
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

    for (size_t i = 0; i < 31; i++) {
        node.metadata = 16*i;
        ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), FALSE);
        pf_allocator_free_list_node_set_is_allocated(&node);
        ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), TRUE);
    }
    
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

START_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__sets_value_correctly__for_successful_use) {
    PFAllocator_FreeListNode_t node = {0};
    node.metadata |= (size_t) 1 << 63;
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), TRUE);
    pf_allocator_free_list_node_set_is_not_allocated(&node);
    ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), FALSE);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_is_not_allocated__works_correctly_without_distrubing_other_bits__when_called) {
    PFAllocator_FreeListNode_t node = {0};
    // size_t is unsigned, so -1 will wrap around, making it the largest possible number,
    // that number is: all bits are 1
    node.metadata = -1;
    ck_assert_int_eq(node.metadata, ((size_t)-1));

    // the highest bit holds is_allocated information, and it is 1 right now
    pf_allocator_free_list_node_set_is_not_allocated(&node);
    // now, the highest bit should be zero, and all the others are 1

    // this mask, produces a 1 in the highest bit, with all others being zero
#ifdef ENVIRONMENT_64
    size_t const mask = 1ULL << 63;
#else
    size_t const mask 1UL << 31;
#endif

    // we compare against the opposite of the mask, making the highest bit zero,
    // and all other bits 1
    ck_assert_int_eq(node.metadata, ~mask);
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
    size_t const bit_size_block_size = sizeof(size_t) - 5;

    for (size_t i = 1; i < bit_size_block_size; i++) {
        size_t const block_size = 16*i;
        node.metadata = block_size;
        ck_assert_int_eq(node.metadata, block_size);
        pf_allocator_free_list_node_set_is_allocated(&node);
        ck_assert_int_ne(node.metadata, block_size);

        size_t const ret = pf_allocator_free_list_node_get_block_size(&node);
        ck_assert_int_eq(ret, block_size);
    }
}
END_TEST

// fn pf_allocator_free_list_node_set_block_size -------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_node_set_block_size__is_defined) {
    int32_t (*fptr)(PFAllocator_FreeListNode_t*, size_t const) = &pf_allocator_free_list_node_set_block_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_block_size__returns_correct_error_code__for_null_ptr_to_node) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_set_block_size(NULL, 30), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_correct_error_message__for_null_ptr_to_node) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_set_block_size(NULL, 30), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_block_size__returns_correct_error_code__for_too_large_block_size) {
    PFAllocator_FreeListNode_t node = {0};
    PF_SUPPRESS_ERRORS
    // here, -1 is like SIZE_T_MAX
    ck_assert_int_eq(pf_allocator_free_list_node_set_block_size(&node, -1), PFEC_ERROR_OUT_OF_BOUNDS_MEMORY_USE);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_correct_error_message__for_too_large_block_size) {
    PFAllocator_FreeListNode_t node = {0};

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_set_block_size(&node, -1), PFEC_ERROR_OUT_OF_BOUNDS_MEMORY_USE);
    PF_UNSUPPRESS_ERRORS

    char const * expected1 = "Cannot set a block size larget than";
    char const * expected2 = " on current platform! Tried to set block_size=";

    ck_assert_in_error_buffer(expected1);
    ck_assert_in_error_buffer(expected2);
}
END_TEST


START_TEST(fn_pf_allocator_free_list_node_set_block_size__sets_block_size_correctly_and_does_not_disturb_other_bits__when_called) {
    PFAllocator_FreeListNode_t node = {0};

    for (size_t i = 1; i < 31; i++) {
        node.metadata = 0;
        size_t const block_size = 16 * i;
        //printf("\n\n\n");
        //printf("Setting block size: %lu\n", block_size);
        ck_assert_int_eq(pf_allocator_free_list_node_set_block_size(&node, block_size), PFEC_NO_ERROR);
        ck_assert_int_eq(pf_allocator_free_list_node_get_block_size(&node), block_size);
    }


}
END_TEST

// fn pf_allocator_free_list_node_get_allocation_size --------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_node_get_allocation_size__is_defined) {
    size_t(*fptr)(PFAllocator_FreeListNode_t const *) = pf_allocator_free_list_node_get_allocation_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_allocation_size__returns_correct_error_code__for_null_ptr_to_node) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_node_get_allocation_size(NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_allocation_size__sets_correct_error_message__for_null_ptr_to_node) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_node_get_allocation_size(NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_allocation_size__returns_correct_value__for_allocation) {
    size_t const size = 512;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(allocator);
    ck_assert_ptr_nonnull(allocator->head);

    size_t const node1_block_size = pf_allocator_free_list_node_get_block_size(allocator->head);
    // the first node contains everything except for the allocator
    size_t const expected_node1_block_size = size - sizeof(PFAllocator_FreeList_t);
    ck_assert_int_eq(node1_block_size, expected_node1_block_size);

    size_t const node1_alloc_size = pf_allocator_free_list_node_get_allocation_size(allocator->head);
    size_t const expected_node1_alloc_size = size - sizeof(PFAllocator_FreeList_t) - sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(node1_alloc_size , expected_node1_alloc_size);
}


// fn pf_allocator_free_list_node_get_padding ----------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_node_get_padding_is_defined) {
    size_t (*fptr)(PFAllocator_FreeListNode_t const *) = &pf_allocator_free_list_node_get_padding;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_padding__returns_correct_error_code__for_null_ptr_to_node) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_padding(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_padding__sets_correct_error_message__for_null_ptr_to_node) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_node_get_padding(NULL), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Null ptr to PFAllocator_FreeListNode_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_node_get_padding__returns_correct_padding_value__during_correct_use) {
    for (int32_t i = 0; i < 8; i++) {
        PFAllocator_FreeListNode_t node = {0};
        pf_allocator_free_list_node_set_padding(&node, i);
        ck_assert_int_eq(pf_allocator_free_list_node_get_padding(&node), i);
        pf_allocator_free_list_node_set_block_size(&node, 16 > 16*i ? 16 : 16*i);
        ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), FALSE);
        pf_allocator_free_list_node_set_is_allocated(&node);
        ck_assert_int_eq(pf_allocator_free_list_node_get_is_allocated(&node), TRUE);
        ck_assert_int_eq(pf_allocator_free_list_node_get_padding(&node), i);
    }
}
END_TEST

// fn pf_allocator_free_list_node_set_padding ----------------------------------------------------------------
// datumArray data structure


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
    ck_assert_int_eq(allocator.free_memory, 0);
    ck_assert_int_eq(allocator.policy, 0);
}
END_TEST


// fn pf_allocator_free_list_create_with_memory --------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_create_with_memory__is_defined) {
    PFAllocator_FreeList_t*(*fptr)(void*, size_t const) = &pf_allocator_free_list_create_with_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__returns_correct_error_code__for_null_base_memory_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__sets_correct_error_message__for_null_base_memory_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot initialize PFAllocator_FreeList_t without valid base_memory ptr!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__returns_correct_error_code__for_zero_size_param) {
    PF_SUPPRESS_ERRORS
    char memory[64];
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(memory, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__sets_correct_error_message__for_zero_size_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    char memory[64];
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(memory, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot initialize PFAllocator_FreeList_t to zero size!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__returns_null__for_80byte_size_param) {
    size_t const size = 80;
    char memory[size];
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(memory, size));
    PF_UNSUPPRESS_ERRORS
}

START_TEST(fn_pf_allocator_free_list_create_with_memory__sets_correct_error_message__for_80byte_size_param) {
    size_t const size = 80;
    char memory[size];

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_create_with_memory(memory, size));
    PF_UNSUPPRESS_ERRORS
    
    char const * expected = "Cannot initialize PFAllocator_FreeList_t with fewer than 80 bytes!  Suggest 4096 minimum.";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__sets_pf_memory_functions__when_used) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(allocator);

    // these are getting set in the header, but are not null when they get back here
    ck_assert_ptr_nonnull(allocator->pf_malloc); 
    ck_assert_ptr_nonnull(allocator->pf_realloc); 
    ck_assert_ptr_nonnull(allocator->pf_free); 
}
END_TEST

START_TEST(fn_pf_allocator_free_list_create_with_memory__sets_memory_values_correctly) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(allocator);

    ck_assert_ptr_eq(allocator->base_memory, &memory);
    ck_assert_int_eq(allocator->base_memory_size, size);

    size_t const expected_memory_remaining = size - sizeof(PFAllocator_FreeList_t) - sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(allocator->free_memory, expected_memory_remaining);
}
END_TEST




START_TEST(fn_pf_allocator_free_list_create_with_memory__after_initialization_allocator_memory_fns_are_usable) {
    size_t const size = 64;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(allocator);

    size_t const allocated_size = 128;
    size_t const allocated_resize = 256;
    char* allocated_memory = allocator->pf_malloc(allocator, allocated_size);
    ck_assert_ptr_nonnull(allocated_memory);
    char* reallocated_memory = allocator->pf_realloc(allocator, allocated_memory, allocated_resize);
    allocated_memory = NULL;
    ck_assert_ptr_nonnull(reallocated_memory);
    allocator->pf_free(allocator, reallocated_memory);
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
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->base_memory_size = 0;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_zero_owned_memory_in_free_list) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->base_memory_size = 0;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_INVALID_LENGTH);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t has invalid owned memory size!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_malloc) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_malloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_malloc) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_malloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_malloc is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_realloc) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_realloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_realloc) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_realloc = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_realloc is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_correct_error_code__for_null_ptr_to_pf_free) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_free = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__sets_correct_error_message__for_null_ptr_to_pf_free) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->pf_free = NULL;
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_ERROR_NULL_PTR);
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->pf_free is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free_all__returns_no_error__on_successful_use) {
    size_t const size = 128;
    uint8_t memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    ck_assert_ptr_nonnull(allocator);
    ck_assert_int_eq(pf_allocator_free_list_free_all(allocator), PFEC_NO_ERROR);
    size_t const measured_node_block_size = pf_allocator_free_list_node_get_block_size(allocator->head);
    // block-size here includes the node, but not the allocator
    size_t const calculated_node_block_size = size - sizeof(PFAllocator_FreeList_t); 
    ck_assert_int_eq(measured_node_block_size, calculated_node_block_size);
}
END_TEST

// fn pf_allocator_free_list_get_node_by_index ---------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_get_node_by_index__is_defined) {
    void*(*fptr)(PFAllocator_FreeList_t const *, size_t) = &pf_allocator_free_list_node_get_node_by_index;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_node_by_index__returns_null__for_null_allocator_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_node_get_node_by_index(NULL, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_node_by_index__sets_correct_error_message__for_null_allocator_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_node_get_node_by_index(NULL, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t pointer is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_node_by_index__returns_head_node__for_zero_index) {
    size_t const size = 512;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    PFAllocator_FreeListNode_t* node = pf_allocator_free_list_node_get_node_by_index(allocator, 0);
    ck_assert_int_eq((size_t)node, (size_t)allocator->head);
}
END_TEST

START_TEST(fn_pf_allocator_fn_list_get_node_by_index__returns_null__for_out_of_range_index) {
    size_t const size = 512;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_nonnull(pf_allocator_free_list_node_get_node_by_index(allocator, 0));
    ck_assert_ptr_null(pf_allocator_free_list_node_get_node_by_index(allocator, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST


// fn pf_allocator_is_power_of_two ---------------------------------------------------------------------------

START_TEST(fn_pf_allocator_is_power_of_two__is_defined) {
    int32_t(*fptr)(size_t) = &pf_allocator_is_power_of_two;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// fn pf_allocator_should_bisect_memory ----------------------------------------------------------------------

START_TEST(fn_pf_allocator_should_bisect_memory__is_defined) {
    int32_t(*fptr)(size_t, size_t, size_t*) = &pf_allocator_should_bisect_memory;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__returns_false__if_block_size_smaller_than_required_size) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_allocator_should_bisect_memory(1, 2, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__sets_correct_error_message__if_block_size_is_smaller_than_required_size) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(FALSE, pf_allocator_should_bisect_memory(1, 2, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got request to analyze block which is smaller than required size!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__returns_false__if_block_size_equals_required_size) {
    ck_assert_int_eq(FALSE, pf_allocator_should_bisect_memory(2, 2, NULL));
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__returns_false__if_block_size_is_bigger_than_request_but_too_small_for_an_additional_block) {
    size_t const MINIMUM_NODE_ALLOC_MEMORY = 256;
    size_t const block_size = MINIMUM_NODE_ALLOC_MEMORY + sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(FALSE, pf_allocator_should_bisect_memory(block_size, 1, NULL));
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__returns_true__if_the_block_size_is_big_enough) {
    ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(512, 128, NULL));
}
END_TEST

START_TEST(fn_pf_allocator_should_bisect_memory__outputs_correct_value__for_cut_at_offset_param) {
    size_t const MINIMUM_NODE_ALLOC_MEMORY = 256;
    size_t const min_block_size = sizeof(PFAllocator_FreeListNode_t) + MINIMUM_NODE_ALLOC_MEMORY;

    // there is not enough room for another block, so memory should not be bisected
    size_t cut_at_offset = 0;
    size_t block_size = min_block_size + 63;
    size_t required_size = 64;
    ck_assert_int_eq(FALSE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
    ck_assert_int_eq(cut_at_offset, 0);

    // there is exactly enough room for 1 more block, so the memory should be cut, and
    // the cut should happen the next byte after this allocation
    cut_at_offset = 0;
    block_size = min_block_size + 64;
    required_size = 64;
    ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
    ck_assert_int_eq(cut_at_offset, required_size);

    // there is plenty of room, so the memory should be cut, and the cut rounds up
    // to the next closest 16 bytes.
    block_size = 4096;
    for (size_t i = 1; i < 16; i++) {
        required_size = 64 + i;
        ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
        ck_assert_int_eq(cut_at_offset, 80);
    }

    // there is plenty of room, so the memory should be cut, but the allocation is
    // naturally 16 byte aligned
    required_size = 64;
    ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
    ck_assert_int_eq(cut_at_offset, required_size);
    required_size = 80;
    ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
    ck_assert_int_eq(cut_at_offset, required_size);
    required_size = 96;
    ck_assert_int_eq(TRUE, pf_allocator_should_bisect_memory(block_size, required_size, &cut_at_offset));
    ck_assert_int_eq(cut_at_offset, required_size);
}
END_TEST



// fn pf_allocator_free_list_get_allocator_available_memory_size ---------------------------------------------
START_TEST(fn_pf_allocator_free_list_get_allocator_available_memory_size__is_defined) {
    size_t(*fptr)(PFAllocator_FreeList_t *) = pf_allocator_free_list_get_allocator_available_memory_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_allocatr_available_memory_size__returns_correct_error_code__for_null_ptr_to_free_list) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_allocator_available_memory_size(NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_allocator_available_memory_size__sets_correct_error_message__for_null_ptr_to_free_list) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR,  pf_allocator_free_list_get_allocator_available_memory_size(NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t pointer is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_allocator_available_memory_size__returns_correct_error_code__for_invalid_head_ptr_in_free_list) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t fl = {0};
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_allocator_available_memory_size(&fl));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_allocator_available_memory_size__sets_correct_error_message__for_invalid_head_ptr_in_free_list) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t fl = {0};
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_allocator_available_memory_size(&fl));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->head is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

// successfully counts allocated memory in different situations
START_TEST(fn_pf_allocator_free_list_get_allocator_available_memory_size__general_testing_fn) {
    // set up an allocator with enough memory to actually test it out
    size_t const size = 4096;
    void* memory = malloc(size);
    PFAllocator_FreeList_t * allocator = pf_allocator_free_list_create_with_memory(memory, size);

    // test the first node which is created when the allocator is created
    size_t const minimum_allocator_overhead = sizeof(PFAllocator_FreeList_t) + sizeof(PFAllocator_FreeListNode_t);
    size_t const expected_first_node_available_memory = size - minimum_allocator_overhead;
    // After being created, the first node contains all of the memory owned by the allocator,
    // except for: memory of the allocator struct itself, and memory of the first node struct itself.
    size_t const measured_first_node_available_memory = pf_allocator_free_list_get_allocator_available_memory_size(allocator);
    ck_assert_int_eq(measured_first_node_available_memory, expected_first_node_available_memory);

    // test an allocation.  The first allocation sets the first node, to be the alloc size,
    // then it creates a 2nd node, to hold the remaining memory in the allocator
    size_t const alloc0_size = 128;
    void* alloc0 = pf_allocator_free_list_malloc(allocator, alloc0_size);
    ck_assert_ptr_nonnull(alloc0);

    // is the allocation's block size equal to the request size + sizeof(node)?
    PFAllocator_FreeListNode_t const * alloc0_node = pf_allocator_free_list_node_get_node_by_index(allocator, 0);
    size_t const alloc0_node_block_size = pf_allocator_free_list_node_get_block_size(alloc0_node);
    ck_assert_int_eq(alloc0_node_block_size, alloc0_size+sizeof(PFAllocator_FreeListNode_t));

    // after the fist allocation, do we have the amount of leftover memory we expect?
    size_t const expected_first_allocation_available_memory = measured_first_node_available_memory - alloc0_node_block_size;
    size_t const measured_first_allocation_available_memory = pf_allocator_free_list_get_allocator_available_memory_size(allocator);
    ck_assert_int_eq(expected_first_allocation_available_memory, measured_first_allocation_available_memory);

    // in this test, after the first allocation, we also expect the memory to have been bisected
    ck_assert_ptr_nonnull(alloc0_node->next);
    size_t const expected_second_node_block_size = allocator->free_memory + sizeof(PFAllocator_FreeListNode_t);
    size_t const measured_second_node_block_size = pf_allocator_free_list_node_get_block_size(alloc0_node->next);
    ck_assert_int_eq(expected_second_node_block_size, measured_second_node_block_size);

    // todo: maybe more allocations?
    
    free(memory);
}
END_TEST



// fn pf_allocator_free_list_get_memory_overhead_size --------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_get_memory_overhead_size__is_defined) {
    size_t(*fptr)(PFAllocator_FreeList_t const *) = pf_allocator_free_list_get_memory_overhead_size;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST


START_TEST(fn_pf_allocator_free_list_get_memory_overhead_size__returns_correct_error_code__for_null_ptr_to_free_list) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_memory_overhead_size(NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_memory_overhead_size__sets_correct_error_message__for_null_ptr_to_free_list) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR,  pf_allocator_free_list_get_memory_overhead_size(NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t pointer is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_memory_overhead_size__returns_correct_error_code__for_invalid_head_ptr_in_free_list) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t const fl = {0};
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_memory_overhead_size(&fl));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_get_memory_overhead_size__sets_correct_error_message__for_invalid_head_ptr_in_free_list) {
    PF_SUPPRESS_ERRORS
    PFAllocator_FreeList_t const fl = {0};
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_get_memory_overhead_size(&fl));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t->head is unexpectedly null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST


// successfully counts overhead memory in different situations


// fn pf_allocator_free_list_calculate_padding_and_header ----------------------------------------------------
START_TEST(fn_pf_allocator_free_list_calculate_padding__is_defined) {
    size_t(*fptr)(uintptr_t, uintptr_t, size_t const) = &pf_allocator_free_list_calculate_padding;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__returns_correct_error_code__for_null_ptr_arg) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)NULL, 16, 16));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__sets_correct_error_message__for_null_ptr_arg) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)NULL, 16, 16));
    PF_UNSUPPRESS_ERRORS

    char const * message = "Header_address argument is unexpectedly null!";
    ck_assert_in_error_buffer(message);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__returns_correct_error_code__for_ptr_misaligned) {
    char data[128] = {0};
    char* ptr = data + 3;

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)ptr, 16, 16));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__sets_correct_error_message__for_ptr_misaligned) {
    char data[128] = {0};
    char* ptr = data + 3;

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)ptr, 16, 16));
    PF_UNSUPPRESS_ERRORS

    char const * message = "Header_address is not aligned to given alignment!";
    ck_assert_in_error_buffer(message);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__returns_correct_error_code__for_not_power_of_two_alignment) {
    char data[128] = {0};

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)data, 15, 16));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__sets_correct_error_message__for_not_power_of_two_alignment) {
    char const data[128] = {0};

    PF_SUPPRESS_ERRORS
    ck_assert_int_eq((size_t)(-1), pf_allocator_free_list_calculate_padding((uintptr_t)data, 15, 16));
    PF_UNSUPPRESS_ERRORS

    char const * message = "Requested alignment is not a power of two!";
    ck_assert_in_error_buffer(message);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__returns_zero__if_header_is_naturally_aligned_to_boundary) {
    char const data[128] = {0};

    for (size_t i = 0; i < 4; i++) {
        ck_assert_int_eq(0, pf_allocator_free_list_calculate_padding((uintptr_t)data, 16, 16*i));
    }

}
END_TEST

START_TEST(fn_pf_allocator_free_list_calculate_padding__returns_padding_number__when_used_correctly) {
    char data[128] = {0};
    size_t const alignment = 16;

    // check for the mod-zero case
    ck_assert_int_eq(0, pf_allocator_free_list_calculate_padding((uintptr_t)data, alignment, alignment));

    // check for the other cases
    for (size_t i = 1; i < 16; i++) {
        ck_assert_int_eq(alignment-i, pf_allocator_free_list_calculate_padding((uintptr_t)data, alignment, alignment+i));
    }
}



// fn pf_allocator_free_list_find_first ----------------------------------------------------------------------
START_TEST(fn_pf_allocator_free_list_find_first__is_defined) {
    PFAllocator_FreeListNode_t* (*fptr)(
        PFAllocator_FreeList_t const * free_list,
        size_t list_size,
        size_t alignment,
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
        size_t list_size,
        size_t alignment,
        size_t* padding,
        PFAllocator_FreeListNode_t** previous_node
        ) = &pf_allocator_free_list_find_best;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__returns_null__for_null_free_list_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(NULL, 16, 16, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__sets_correct_error_message__for_null_free_list_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(NULL, 16, 16, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got null ptr to PFAllocator_FreeList_t!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__returns_null__for_requested_size_smaller_than_node_size) {
    PFAllocator_FreeList_t const allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(&allocator, 15, 16, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__sets_correct_error_message__for_requested_size_smaller_than_node_size) {
    PFAllocator_FreeList_t const allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(&allocator, 15, 16, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got request for too-small allocation!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__returns_null__for_non_power_of_two_alignment) {
    PFAllocator_FreeList_t const allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(&allocator, sizeof(PFAllocator_FreeListNode_t), 17, NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_find_best__sets_correct_error_message__for_non_power_of_two_alignment) {
    PFAllocator_FreeList_t const allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_find_best(&allocator, sizeof(PFAllocator_FreeListNode_t), 17, NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got request for non-power-of-two alignment!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

// fn works w/o optional params
// fn works w/ optional params

// returns null if all nodes are allocated
// returns null if unallocated nodes cannot hold allocation request
// returns best fitting node, not first fitting node
// correctly returns best fit node, if it's last
// correctly returns best fit node, if it's first








// fn pf_allocator_free_list_malloc --------------------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_malloc__is_defined) {
    void*(*fptr)(PFAllocator_FreeList_t*, size_t const) = pf_allocator_free_list_malloc;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__for_null_allocator_param) {
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(NULL, 1));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__sets_coorect_error_message__for_null_allocator_param) {
    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(NULL, 1));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot allocate with null ptr to allocator!";
    ck_assert_in_error_buffer(expected);
}

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__for_requested_memory_size_of_zero) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 0));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__sets_correct_error_message__for_requested_memory_size_of_zero) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 0));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Cannot allocate 0-bytes!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__for_null_ptr_to_base_memory) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->base_memory = NULL;

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 32));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__sets_correct_error_message__for_null_ptr_to_base_memory) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->base_memory = NULL;

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 32));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t is in a bad state! Ptr to base memory was NULL!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__if_it_cannot_fulfill_memory_request) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    PF_SUPPRESS_ERRORS
    // this will always fail, b/c the allocator struct is inside of the memory it's been given
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, size));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__sets_correct_error_message__if_it_cannot_fulfill_memmory_request) {
    size_t const size = 128;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, size));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "PFAllocator_FreeList_t does not have enough memory to fulfill this memory request!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__if_it_cannot_find_a_node_for_allocation) {
    size_t const size = 512;
    char memory[size];
    memset(memory, 0, size);
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->head = NULL;

    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 128));
    PF_UNSUPPRESS_ERRORS
}

START_TEST(fn_pf_allocator_free_list_malloc__sets_correct_error_message__if_it_cannot_find_a_node_for_allocation) {
    size_t const size = 512;
    char memory[size];
    memset(memory, 0, size);
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    allocator->head = NULL;

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 128));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Could not find a free node for allocation!";
    ck_assert_in_error_buffer(expected);
}

START_TEST(fn_pf_allocator_free_list_malloc__returns_null__if_the_node_it_finds_for_allocation_is_already_allocated) {
    size_t const size = 512;
    char memory[size];
    memset(memory, 0, size);
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    pf_allocator_free_list_node_set_is_allocated(allocator->head);
    
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 128));
    PF_UNSUPPRESS_ERRORS
}

START_TEST(fn_pf_allocator_free_list_malloc__sets_correct_error_message__if_the_node_it_finds_for_allocation_is_already_allocated) {
    size_t const size = 512;
    char memory[size];
    memset(memory, 0, size);
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    pf_allocator_free_list_node_set_is_allocated(allocator->head);

    pf_clear_error();
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_null(pf_allocator_free_list_malloc(allocator, 128));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Tried to allocate memory from a node which is already allocated!";
    ck_assert_in_error_buffer(expected);
}


START_TEST(fn_pf_allocator_free_list_malloc__returns_memory__if_it_gives_out_the_last_byte_during_request) {
    size_t const size = 128;
    char memory[size];

    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    size_t const memory_request = allocator->free_memory - sizeof(PFAllocator_FreeListNode_t);
    PF_SUPPRESS_ERRORS
    ck_assert_ptr_nonnull(pf_allocator_free_list_malloc(allocator, memory_request));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

// pf_allocator_free_list_realloc ----------------------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_realloc__is_defined) {
    void*(*fptr)(PFAllocator_FreeList_t*, void*, size_t) = pf_allocator_free_list_realloc;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

// pf_allocator_free_list_free -------------------------------------------------------------------------------

START_TEST(fn_pf_allocator_free_list_free__is_defined) {
    int32_t(*fptr)(PFAllocator_FreeList_t*, void*) = pf_allocator_free_list_free;
    ck_assert_ptr_nonnull(fptr);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free__returns_correct_error_code__for_null_ptr_to_allocator) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_free(NULL, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free__sets_correct_error_message__for_null_ptr_to_allocator) {
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_free(NULL, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Got null ptr to allocator!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free__returns_correct_error_code__for_null_ptr_to_vacated_memory) {
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_free(&allocator, NULL));
    PF_UNSUPPRESS_ERRORS
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free__sets_correct_error_message__for_null_ptr_to_vacated_memory) {
    PFAllocator_FreeList_t allocator = {0};
    PF_SUPPRESS_ERRORS
    ck_assert_int_eq(PFEC_ERROR_NULL_PTR, pf_allocator_free_list_free(&allocator, NULL));
    PF_UNSUPPRESS_ERRORS

    char const * expected = "Was asked to free ptr to null!";
    ck_assert_in_error_buffer(expected);
}
END_TEST

START_TEST(fn_pf_allocator_free_list_free___) {
    size_t const size = 512;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);
    // verify the allocator seems set up correctly
    ck_assert_int_eq(size, allocator->base_memory_size);
    size_t const expected_free_memory_size = size - sizeof(PFAllocator_FreeList_t) - sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(expected_free_memory_size, allocator->free_memory);

    // do an allocation
    size_t const alloc1_size_request =  128;
    void* alloc1 = allocator->pf_malloc(allocator, alloc1_size_request);
    ck_assert_ptr_nonnull(alloc1);

    for (size_t i = 0; i < alloc1_size_request; i++) {
        char* write = (char*)((uintptr_t)alloc1 + i);
        *write = 'X';
    }

    
    // verify the allocator seems to have allocated the correct amount
    size_t const expected_free_memory_after_alloc = expected_free_memory_size - alloc1_size_request - sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(expected_free_memory_after_alloc, allocator->free_memory);

    // verify the allocator seems to have allocated, starting at the correct address
    uintptr_t const expected_memory_address = (uintptr_t)allocator->head;
    ck_assert_int_eq(expected_memory_address, (uintptr_t)alloc1);
    uintptr_t const expected_node_address = (uintptr_t)alloc1 - sizeof(PFAllocator_FreeListNode_t);
    ck_assert_int_eq(expected_node_address, (uintptr_t)allocator->head);

    
    allocator->pf_free(allocator, alloc1);
}
END_TEST



// PFAllocator_FreeList_t general usage tests ----------------------------------------------------------------

START_TEST(fn_PFAllocator_FreeList_t__correctly_creates_nodes__during_usage) {
    size_t const size = 512;
    char memory[size];
    PFAllocator_FreeList_t* allocator = pf_allocator_free_list_create_with_memory(memory, size);

    size_t const alloc1_size_request = 32;
    void* alloc1 = allocator->pf_malloc(allocator, alloc1_size_request);
    ck_assert_ptr_nonnull(alloc1);

    //size_t const alloc1_base_offset = (size_t)alloc1 - sizeof(PFAllocator_FreeListNode_t);
    //PFAllocator_FreeListNode_t const * alloc1_base = (void*)alloc1 + alloc1_base_offset;
    //size_t const alloc1_metadata_size = pf_allocator_free_list_node_get_block_size(alloc1_base);
    //ck_assert_int_eq(32, alloc1_metadata_size);
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







