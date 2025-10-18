// paciFIST studios. 2025. MIT License

#ifndef PF_ALLOCATOR_FREE_LIST_H
#define PF_ALLOCATOR_FREE_LIST_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine





/**
 * PFAllocator_FreeList is meant to be a general purpose allocator,
 * which replaces use of malloc, realloc, and free.
 * 
 * While acting similarly to those fns, PFAllocator_FreeList_t
 * has several specific features which set it apart:
 *
 *    1. it only uses memory it is provided
 *    2. it supports memory compaction
 *    3. it uses specific instrumentation, to deliver detailed
 *        information about existing allocations
 *
 * https://www.gingerbill.org/series/memory-allocation-strategies/ 
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
    EAPFL_POLICY_FIND_BEST = 0,
    EAPFL_POLICY_FIND_FIRST
} EAllocationPolicy_FreeList;

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

/**
 * @brief Returns TRUE(1) if the supplied node's metadata marks it as allocated
 *
 * @param node 
 * @return 
 */
int32_t pf_allocator_free_list_node_get_is_allocated(PFAllocator_FreeListNode_t const * node);

/**
 * @brief Sets the supplied node's metadata to indicate this node IS allocated
 *
 * @param node 
 */
void pf_allocator_free_list_node_set_is_allocated(PFAllocator_FreeListNode_t* node);

/**
 * @brief Sets the supplied node's metadata to indicate this node is NOT allocated
 *
 * @param node 
 */
void pf_allocator_free_list_node_set_is_not_allocated(PFAllocator_FreeListNode_t* node);

/**
 * @brief Returns the size of this node's memory block, according to its metadata
 * @note: the node block size is the allocation size, plus sizeof(PFAllocator_FreeListNode_t)
 *
 * @param node 
 * @return 
 */
size_t pf_allocator_free_list_node_get_block_size(PFAllocator_FreeListNode_t const * node);

/**
 * @brief Sets the size of this node's memory block, and stores the value in the node's metadata
 *
 * @param node 
 * @param block_size 
 * @return 
 */
int32_t pf_allocator_free_list_node_set_block_size(PFAllocator_FreeListNode_t * node, size_t block_size);

/**
 * @brief Returns the amount of memory this node has allocated (or can allocate) to a caller
 * 
 * @param node 
 * @return 
 */
size_t pf_allocator_free_list_node_get_allocation_size(PFAllocator_FreeListNode_t const * node);

/**
 * @brief Gets the amount of padding between the node struct, and the data to which it points
 *
 * @param node 
 * @return 
 */
size_t pf_allocator_free_list_node_get_padding(PFAllocator_FreeListNode_t const * node);


/**
 * @brief Sets the amount of padidng between the node struct, and the data to which it points
 *
 * @param node 
 * @param padding 
 * @return 
 */
int32_t pf_allocator_free_list_node_set_padding(PFAllocator_FreeListNode_t* node, size_t padding);




/**
 * @brief This struct organizes the FreeList allocator, and makes available fns needed to use it
 *
 * @property    void*   base_memory         - ptr to all memory managed by this allocator,
 *                                            Note: the allocator itself lives at this address
 * @property    size_t  base_memory_size    - size of all memory managed by allocator
 * @property    size_t  free_memory         - size of remaining memory available for allocation
 *                                            (some of this will be used to adminstrate the allocation)
 *
 * @property    PFAllocator_FreeListNode_t* head    - head of linked list, containing the allocations =
 * @property    EAllocationPolicy_FreeList  policy  - first available, or best-match
 *
 * @property    void*(*pf_malloc)(size_t size)
 * @property    void*(*pf_realloc)(void* ptr, size_t size)
 * @property    void (*pf_free)(void* ptr)
 * 
 */
typedef struct PFAllocator_FreeList_t {
    void * base_memory;
    size_t base_memory_size;
    size_t free_memory;

    PFAllocator_FreeListNode_t* head;
    EAllocationPolicy_FreeList policy;

    void* (*pf_malloc)( struct PFAllocator_FreeList_t* allocator, size_t size);
    void* (*pf_realloc)(struct PFAllocator_FreeList_t* allocator, void* ptr, size_t size);
    int32_t (*pf_free)( struct PFAllocator_FreeList_t* allocator, void* ptr);

} PFAllocator_FreeList_t;

/**
 * @brief Creates and initializes the allocator, placing it at the base of this memory.
 * @note Once created, the allocator assumes it controls all of the memory it was created with
 *
 * @param base_memory 
 * @param base_memory_size 
 * @return 
 */
PFAllocator_FreeList_t* pf_allocator_free_list_create_with_memory(void* base_memory, size_t const base_memory_size);

/**
 * @brief - zeroes out all memory managed by this allocator, and resets the allocator to a fresh state 
 *
 * @param pf_free_list 
 * @return 
 */
int32_t pf_allocator_free_list_free_all(PFAllocator_FreeList_t* pf_free_list);


/**
 * @brief returns the Nth node in the allocator, according to the supplied index
 *
 * @param allocator 
 * @param idx 
 * @return 
 */
void* pf_allocator_free_list_node_get_node_by_index(PFAllocator_FreeList_t const * allocator, size_t idx);


