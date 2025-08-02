// paciFIST studios. 2025. MIT License

// include
#include "../allocator/PFAllocator.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../core/assert.h"
#include "../../core/define.h"
#include "../../core/error.h"

static PFAllocator_t s_allocator = {0};


// -----------------------------------------------------------------------------------------------------------
// PFAllocator_FreeList_t
// -----------------------------------------------------------------------------------------------------------


void* pf_allocator_provided_memory_free_list_allocator(size_t const size) {
    if (s_allocator.memory.base_memory == NULL || s_allocator.memory.owned_memory == 0){
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator is not initialized!");
        return NULL;
    }
    if (s_allocator.memory.used_memory > s_allocator.memory.owned_memory) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator is using memory it doesn't own!");
        return NULL;
    }

    return NULL;
}



int32_t pf_allocator_free_list_initialize(PFAllocator_FreeList_t* pf_free_list, void* base_memory, size_t const size) {
    if (pf_free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t without valid ptr to free list allocator!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t without valid base_memory ptr!")
        return PFEC_ERROR_NULL_PTR;
    }
    if (size == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t to zero size!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    
    pf_malloc = &malloc;
    pf_realloc = &realloc;
    pf_free = &free;

    pf_free_list->base_memory = base_memory;
    pf_free_list->owned_memory = size;

    return 0;
}





void pf_allocator_free_list_free_all(PFAllocator_FreeList_t* pf_free_list) {
    if (pf_free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator is not initialized!");
        return;
    }

    // we're not overwriting the beginning of the allocation, b/c this is where
    // the free list struct itself lives
    size_t const pf_free_list_size = sizeof(PFAllocator_FreeList_t);
    void* pf_free_list_usable_memory_start = (void*)((uint64_t)pf_free_list->base_memory + pf_free_list_size);

    // zero out the whole thing immediately, and hope it crashes, if anyone is still using it
    size_t const owned_memory_size = pf_free_list->owned_memory;
    for (size_t i = pf_free_list_size; i < owned_memory_size; i++) {
        uint8_t* ptr = (void*)(pf_free_list->owned_memory + i);
        *ptr = 0;
    }

    // memory is now fresh, so just start rebuilding
    pf_free_list->used_memory = 0;
    PFAllocator_FreeListNode_t* first_node = pf_free_list_usable_memory_start;
    first_node->block_size = pf_free_list->owned_memory - pf_free_list_size;
    first_node->next = NULL;
    pf_free_list->head = first_node;
}

int32_t pf_allocator_is_power_of_two(size_t const size) {
    // we're not checking against zero, b/c size_t can't be negative
    return (size & (size - 1)) == 0;
}


size_t pf_allocator_free_list_calculate_padding_and_header(
    uintptr_t ptr,
    uintptr_t alignment,
    size_t const header_size)
{
    PF_ASSERT(pf_allocator_is_power_of_two(alignment));

    uintptr_t padding = 0;

    uintptr_t const modulo = ptr & (alignment - 1);
    if (modulo != 0) {
        padding = alignment - modulo;
    }

    uintptr_t needed_space = (uintptr_t)header_size;
    if (padding < needed_space) {
        needed_space -= padding;
        if ((needed_space & (alignment - 1)) != 0) {
            padding += alignment * (1 + (needed_space / alignment));
        } else {
            padding += alignment * (needed_space / alignment);
        }
    }

    return (size_t)padding;
}




PFAllocator_FreeListNode_t* pf_allocator_free_list_find_first(
    PFAllocator_FreeList_t const * free_list,
    size_t const requested_sz,
    size_t const alignment,
    size_t * out_padding,
    PFAllocator_FreeListNode_t** out_previous_node)
{
    PFAllocator_FreeListNode_t* node = free_list->head;
    PFAllocator_FreeListNode_t* prev_node = NULL;

    size_t padding = 0;
    size_t const header_size = sizeof(PFAllocator_FreeListAllocationHeader_t);
    
    while (node != NULL) {
        padding = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const required_size = requested_sz + padding;
        if (node->block_size >= required_size) {
            break;
        }

        prev_node = node;
        node = node->next;
    }

    if (out_padding){ *out_padding = padding; }
    if (out_previous_node){ *out_previous_node = prev_node; }

    return node;
}


PFAllocator_FreeListNode_t* pf_allocator_free_list_find_best(
    PFAllocator_FreeList_t const * free_list,
    size_t const requested_sz,
    size_t const alignment,
    size_t* out_padding,
    PFAllocator_FreeListNode_t** out_previous_node)
{
    // the difference in size, between the requested size, and the discovered size
    // this algorithm takes a size_t, with a 1 in every bit, and progressively
    // finds smaller values as it scans the allocator's backing memory.
    // after all the backing memory is scanned, the smallest viable block
    // of memory is returned
    size_t smallest_diff_space = ~(size_t)0;

    PFAllocator_FreeListNode_t* node = free_list->head;
    PFAllocator_FreeListNode_t* prev_node = NULL;
    PFAllocator_FreeListNode_t* best_node = NULL;

    size_t padding = 0;
    size_t const header_size = sizeof(PFAllocator_FreeListAllocationHeader_t);

    while (node != NULL) {
        padding = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const required_size = requested_sz + padding;
        // technically, we are losing part of the information where diff_space would be
        // negative.  However, in this case, required_size would be greater than the
        // block size, so we are still checking against that in the first logical
        // clause here
        size_t const diff_space = node->block_size - requested_sz;
        if (node->block_size >= required_size && (diff_space < smallest_diff_space)) {
            best_node = node;
            smallest_diff_space = diff_space;
        }

        prev_node = node;
        node = node->next;
    }

    if (out_padding){ *out_padding = padding;}
    if (out_previous_node){ *out_previous_node = prev_node; }

    return best_node;
}




// -----------------------------------------------------------------------------------------------------------
// PFAllocator_RedBlackTree_t
// -----------------------------------------------------------------------------------------------------------

int32_t pf_allocator_red_black_tree_initialize(void* base_memory, size_t const size) {
    pf_malloc = &malloc;
    pf_realloc = &realloc;
    pf_free = &free;
    return 0;
}


void* pf_provided_memory_red_black_tree_allocator_allocator(size_t const size) {
    return NULL;
}





// -----------------------------------------------------------------------------------------------------------
// PFAllocator_t Interface
// -----------------------------------------------------------------------------------------------------------


//void * pf_malloc(size_t const size) {
//    if (size == 0) {
//        PF_LOG_ERROR(PF_ALLOCATOR, "Invalid memory size");
//        return NULL;
//    }
//    if (s_allocator.memory.base_memory == NULL || s_allocator.memory.owned_memory == 0){
//        PF_LOG_CRITICAL(PF_ALLOCATOR, "Allocator is not initialized!");
//        return NULL;
//    }
//    if (s_allocator.memory.used_memory + size >= s_allocator.memory.owned_memory) {
//        PF_LOG_CRITICAL(PF_ALLOCATOR, "Allocator out of memory!");
//        return NULL;
//    }
//
//    //return pf_provided_memory_allocator(size);
//    return NULL;
//}
//
//
//void * pf_realloc(void* ptr, size_t const size) {
//    return NULL;
//}
//
//
//void pf_free(void* ptr) {
//    
//}



//void* pf_provided_memory_allocator(PFAllocator_t const * allocator, size_t const size) {
//    if (size == 0) {
//        PF_LOG_ERROR(PF_ALLOCATOR, "Invalid memory size");
//        return NULL;
//    }
//
//#ifdef USE_FREE_LIST_IMPLEMENTATION
//    return pf_provided_memory_free_list_allocator(size);
//#else
//    return pf_provided_memory_red_black_tree_allocator(size)
//#endif
//
//    
//    //size_t const allocation_size = size + sizeof(PFAllocator_t);
//    //if (s_allocator.memory.used_memory + allocation_size >= s_allocator.memory.owned_memory) {
//    //    PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator has too little memory to complete allocation!");
//    //    return NULL;
//    //}
//
//
//    //PFAllocationHeader_t* prev_header; /* search a linked list (a tree?) for previous_header */
//    //PFAllocationHeader_t* this_header = { .next_header = NULL, .prev_header = prev_header, .is_allocated = 0x1 };
//    //prev_header->next_header = this_header;
//
//    //size_t const header_size = sizeof(PFAllocationHeader_t);
//    //
//    //void* ptr = s_allocator.memory.base_memory + s_allocator.memory.used_memory;
//    //
//    //memset(ptr, 0, header_size);
//    //ptr = memcpy(ptr, this_header, header_size);
//    //
//    //s_allocator.memory.used_memory += allocation_size;
//
//    //return ptr;
//    return NULL;
//}



int32_t pf_allocator_initialize(void * base_memory, size_t const size) {
    if (base_memory == NULL) {
        PF_LOG_ERROR(PF_ALLOCATOR, "Null ptr to base memory");
        return PFEC_ERROR_NULL_PTR;
    }
    if (size == 0) {
        PF_LOG_ERROR(PF_ALLOCATOR, "Invalid memory size");
        return PFEC_ERROR_INVALID_LENGTH;
    }

#ifdef USE_FREE_LIST_IMPLEMENTATION
    return pf_allocator_free_list_initialize(&s_allocator.memory, base_memory, size);
#else
    return pf_allocator_red_black_tree_initialize(base_memory, size);
#endif
}





