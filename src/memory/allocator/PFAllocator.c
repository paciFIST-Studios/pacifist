// paciFIST studios. 2025. MIT License

// include
#include "../allocator/PFAllocator.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../core/assert.h"
#include "../../core/debug.h"
#include "../../core/define.h"
#include "../../core/error.h"

// Defines
#define BITS_PER_BYTE 8

#define FREE_LIST_32BIT_MAX_BLOCK_SIZE 0x7FFFFFF
#define FREE_LIST_64BIT_MAX_BLOCK_SIZE 0x7FFFFFFFFFFFFFF

// these masks are pre-computed, b/c it's just too complicated to do it at runtime
// we can do it at runtime, but the number of bits used this way aren't going to
// change very often (if ever), so it's just faster and easier to pre-compute
#define FREE_LIST_NODE_METADATA_BITS 5
#define FREE_LIST_NODE_METADATA_32BIT_MASK_IS_ALLOCATED 0x80000000
#define FREE_LIST_NODE_METADATA_64BIT_MASK_IS_ALLOCATED 0x8000000000000000
#define FREE_LIST_NODE_METADATA_32BIT_MASK_PADDING 0x78000000
#define FREE_LIST_NODE_METADATA_64BIT_MASK_PADDING 0x7800000000000000
#define FREE_LIST_NODE_METADATA_32BIT_MASK_BLOCK_SIZE 0x7FFFFFF
#define FREE_LIST_NODE_METADATA_64BIT_MASK_BLOCK_SIZE 0x7FFFFFFFFFFFFFF

#ifdef ENVIRONMENT_64
#define FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED FREE_LIST_NODE_METADATA_64BIT_MASK_IS_ALLOCATED
#define FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE FREE_LIST_NODE_METADATA_64BIT_MASK_BLOCK_SIZE
#define FREE_LIST_NODE_METADATA__MASK_PADDING FREE_LIST_NODE_METADATA_64BIT_MASK_PADDING
#else
#define FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED FREE_LIST_NODE_METADATA_32BIT_MASK_IS_ALLOCATED
#define FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE FREE_LIST_NODE_METADATA_32BIT_MASK_BLOCK_SIZE
#define FREE_LIST_NODE_METADATA__MASK_PADDING FREE_LIST_NODE_METADATA_32BIT_MASK_PADDING
#endif

// -----------------------------------------------------------------------------------------------------------
// PFAllocator_FreeList_t
// -----------------------------------------------------------------------------------------------------------


int32_t pf_allocator_free_list_node_is_allocated(PFAllocator_FreeListNode_t const * node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }

    return (node->metadata & FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED) > 0;
}

void pf_allocator_free_list_node_set_is_allocated(PFAllocator_FreeListNode_t * node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return;
    }
    
    node->metadata |= FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED;
}

void pf_allocator_free_list_node_set_is_not_allocated(PFAllocator_FreeListNode_t *node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return;
    }
    node->metadata &= ~FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED;
}

size_t pf_allocator_free_list_node_get_block_size(PFAllocator_FreeListNode_t const * node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }
    return (node->metadata & FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE);
}

int32_t pf_allocator_free_list_node_set_block_size(PFAllocator_FreeListNode_t * node, size_t const block_size) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }

    if (block_size < FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE) {
        node->metadata |= (block_size & FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE);
        return PFEC_NO_ERROR;
    }

    // TODO: make a variadic logging macro or something
    char const * error_message_base = "Cannot set a block size larget than %lu on current platform! Tried to set block_size=%lu";
    size_t const error_message_base_length = pf_strlen(error_message_base);
    char error_message[error_message_base_length + 128];
    sprintf(error_message, error_message_base, FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE, block_size);

    PF_LOG_CRITICAL(PF_ALLOCATOR, error_message);
    return PFEC_ERROR_OUT_OF_BOUNDS_MEMORY_USE; 
}

