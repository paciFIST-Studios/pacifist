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
    struct PFAllocator_FreeListAllocationHeader_t* next;
    size_t const block_size;
    size_t const padding;
} PFAllocator_FreeListAllocationHeader_t;

typedef struct PFAllocator_FreeListNode_t {
    struct PFAllocator_FreeListNode_t* next;
    // the top 5 bits are used for metadata:
    // top bit: 1 = is allocated, 0 = is free
    // next 4 bits: padding before data starts
    /**
     * NOTE: we're doing it w/ size_t, in an attempt to make this program
     *      executable on 32bit and 64bit systems.  On a 64bit system,
     *      this allocator can deal with single blocks in the petabyte range,
     *      so I'm just going to completely ignore it.
     *
     *      On a 32bit system, the allocator has a max allocation size of
     *      134,217,727 bytes in a single allocation block.  134(ish) mb
     *      is still pretty big, but it's conceivable to have something which
     *      wouldn't fit.  Especially for things like, a texture atlas which
     *      covers every texture in the game.  In these cases, we'd still be
     *      able to use an arena allocator to store those objects, or we
     *      could create some other bespoke allocator which has large node headers,
     *      and can handle a larger contiguous block of memory.  Either way,
     *      it feels like this should(TM) be viable for our uses
     */
    size_t metadata;
} PFAllocator_FreeListNode_t;

int32_t pf_allocator_free_list_node_is_allocated(PFAllocator_FreeListNode_t const * node);

void pf_allocator_free_list_node_set_is_allocated(PFAllocator_FreeListNode_t* node);

void pf_allocator_free_list_node_set_is_not_allocated(PFAllocator_FreeListNode_t* node);

size_t pf_allocator_free_list_node_get_block_size(PFAllocator_FreeListNode_t const * node);

int32_t pf_allocator_free_list_node_set_block_size(PFAllocator_FreeListNode_t * node, size_t const block_size);

size_t pf_allocator_free_list_node_get_padding(PFAllocator_FreeListNode_t const * node);

int32_t pf_allocator_free_list_node_set_padding(PFAllocator_FreeListNode_t* node, size_t const padding);



typedef struct PFAllocator_FreeList_t {
    void * base_memory;
    size_t base_memory_size;
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


size_t pf_allocator_free_list_calculate_padding_and_header(uintptr_t const ptr, uintptr_t const alignment, size_t const header_size);

PFAllocator_FreeListNode_t* pf_allocator_free_list_find_first(
    PFAllocator_FreeList_t const * free_list,
    size_t const requested_size,
    size_t const alignment,
    size_t*  out_padding,
    PFAllocator_FreeListNode_t** out_previous_node);

PFAllocator_FreeListNode_t* pf_allocator_free_list_find_best(
    PFAllocator_FreeList_t const * free_list,
    size_t const list_size,
    size_t const alignment,
    size_t * out_padding,
    PFAllocator_FreeListNode_t** out_previous_node);



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