/**
 * @brief Given a memory address, returns the node managing that address (or NULL if there's a problem)
 * @note  returns NULL if the given memory is not actually managed by the given allocator
 *
 * @param allocator 
 * @param memory 
 * @return 
 */
PFAllocator_FreeListNode_t* pf_allocator_free_list_get_managing_node(PFAllocator_FreeList_t const * allocator, void* memory);


/**
 * @brief Returns TRUE(1), if the size provided is a power of two
 *
 * @param size 
 * @return 
 */
int32_t pf_allocator_is_power_of_two(size_t size);

/**
 * @brief Returns TRUE(1), if the memory block should be split, otherwise false
 * @note if this fn returns true, it will also provide the offset, at which the block should be split
 *
 * @param block_size
 * @param required_size
 * @param out_cut_at_offset 
 * @return 
 */
int32_t pf_allocator_should_bisect_memory(size_t block_size, size_t required_size, uintptr_t* out_cut_at_offset);

/**
 * @brief returns a count of how many bytes have been given out to users
 *
 * @param pf_free_list 
 * @return 
 */
size_t pf_allocator_free_list_get_allocator_available_memory_size(PFAllocator_FreeList_t * pf_free_list);

/**
 * @brief returns a count of how much memory is used by the allocator itself
 * @note: ie: the PFAllocator_FreeList_t struct, and a PFAllocator_FreeListNode_t for each allocation
 *
 * @param pf_free_list 
 * @return 
 */
size_t pf_allocator_free_list_get_memory_overhead_size(PFAllocator_FreeList_t const * pf_free_list);

/**
 * @brief given an address, a header size, and an alignment width, determines how much padding you should add, the next byte after your header+padding is also aligned
 * @note In a strict sense, we're aligning something which comes _after_ the header.
 *       For FreeList nodes, each node is a header, and the padding exists, to make sure the data is aligned.
 *       This fn does check that the header address is aligned as well.
 *
 *
 * @param header_address - the memory address which the header will live at.  This MUST be aligned to the alignment param
 * @param alignment - usually 16 byte aligned, but no default
 * @param header_size - the size of the header to calculate padding for
 * @return 
 */
size_t pf_allocator_free_list_calculate_padding(uintptr_t header_address, uintptr_t alignment, size_t header_size);

/**
 * @brief Searches the PFAllocator_FreeList_t for an empty memory block, and takes the first available
 *
 * @param free_list 
 * @param requested_size 
 * @param alignment 
 * @param out_padding 
 * @param out_previous_node 
 * @return 
 */
PFAllocator_FreeListNode_t* pf_allocator_free_list_find_first(
    PFAllocator_FreeList_t const * free_list,
    size_t requested_size,
    size_t alignment,
    uintptr_t*  out_padding,
    PFAllocator_FreeListNode_t** out_previous_node);

/**
 * @brief Searches all available memory blocks in a PFAllocator_FreeList_t, and takes the best fit 
 *
 * @param free_list 
 * @param requested_size 
 * @param alignment 
 * @param optional_out_padding - the padding between the node, and its data, fn will work if this is NULL 
 * @param optional_out_previous_node - the node just prior in the linked list, fn will work if this is NULL
 * @return 
 */
PFAllocator_FreeListNode_t* pf_allocator_free_list_find_best(
    PFAllocator_FreeList_t const * free_list,
    size_t requested_size,
    size_t alignment,
    uintptr_t* optional_out_padding,
    PFAllocator_FreeListNode_t** optional_out_previous_node);


/**
 * @brief Starting with the supplied coalescing_node, coalesces the next N nodes
 * @note  this fn will not coalesce allocated nodes
 *
 * @param allocator 
 * @param coalescing_node 
 * @param coalesce_count 
 * @return 
 */
int32_t pf_allocator_free_list_coalesce_n_nodes(
    PFAllocator_FreeList_t const * allocator,
    PFAllocator_FreeListNode_t * coalescing_node,
    size_t coalesce_count);



/**
 * 
 * @param allocator 
 * @return 
 */
int32_t pf_allocator_free_list_coalesce_unallocated_nodes(PFAllocator_FreeList_t const * allocator);




/**
 * @brief a Malloc fn which uses the PFAllocator_FreeList_t 
 *
 * @param allocator
 * @param requested_size 
 * @return 
 */
void* pf_allocator_free_list_malloc(PFAllocator_FreeList_t* allocator, size_t requested_size);

/**
 * @brief a Realloc fn which uses the PFAllocator_FreeList_t
 *
 * @param allocator
 * @param memory 
 * @param size 
 * @return 
 */
void* pf_allocator_free_list_realloc(PFAllocator_FreeList_t* allocator, void* memory, size_t size);

/**
 * @brief a Free fn which uses the PFAllocator_FreeList_t
 *
 * @param allocator
 * @param memory 
 */
int32_t pf_allocator_free_list_free(PFAllocator_FreeList_t* allocator, void* memory);



__attribute__((unused))
void pf_allocator_free_list_do_compaction(PFAllocator_FreeList_t* allocator);






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


#endif //PF_ALLOCATOR_FREE_LIST_H