size_t pf_allocator_free_list_node_get_padding(PFAllocator_FreeListNode_t const * node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }

    size_t const shifted_padding = (node->metadata & FREE_LIST_NODE_METADATA__MASK_PADDING);
    size_t const padding_shift = (sizeof(size_t) * BITS_PER_BYTE) - FREE_LIST_NODE_METADATA_BITS;
    return shifted_padding >> padding_shift;
}

int32_t pf_allocator_free_list_node_set_padding(PFAllocator_FreeListNode_t* node, size_t const padding) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }

    size_t const padding_shift = (sizeof(size_t) * BITS_PER_BYTE) - FREE_LIST_NODE_METADATA_BITS;
    size_t const shifted_pad_value = padding << padding_shift;
    node->metadata |= (shifted_pad_value & FREE_LIST_NODE_METADATA__MASK_PADDING);
    return PFEC_NO_ERROR;
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

    // set the memory usage fns
    pf_free_list->pf_malloc = &pf_allocator_free_list_malloc;
    pf_free_list->pf_realloc = &pf_allocator_free_list_realloc;
    pf_free_list->pf_free = &pf_allocator_free_list_free;

    pf_free_list->base_memory = base_memory;
    pf_free_list->base_memory_size = size;



    // does the free-list struct live inside the memory it manages?
    int32_t const bFreeListIsInBaseMemory = pf_free_list == pf_free_list->base_memory;

    // we're not overwriting the beginning of the allocation, b/c this is where
    // the free list struct itself lives
    void* assignable_memory_start = NULL;
    size_t assignable_memory_size = 0;

    if (bFreeListIsInBaseMemory) {
        // if the free list lives in its own base memory, then all of the work it does
        // has to skip over the memory occupied by the struct itself.  In that case,
        // both the usable memory and the assignable memory, are smaller than the
        // base memory.
        size_t const pf_free_list_size = sizeof(PFAllocator_FreeList_t);
        assignable_memory_start = (void*)((uint64_t)pf_free_list->base_memory + pf_free_list_size);
        assignable_memory_size = pf_free_list->base_memory_size - pf_free_list_size;
    } else {
        // if the free list lives outside of its own base memory, then all of the
        // memory it owns is assignable, and we can just operate on the entire thing
        assignable_memory_start = pf_free_list->base_memory;
        assignable_memory_size = pf_free_list->base_memory_size;
    }

    // this should already be zeroed out, but let's not assume
    for (size_t i = 0; i < assignable_memory_size; i++) {
        uintptr_t* ptr = (void*)((uint64_t)assignable_memory_start + i);
        *ptr = 0;
    }

    // memory is now fresh, so just start rebuilding
    pf_free_list->used_memory = 0;
    PFAllocator_FreeListNode_t* first_node = assignable_memory_start;
    first_node->metadata = assignable_memory_size - sizeof(PFAllocator_FreeListNode_t);
    first_node->next = NULL;
    pf_free_list->head = first_node;
    return PFEC_NO_ERROR;
}



