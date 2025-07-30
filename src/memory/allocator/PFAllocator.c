// paciFIST studios. 2025. MIT License

// include
#include "../allocator/PFAllocator.h"

// stdlib
#include <stdlib.h>
// framework
// engine

#include "../../core/error.h"

static PFAllocator_t s_allocator = {0};


// -----------------------------------------------------------------------------------------------------------
// PFAllocator_FreeList_t
// -----------------------------------------------------------------------------------------------------------


void* pf_provided_memory_free_list_allocator(size_t const size) {
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
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator is not initialized!");
        return -1;
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





