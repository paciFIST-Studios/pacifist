// paciFIST studios. 2025. MIT License

#ifndef PF_ALLOCATOR_H
#define PF_ALLOCATOR_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine





#define USE_PROVIDED_MEMORY 1
#define USE_FREE_LIST_IMPLEMENTATION 1


 
/**
 * PFAllocator is meant to be a general purpose allocator, which
 * seems to replace the normal calls to malloc, realloc, and free
 * While acting similarly to those fns, PFAllocator has several
 * specific features which set it apart:
 *
 *    1. it uses memory for the application arena
 *    2. it supports memory compaction
 *    3. it allows access to memory via handles, not pointers
 *        a. the handles can return pointers
 *    4. it uses specific instrumentation, to deliver detailed
 *        information about existing allocations
 *    5. (GC?)
 *
 * it probably won't be a perfect match, but I'm going to try to have
 * these fns work the same
 * 
 * 
 * 
 * https://www.gingerbill.org/series/memory-allocation-strategies/ 
 * 
 * 
 * 
 */

// sets memory to zero before returning it
// returns ptr to "new" memory, which is aligned correctly
// does zero out memory
// will accommodate several size regimes,
// will return null for out of memory error
// will set error message for out of memory error
//static void* (*pf_malloc)(size_t const);


// changes the size of this memory to the supplied size
// does zero out memory which is being released (but you don't know that)
// does zero out expanded memory
// returns pointer to new memory, which is aligned correctly
// if ptr is null, is like pf_malloc
// will accommodate several size regimes,
// will return null for out of memory error
// will set error message for out of memory error
//static void* (*pf_realloc)(void*, size_t const);


// "frees" memory pointed to by this ptr.
// sets ptr to null 
//static void (*pf_free)(void*);


// free-list implementation of a PFAllocator -----------------------------------------------------------------

typedef enum EAllocationPolicy_FreeList {
    EAPFL_POLICY_FIND_FIRST = 0,
    EAPFL_POLICY_FIND_BEST
} EAllocationPolicy_FreeList;

typedef struct PFAllocator_FreeListAllocationHeader_t {
    size_t const block_size;
    size_t const padding;
} PFAllocator_FreeListAllocationHeader_t;

typedef struct PFAllocator_FreeListNode_t {
    struct PFAllocator_FreeListNode_t* next;
    size_t block_size;
} PFAllocator_FreeListNode_t;

typedef struct PFAllocator_FreeList_t {
    void * base_memory;
    size_t owned_memory;
    size_t used_memory;

    PFAllocator_FreeListNode_t* head;
    EAllocationPolicy_FreeList policy;

    void* (*pf_malloc)(size_t const size);
    void* (*pf_realloc)(void* ptr, size_t const size);
    void  (*pf_free)(void* ptr);
    
} PFAllocator_FreeList_t;



int32_t pf_allocator_free_list_initialize(PFAllocator_FreeList_t* pf_free_list, void* base_memory, size_t const size);

int32_t pf_allocator_free_list_free_all(PFAllocator_FreeList_t* pf_free_list);

int32_t pf_allocator_is_power_of_two(size_t const size);


size_t pf_allocator_free_list_calculate_padding_and_header(uintptr_t ptr, uintptr_t alignment, size_t const header_size);

PFAllocator_FreeListNode_t* pf_allocator_free_list_find_first(
    PFAllocator_FreeList_t const * free_list,
    size_t const list_size,
    size_t const alignment,
    size_t * padding,
    PFAllocator_FreeListNode_t** previous_node);

PFAllocator_FreeListNode_t* pf_allocator_free_list_find_best(
    PFAllocator_FreeList_t const * free_list,
    size_t const list_size,
    size_t const alignment,
    size_t * padding,
    PFAllocator_FreeListNode_t** previous_node);



void* pf_allocator_free_list_malloc(size_t const size);

void* pf_allocator_free_list_realloc(void* ptr, size_t const size);

void pf_allocator_free_list_free(void* ptr);









//  red-black-tree implementation of PFAllocator -------------------------------------------------------------

typedef enum EAllocationPolicy_RedBlackTree {
    EAPRBT_UNDEFINED = 0
} EAllocationPolicy_RedBlackTree;

typedef struct PFAllocator_RedBlackTreeAllocationHeader_t {
    size_t const block_size;
    size_t const padding;
} PFAllocator_RedBlackTreeAllocationHeader_t;

typedef struct PFAllocator_RedBlackTreeNode_t {
    PFAllocator_RedBlackTreeAllocationHeader_t* next;
    size_t const block_size;
} PFAllocator_RedBlackTreeNode_t;

typedef struct PFAllocator_RedBlackTree_t {
    void * base_memory;
    size_t owned_memory;
    size_t used_memory;

    PFAllocator_RedBlackTreeNode_t* root;
    EAllocationPolicy_RedBlackTree policy;
} PFAllocator_RedBlackTree_t;

int32_t pf_allocator_red_black_tree_initialize(void* base_memory, size_t const size);

void* pf_provided_memory_red_black_tree_allocator(size_t const size);

// generic fns -----------------------------------------------------------------------------------------------

//typedef struct PFAllocator_t {
//    void* (*malloc)(size_t const size);
//    void* (*realloc)(void* ptr, size_t const size);
//    void  (*free)(void* ptr);
//
//#ifdef USE_FREE_LIST_IMPLEMENTATION
//    PFAllocator_FreeList_t memory;
//#else
//    PFAllocator__RedBlackTree_t memory;
//#endif
//} PFAllocator_t;













//int32_t pf_allocator_initialize(void * base_memory, size_t const size);


#endif //PF_ALLOCATOR_H