int32_t pf_allocator_free_list_free_all(PFAllocator_FreeList_t* pf_free_list) {
    if (pf_free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t* pointer is null!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (pf_free_list->base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t has no initialized base memory!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (pf_free_list->base_memory_size == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t has invalid owned memory size!");
        return PFEC_ERROR_INVALID_LENGTH;
    }
    if (pf_free_list->pf_malloc == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t->pf_malloc is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (pf_free_list->pf_realloc == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t->pf_realloc is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (pf_free_list->pf_free == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t->pf_free is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }

    return pf_allocator_free_list_initialize(
        pf_free_list,
        pf_free_list->base_memory,
        pf_free_list->base_memory_size);
    
    //// does the free-list struct live inside the memory it manages?
    //int32_t const bFreeListIsInBaseMemory = pf_free_list == pf_free_list->base_memory;

    //// we're not overwriting the beginning of the allocation, b/c this is where
    //// the free list struct itself lives
    //void* assignable_memory_start = NULL;
    //size_t assignable_memory_size = 0;

    //if (bFreeListIsInBaseMemory) {
    //    // if the free list lives in its own base memory, then all of the work it does
    //    // has to skip over the memory occupied by the struct itself.  In that case,
    //    // both the usable memory and the assignable memory, are smaller than the
    //    // base memory.
    //    size_t const pf_free_list_size = sizeof(PFAllocator_FreeList_t);
    //    assignable_memory_start = (void*)((uint64_t)pf_free_list->base_memory + pf_free_list_size);
    //    assignable_memory_size = pf_free_list->base_memory_size - pf_free_list_size;
    //} else {
    //    // if the free list lives outside of its own base memory, then all of the
    //    // memory it owns is assignable, and we can just operate on the entire thing
    //    assignable_memory_start = pf_free_list->base_memory;
    //    assignable_memory_size = pf_free_list->base_memory_size;
    //}

    //// zero out the whole thing immediately, and hope it crashes, if anyone is still using it
    //for (size_t i = 0; i < assignable_memory_size; i++) {
    //    uintptr_t* ptr = (void*)((uint64_t)assignable_memory_start + i);
    //    *ptr = 0;
    //}

    //// memory is now fresh, so just start rebuilding
    //pf_free_list->used_memory = 0;
    //PFAllocator_FreeListNode_t* first_node = assignable_memory_start;
    //first_node->block_size = assignable_memory_size - sizeof(PFAllocator_FreeListNode_t);
    //first_node->next = NULL;
    //pf_free_list->head = first_node;

    //return PFEC_NO_ERROR;
}

int32_t pf_allocator_is_power_of_two(size_t const size) {
    // we're not checking against zero, b/c size_t can't be negative
    return (size & (size - 1)) == 0;
}


size_t pf_allocator_free_list_calculate_padding_and_header(
    uintptr_t const ptr,
    uintptr_t const alignment,
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
    size_t const requested_size,
    size_t const alignment,
    size_t * out_padding,
    PFAllocator_FreeListNode_t** out_previous_node)
{
    PFAllocator_FreeListNode_t* node = free_list->head;
    PFAllocator_FreeListNode_t* prev_node = NULL;

    size_t padding = 0;

    while (node != NULL) {
        size_t const header_size = sizeof(PFAllocator_FreeListAllocationHeader_t);
        
        padding = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const required_sz = requested_size + padding;
        if (node->metadata >= required_sz) {
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
    size_t const requested_size,
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

    while (node != NULL) {
        size_t const header_size = sizeof(PFAllocator_FreeListAllocationHeader_t);
 
        padding = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const req_size = requested_size + padding;
        // technically, we are losing part of the information where diff_space would be
        // negative.  However, in this case, required_size would be greater than the
        // block size, so we are still checking against that in the first logical
        // clause here
        size_t const diff_space = node->metadata - req_size;
        if (node->metadata >= req_size && (diff_space < smallest_diff_space)) {
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

void * pf_allocator_free_list_malloc(size_t const size) {
    return malloc(size);
}

void * pf_allocator_free_list_realloc(void *ptr, size_t const size) {
    return realloc(ptr, size);
}

void pf_allocator_free_list_free(void *ptr) {
    free(ptr);
}


// -----------------------------------------------------------------------------------------------------------
// PFAllocator_RedBlackTree_t
// -----------------------------------------------------------------------------------------------------------

int32_t pf_allocator_red_black_tree_initialize(void* base_memory, size_t const size) {
    //pf_malloc = &malloc;
    //pf_realloc = &realloc;
    //pf_free = &free;
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



//int32_t pf_allocator_initialize(void * base_memory, size_t const size) {
//    if (base_memory == NULL) {
//        PF_LOG_ERROR(PF_ALLOCATOR, "Null ptr to base memory");
//        return PFEC_ERROR_NULL_PTR;
//    }
//    if (size == 0) {
//        PF_LOG_ERROR(PF_ALLOCATOR, "Invalid memory size");
//        return PFEC_ERROR_INVALID_LENGTH;
//    }
//
//#ifdef USE_FREE_LIST_IMPLEMENTATION
//    return pf_allocator_free_list_initialize(&s_allocator.memory, base_memory, size);
//#else
//    return pf_allocator_red_black_tree_initialize(base_memory, size);
//#endif
//}





