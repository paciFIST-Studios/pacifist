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
// by default, we try to align all memory use to this N-byte boundary
#define MEMORY_ALIGNMENT_SIZE 16
// smallest amount of memory a PFAllocator_FreeListNode_t can allocate
#define MINIMUM_NODE_ALLOC_MEMORY 256

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


int32_t pf_allocator_free_list_node_get_is_allocated(PFAllocator_FreeListNode_t const * node) {
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
        size_t const padding = pf_allocator_free_list_node_get_padding(node);
        size_t const is_allocated = node->metadata & FREE_LIST_NODE_METADATA__MASK_IS_ALLOCATED;

        node->metadata = 0;

        node->metadata |= (block_size & FREE_LIST_NODE_METADATA__MASK_BLOCK_SIZE);
        node->metadata |= is_allocated;
        pf_allocator_free_list_node_set_padding(node, padding);
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

size_t pf_allocator_free_list_node_get_allocation_size(PFAllocator_FreeListNode_t const *node) {
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Null ptr to PFAllocator_FreeListNode_t!");
        return PFEC_ERROR_NULL_PTR;
    }
    
    size_t const block_size = pf_allocator_free_list_node_get_block_size(node);
    return block_size - sizeof(PFAllocator_FreeListNode_t); 
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


PFAllocator_FreeList_t* pf_allocator_free_list_create_with_memory(void* base_memory, size_t const size) {
    if (base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t without valid base_memory ptr!")
        return NULL;
    }
    if (size == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t to zero size!");
        return NULL;
    }
    if (size <= 80) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t with fewer than 80 bytes!  Suggest 4096 minimum.");
        return NULL;
    }

    // this should already be zeroed out, but let's not assume
    memset(base_memory, 0, size);

    PFAllocator_FreeList_t* pf_free_list = (PFAllocator_FreeList_t*)base_memory;

    pf_free_list->base_memory = base_memory;
    pf_free_list->base_memory_size = size;

    // set the memory usage fns
    pf_free_list->pf_malloc = &pf_allocator_free_list_malloc;
    pf_free_list->pf_realloc = &pf_allocator_free_list_realloc;
    pf_free_list->pf_free = &pf_allocator_free_list_free;

    pf_free_list->policy = EAPFL_POLICY_FIND_BEST;
    
    // available memory starts just after the end of this allocator
    size_t const allocator_available_memory_offset = (size_t)base_memory + sizeof(PFAllocator_FreeList_t);
    void* allocator_available_memory_base = (void*)(allocator_available_memory_offset );


    // set up the first node
    PFAllocator_FreeListNode_t* first_node = allocator_available_memory_base;
    // data can be allocated, starting at the end of this node
    size_t const first_node_data_start_offset = allocator_available_memory_offset + sizeof(PFAllocator_FreeListNode_t);
    // the first created node contains all of the memory remaining to the allocator
    // which is:
    //      80 = sizeof(allocator) + sizeof(node)
    //      total - sizeof(allocator) - sizeof(node)
    size_t const first_node_block_size = ((size_t)base_memory+size) - first_node_data_start_offset;
    pf_allocator_free_list_node_set_block_size(first_node, first_node_block_size);
    // the user allocates this memory, not us, so we're setting it to not-allocated
    pf_allocator_free_list_node_set_is_not_allocated(first_node);

    // some of the memory is used to hold the allocator struct itself
    pf_free_list->free_memory = first_node_block_size;

    // set up the list itself
    first_node->next = NULL;
    pf_free_list->head = first_node;
    return pf_free_list;
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

    void* memory = pf_free_list->base_memory;
    size_t const size = pf_free_list->base_memory_size;

    // zero out all of this memory, entirely
    memset(memory, 0, size);

    // re-create on the same memory
    pf_free_list = pf_allocator_free_list_create_with_memory(memory, size);

    return PFEC_NO_ERROR;
}

void * pf_allocator_free_list_node_get_node_by_index(PFAllocator_FreeList_t const * allocator, size_t idx) {
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t pointer is unexpectedly null!");
        return NULL;
    }

    if (idx == 0) {
        return allocator->head;
    }

    size_t node_index = 0;

    PFAllocator_FreeListNode_t* node = allocator->head;
    while (node->next != NULL) {
        node = node->next;
        node_index += 1;

        if (node_index == idx) {
            return node;
        }
    }
    
    return NULL;
}

int32_t pf_allocator_is_power_of_two(size_t const size) {
    // we're not checking against zero, b/c size_t can't be negative
    return (size & (size - 1)) == 0;
}

int32_t pf_allocator_should_bisect_memory(
    size_t const block_size,
    size_t const required_size,
    size_t *out_cut_at_offset) {
    // block size cannot fulfill request
    if (block_size < required_size){
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got request to analyze block which is smaller than required size!");
        return FALSE;
    }
    // block size exactly fulfills request
    if (block_size == required_size) {
        return FALSE;
    }
    // block size is bigger than request, but too small for an additional block
    if (block_size < required_size + (sizeof(PFAllocator_FreeListNode_t) + MINIMUM_NODE_ALLOC_MEMORY)) {
        return FALSE;
    }
    // block is big enough to get split in two
    if (out_cut_at_offset != NULL){
        if (required_size % MEMORY_ALIGNMENT_SIZE == 0) {
            *out_cut_at_offset = required_size;
        } else {
            *out_cut_at_offset = required_size - (required_size % MEMORY_ALIGNMENT_SIZE) + MEMORY_ALIGNMENT_SIZE;
        }
    }

    return TRUE;
}

size_t pf_allocator_free_list_get_allocator_available_memory_size(PFAllocator_FreeList_t * free_list) {
    if (free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t pointer is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (free_list->head == NULL){
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t->head is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }

    size_t const base_memory_size = free_list->base_memory_size;
    
    size_t used_memory_count = sizeof(PFAllocator_FreeList_t);
    PFAllocator_FreeListNode_t const * node = free_list->head;
    while (node != NULL) {
        int32_t const isAllocated = pf_allocator_free_list_node_get_is_allocated(node);

        // if a node is "allocated", then a user is using that memory, and it's not available
        if (isAllocated) {
            used_memory_count += pf_allocator_free_list_node_get_block_size(node);
            // if a node is not allocated, then only the memory of the node itself is unavailable
        } else {
            used_memory_count += sizeof(PFAllocator_FreeListNode_t);
        }

        node = node->next;
    }

    size_t const calculated_free_memory = base_memory_size - used_memory_count;

    // This error message helps diagnose the lost memory issue
    {
        char message[256] = {0};
        sprintf(message, "PFAllocator_FreeList_t has lost track of some memory!\n\tAllocator.CalculatedFreeMemory=%d\n\tAllocator.FreeMemory=%d\n\tAllocator.OwnedMemory=%d\n\tAllocator.UsedMemory=%d\n",
            (uint32_t)calculated_free_memory, (uint32_t)free_list->free_memory, (uint32_t)base_memory_size, (uint32_t)used_memory_count);
        PF_ASSERT_MESSAGE((free_list->free_memory == calculated_free_memory), message);
    }
    free_list->free_memory = calculated_free_memory;

    return free_list->free_memory;
}

size_t pf_allocator_free_list_get_memory_overhead_size(PFAllocator_FreeList_t const * free_list) {
    if (free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t pointer is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (free_list->head == NULL){
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t->head is unexpectedly null!");
        return PFEC_ERROR_NULL_PTR;
    }

    size_t accumulator = sizeof(PFAllocator_FreeList_t);
    PFAllocator_FreeListNode_t const * node = free_list->head;
    while (node != NULL) {
        accumulator += sizeof(PFAllocator_FreeListNode_t);
        node = node->next;
    }
    return accumulator;
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
        size_t const header_size = sizeof(PFAllocator_FreeListNode_t);

        padding = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const required_sz = requested_size + padding;
        size_t const node_sz = pf_allocator_free_list_node_get_block_size(node);
        if (node_sz >= required_sz) {
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

    size_t header_offset = 0;

    while (node != NULL) {
        size_t const header_size = sizeof(PFAllocator_FreeListNode_t);
 
        header_offset = pf_allocator_free_list_calculate_padding_and_header(
            (uintptr_t)node,
            (uintptr_t)alignment,
            header_size);

        size_t const req_size = requested_size + header_offset;
        // technically, we are losing part of the information where diff_space would be
        // negative.  However, in this case, required_size would be greater than the
        // block size, so we are still checking against that in the first logical
        // clause here
        size_t const node_size = pf_allocator_free_list_node_get_block_size(node);
        size_t const diff_space = node_size - req_size;
        if (node_size >= req_size && (diff_space < smallest_diff_space)) {
            best_node = node;
            smallest_diff_space = diff_space;
        }

        prev_node = node;
        node = node->next;
    }

    if (out_padding){ *out_padding = header_offset;}
    if (out_previous_node){ *out_previous_node = prev_node; }

    return best_node;
}

void * pf_allocator_free_list_malloc(PFAllocator_FreeList_t* allocator, size_t const requested_size) {
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot allocate with null ptr to allocator!");
        return NULL;
    }
    if (requested_size == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot allocate 0-bytes!");
        return NULL;
    }
    if (allocator->base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t is in a bad state! Ptr to base memory was NULL!");
        // NOTE: if the pf_allocator_create_with_memory fn in unchanged, this should(TM) be impossible, b/c
        // the allocator itself sits at the base of the base memory, and we shouldn't(TM) have gotten a ptr
        // to it, if the base memory was actually null.
        return NULL;
    }

    size_t const available_memory = allocator->free_memory;
    size_t const required_memory = requested_size + sizeof(PFAllocator_FreeListNode_t);
    if (available_memory < required_memory) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "PFAllocator_FreeList_t does not have enough memory to fulfill this memory request!");
        return NULL;
    }
    if (available_memory == required_memory) {
        PF_LOG_WARNING(PF_ALLOCATOR, "PFAllocator_FreeList_t just gave out its last byte of memory! Continuing program.");
    }

    PFAllocator_FreeListNode_t* alloc_owning_node = NULL;
    size_t alloc_owning_node_padding;

    // get the node we will allocate from, depending on our lookup policy
    if (allocator->policy == EAPFL_POLICY_FIND_BEST) {
        alloc_owning_node = pf_allocator_free_list_find_best(allocator, requested_size, 16, &alloc_owning_node_padding, NULL);
    } else {
        alloc_owning_node = pf_allocator_free_list_find_first(allocator, requested_size, 16, &alloc_owning_node_padding, NULL);
    }

    if (alloc_owning_node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Could not find a free node for allocation!");
        return NULL;
    }
    if (pf_allocator_free_list_node_get_is_allocated(alloc_owning_node)) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Tried to allocate memory from a node which is already allocated!");
        return NULL;
    }

    // this is the address we'll return to the user, which represents the beginning of the
    // memory they've requested, and they just have to only use the amount they requested
    size_t const alloc_owning_node_user_memory_start = (size_t)alloc_owning_node
                                                        + alloc_owning_node_padding
                                                        + sizeof(PFAllocator_FreeListNode_t);

    // we since we're using some/all of this memory, set the node as allocated
    pf_allocator_free_list_node_set_is_allocated(alloc_owning_node);
    // this memory is now claimed
    allocator->free_memory -= requested_size;
    
    // There is a question of how much memory this gives the user.  Let's measure how much that is,
    // and if we should cut it up, and create a new node to manage it, we will
    size_t const alloc_owning_node_pre_bisect_block_size = pf_allocator_free_list_node_get_block_size(alloc_owning_node);

    size_t bisect_at_offset = 0;
    if (pf_allocator_should_bisect_memory(alloc_owning_node_pre_bisect_block_size, required_memory, &bisect_at_offset)){
        // We're bisecting the memory.  This means:
        //  1. All the memory in this node, will be cut into two portions: portion_A & portion_B
        //  2. portion_A, is resized to be the requested size
        //      a. portion_A stays with the current node
        //  3. portion_b, is used to make a new node
        //      a. creating this new node means decreasing free memory by the size of 1 node
        //      b. the new node manages all of portion_b's memory

        // create a new node, which will live 1 byte past the requested alloc
        size_t const next_node_offset = (size_t)alloc_owning_node + alloc_owning_node_padding + bisect_at_offset;
        PFAllocator_FreeListNode_t* next_node = (void*)next_node_offset;
        alloc_owning_node->next = next_node;

        // decrease the free_memory size to account for the new node.  the memory it manages
        // is still available to allocate, so don't decrease by the whole block size
        allocator->free_memory -= sizeof(PFAllocator_FreeListNode_t);


        // reset the sizes of the next node, and the current node, to distribute the memory correctly
        size_t const current_node_block_size = requested_size + sizeof(PFAllocator_FreeListNode_t);
        // ok, so this is complicated for me.  Don't touch this math, it's correct.
        //
        //  When we're doing the 1st allocation, we have an allocator and a node, and the node
        //  owns all the memory we could allocate.  When we measure the alloc_owning...block_size,
        //  that block size includes the size of the node itself, and the bisect_at_offset also
        //  includes the memory which will hold its node, so we mix in the size of a node directly,
        //  we'll end up counting it twice
        size_t const next_node_block_size = alloc_owning_node_pre_bisect_block_size - bisect_at_offset;

        // This assertion prints a helpful debugging message if it fails
        // 20250917 EBarrett: I'm just going to leave it in, until this thing is battle tested
        {
            char assert_message[196] = {0};
            sprintf(assert_message, "Warning! PFAllocator_FreeList_t has lost track of some memory!\n\tCurrentNode.NewBlockSize=%d\n\tNextNode.NewBlockSize=%d\n\tCurrentNode.OldBlockSize=%d\n\tMissingSize=%d\n",
                (int32_t)current_node_block_size, (int32_t)next_node_block_size, (int32_t)alloc_owning_node_pre_bisect_block_size, (int32_t)(alloc_owning_node_pre_bisect_block_size - current_node_block_size - next_node_block_size));
            PF_ASSERT_MESSAGE((current_node_block_size + next_node_block_size == alloc_owning_node_pre_bisect_block_size),
                assert_message);
        }

        // update the block sizes
        pf_allocator_free_list_node_set_block_size(next_node, next_node_block_size);
        pf_allocator_free_list_node_set_block_size(alloc_owning_node, bisect_at_offset);

    }

    return (void*)alloc_owning_node_user_memory_start;
}

void * pf_allocator_free_list_realloc(PFAllocator_FreeList_t* allocator, void *ptr, size_t const size) {
    return NULL;
}

void pf_allocator_free_list_free(PFAllocator_FreeList_t* allocator, void *ptr) {
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





