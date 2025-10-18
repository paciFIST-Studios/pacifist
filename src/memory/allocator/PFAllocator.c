// paciFIST studios. 2025. MIT License

// include
#include "memory/allocator/PFAllocator.h"

// stdlib
#include <stdlib.h>
// framework
// engine
#include <core/common.h>
#include <memory/hash_table/PFCompactHashTable.h>

#include "core/assert.h"
#include "core/debug.h"
#include "core/define.h"
#include "core/error.h"

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
    size_t const error_message_base_length = strlen(error_message_base);
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


PFAllocator_FreeList_t* pf_allocator_free_list_create_with_memory(void* base_memory, size_t const base_memory_size) {
    if (base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t without valid base_memory ptr!")
        return NULL;
    }
    if (base_memory_size == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t to zero size!");
        return NULL;
    }
    if (base_memory_size <= 80) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot initialize PFAllocator_FreeList_t with fewer than 80 bytes!  Suggest 4096 minimum.");
        return NULL;
    }

    // this should already be zeroed out, but let's not assume
    memset(base_memory, 0, base_memory_size);

    // create & initialize the allocator
    PFAllocator_FreeList_t* pf_free_list = (PFAllocator_FreeList_t*)base_memory;
    pf_free_list->base_memory = base_memory;
    pf_free_list->base_memory_size = base_memory_size;
    // set the memory usage fns
    pf_free_list->pf_malloc = &pf_allocator_free_list_malloc;
    pf_free_list->pf_realloc = &pf_allocator_free_list_realloc;
    pf_free_list->pf_free = &pf_allocator_free_list_free;
    // set the default policy
    pf_free_list->policy = EAPFL_POLICY_FIND_BEST;
    
    // creating & initialize the "first" node
    uintptr_t const first_node_memory_offset = (size_t)base_memory + sizeof(PFAllocator_FreeList_t);
    void* first_node_node_memory = (void*)first_node_memory_offset;
    PFAllocator_FreeListNode_t* first_node = first_node_node_memory;
    // the memory block in the first node, is everything except for the allocator itself
    size_t const first_node_block_size = base_memory_size - sizeof(PFAllocator_FreeList_t);
    pf_allocator_free_list_node_set_block_size(first_node, first_node_block_size);
    // the user allocates this memory, not us, so we're setting it to not-allocated
    pf_allocator_free_list_node_set_is_not_allocated(first_node);

    // update the free_memory count, so we know how much free memory there is.  At this point,
    // we've spent memory on storing the allocator, and on storing the first node, but otherwise,
    // we should have everything left over from the base memory
    pf_free_list->free_memory = base_memory_size - sizeof(PFAllocator_FreeList_t) - sizeof(PFAllocator_FreeListNode_t);

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

PFAllocator_FreeListNode_t * pf_allocator_free_list_get_managing_node(
    PFAllocator_FreeList_t const * allocator,
    void* memory)
{
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to PFAllocator_FreeList_t param!");
        return NULL;
    }
    if (memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to memory param!");
        return NULL;
    }
    if ((uintptr_t)memory < (uintptr_t)allocator || (uintptr_t)memory > (uintptr_t)(allocator + allocator->base_memory_size)) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got a ptr to memory which is not owned by this allocator!");
        return NULL;
    }

    PFAllocator_FreeListNode_t* node = allocator->head;
    while (node != NULL) {
        size_t const node_size = sizeof(PFAllocator_FreeListNode_t);
        size_t const node_padding = pf_allocator_free_list_node_get_padding(node);
        void* data = (void*)((uintptr_t)node + node_size + node_padding);
        if (memory == data) {
            return node;
        }
        node = node->next;
    }

    PF_LOG_CRITICAL(PF_ALLOCATOR, "Could not find managing node for this memory! Allocator is likely in a bad state!");

    return NULL;
}

int32_t pf_allocator_is_power_of_two(size_t const size) {
    // we're not checking against zero, b/c size_t can't be negative
    return (size & (size - 1)) == 0;
}

int32_t pf_allocator_should_bisect_memory(
    size_t const block_size,
    size_t const required_size,
    uintptr_t *out_cut_at_offset) {
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
    if (block_size < required_size + (sizeof(PFAllocator_FreeListNode_t) /*+ MINIMUM_NODE_ALLOC_MEMORY*/)) {
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

size_t pf_allocator_free_list_get_allocator_available_memory_size(PFAllocator_FreeList_t* free_list) {
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
        sprintf(message, "PFAllocator_FreeList_t has lost track of some memory!\n\tAllocator.CalculatedFreeMemory=%zu\n\tAllocator.FreeMemory=%zu\n\tAllocator.OwnedMemory=%zu\n\tAllocator.UsedMemory=%zu\n",
            calculated_free_memory, free_list->free_memory, base_memory_size, used_memory_count);
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


size_t pf_allocator_free_list_calculate_padding(
    uintptr_t const header_address,
    uintptr_t const alignment,
    size_t const header_size)
{
    if (!pf_allocator_is_power_of_two(alignment)) {
        char message[64] ={0};
        snprintf(message, 64, "Requested alignment is not a power of two! Align=%ld", alignment);
        PF_LOG_CRITICAL(PF_ALLOCATOR, message);
        return -1;
    }
    if (header_address == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Header_address argument is unexpectedly null!");
        return -1;
    }
    if (header_address % alignment != 0) {
        char message[80] = {0};
        snprintf(message,  80, "Header_address is not aligned to given alignment! Align=%ld, OffBy=%ld", alignment, header_address % alignment);
        PF_LOG_CRITICAL(PF_ALLOCATOR, message);
        return -1;
    }

    // is the header naturally aligned to this boundary?
    size_t const modular_remainder = header_size % alignment;
    if (modular_remainder == 0) {
        return 0;
    }

    return alignment - modular_remainder;
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

    uintptr_t padding = 0;

    // there will always be at least 1 unallocated node, unless the allocator is 100% full
    while (node != NULL) {

        // find the unallocated node, and see if it can hold this allocation
        if (!pf_allocator_free_list_node_get_is_allocated(node)) {

            padding = pf_allocator_free_list_calculate_padding(
                (uintptr_t)node,
                (uintptr_t)alignment,
                sizeof(PFAllocator_FreeListNode_t));
 
            size_t const required_sz = requested_size + padding;
            size_t const node_sz = pf_allocator_free_list_node_get_block_size(node);

            // actually, it can
            if (node_sz >= required_sz) {
                break;
            }
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
    uintptr_t* optional_out_padding,
    PFAllocator_FreeListNode_t** optional_out_previous_node)
{
    if (free_list == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to PFAllocator_FreeList_t!");
        return NULL;
    }
    if (requested_size < sizeof(PFAllocator_FreeListNode_t)) {
        char message[128] = {0};
        snprintf(message, 128, "Got request for too-small allocation!  Requested=%ld, Minimum=%ld", requested_size, sizeof(PFAllocator_FreeListNode_t));
        PF_LOG_CRITICAL(PF_ALLOCATOR, message);
        return NULL;
    }
    if (!pf_allocator_is_power_of_two(alignment)) {
        char message[128] = {0};
        snprintf(message, 128, "Got request for non-power-of-two alignment! RequestedAlign=%ld", alignment);
        PF_LOG_CRITICAL(PF_ALLOCATOR, message);
        return NULL;
    }
    // we're not checking for these param, b/c they're optional
    //if (out_padding == NULL) {}
    //if (optional_out_previous_node == NULL) {}

    // the difference in size, between the requested size, and the discovered size
    // this algorithm takes a size_t, with a 1 in every bit, and progressively
    // finds smaller values as it scans the allocator's backing memory.
    // after all the backing memory is scanned, the smallest viable block
    // of memory is returned
    size_t smallest_diff_space = ~(size_t)0;

    PFAllocator_FreeListNode_t* node = free_list->head;
    PFAllocator_FreeListNode_t* prev_node = NULL;
    PFAllocator_FreeListNode_t* prev_to_best_node = NULL;
    PFAllocator_FreeListNode_t* best_node = NULL;

    // offset after the header, and before the data
    uintptr_t padding = 0;

    while (node != NULL) {

        // only check nodes which aren't allocated
        if (!pf_allocator_free_list_node_get_is_allocated(node)) {
            padding = pf_allocator_free_list_calculate_padding(
                (uintptr_t)node,
                (uintptr_t)alignment,
                sizeof(PFAllocator_FreeListNode_t));

            // the memory needed for this allocation
            size_t const needed_size = requested_size + padding;
            // the memory available for this allocation
            size_t const block_size = pf_allocator_free_list_node_get_block_size(node);

            // we're looking for the smallest difference between the size we need and the size we have
            // if we get a negative, it turns into an INT_MAX kind of number, and is rejected by the algorithm
            size_t const diff_space = block_size - needed_size;

            // is there enough space, and is this a closer fit than our previous best?
            if (block_size >= needed_size && (diff_space < smallest_diff_space)) {
                best_node = node;
                prev_to_best_node = prev_node;
                smallest_diff_space = diff_space;

                if (smallest_diff_space == 0){ break; }
            }
        }

        prev_node = node;
        node = node->next;
    }



    if (optional_out_padding){ *optional_out_padding = padding;}
    if (optional_out_previous_node){ *optional_out_previous_node = prev_to_best_node; }

    return best_node;
}

int32_t pf_allocator_free_list_coalesce_n_nodes(
    PFAllocator_FreeList_t const * allocator,
    PFAllocator_FreeListNode_t* coalescing_node,
    size_t coalesce_count)
{
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to PFallocator_FreeList_t");
        return PFEC_ERROR_NULL_PTR;
    }
    if (coalescing_node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to PFAllocator_FreeListNode_t");
        return PFEC_ERROR_NULL_PTR;
    }
    if (coalesce_count < 2) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot coalesce fewer than 2 nodes!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    PFAllocator_FreeListNode_t* node = coalescing_node;
    size_t const coalescing_node_padding = pf_allocator_free_list_node_get_padding(node);

    // count the block size of all the nodes we're going to coalesce,
    // starting with the coalescing_node
    size_t coalescing_block_size_accumulator = 0;
    for (size_t i = 0; i < coalesce_count; i++) {
        // padding is usually going to be zero, but make sure we're taking it into account
        coalescing_block_size_accumulator += pf_allocator_free_list_node_get_padding(node);
        coalescing_block_size_accumulator += pf_allocator_free_list_node_get_block_size(node);
        node = node->next;
    }

    // zeroing this out sets it to not_allocated as well
    memset(coalescing_node, 0, coalescing_block_size_accumulator + coalescing_node_padding);

    pf_allocator_free_list_node_set_padding(coalescing_node, coalescing_node_padding);
    pf_allocator_free_list_node_set_block_size(coalescing_node, coalescing_block_size_accumulator);

    // don't forget to keep the connection to the next node, which might be null,
    // but probably isn't (because we're probably in the middle of the free-list)
    coalescing_node->next = node;
    
    return TRUE;
}



int32_t pf_allocator_free_list_coalesce_unallocated_nodes(PFAllocator_FreeList_t const * allocator) {
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got ptr to null PFAllocator_FreeList_t param!");
        return FALSE;
    }

    int32_t coalescing_node_count = 0;
    PFAllocator_FreeListNode_t* coalescing_node = NULL;

    PFAllocator_FreeListNode_t* current_node = allocator->head;
    while (current_node != NULL) {
        int32_t const current_node_is_allocated = pf_allocator_free_list_node_get_is_allocated(current_node);

        // node not allocated
        if (!current_node_is_allocated) {
            if (coalescing_node == NULL) {
                coalescing_node = current_node;
            }

            coalescing_node_count++;

        // node is allocated
        } else {
            // see if we've found more than 1 unallocated node in a row
            // if there's a single unallocated block, we can't merge it with anything
            if (coalescing_node_count > 1) {
                pf_allocator_free_list_coalesce_n_nodes(allocator, coalescing_node, coalescing_node_count);
            }

            // reset the state so we're not coalescing
            coalescing_node = NULL;
            coalescing_node_count = 0;
        }

        current_node = current_node->next;
    }

    // handles the situation where there is a run of empty nodes, and the last node is also empty,
    // so we haven't triggered their coalescence yet
    if (coalescing_node_count > 1) {
        pf_allocator_free_list_coalesce_n_nodes(allocator, coalescing_node, coalescing_node_count);
    }

    return TRUE;
}

void* pf_allocator_free_list_malloc(PFAllocator_FreeList_t* allocator, size_t const requested_size) {
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
    PFAllocator_FreeListNode_t* prev_node = NULL;
    uintptr_t alloc_owning_node_padding = 0;

    // get the node we will allocate from, depending on our lookup policy
    if (allocator->policy == EAPFL_POLICY_FIND_BEST) {
        alloc_owning_node = pf_allocator_free_list_find_best(allocator, requested_size, 16, &alloc_owning_node_padding, &prev_node);
    } else {
        alloc_owning_node = pf_allocator_free_list_find_first(allocator, requested_size, 16, &alloc_owning_node_padding, &prev_node);
    }

    if (alloc_owning_node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Could not find a free node for allocation!");
        return NULL;
    }

    // this is the address we'll return to the user, which represents the beginning of the
    // memory they've requested, and they just have to only use the amount they requested
    uintptr_t const alloc_owning_node_user_memory_offset = (uintptr_t)alloc_owning_node
                                                         + alloc_owning_node_padding
                                                         + sizeof(PFAllocator_FreeListNode_t);

    // we since we're using some/all of this memory, set the node as allocated
    pf_allocator_free_list_node_set_is_allocated(alloc_owning_node);
    // this memory is now claimed
    allocator->free_memory -= requested_size;

    // There is a question of how much memory this gives the user.  Let's measure how much that is,
    // and if we should cut it up, and create a new node to manage it, we will
    size_t const alloc_owning_node_pre_bisect_block_size = pf_allocator_free_list_node_get_block_size(alloc_owning_node);

    uintptr_t bisect_at_offset = 0;
    if (pf_allocator_should_bisect_memory(alloc_owning_node_pre_bisect_block_size, required_memory, &bisect_at_offset)){
        // We're bisecting the memory.  This means:
        //  1. All the memory in this node, will be cut into two portions: portion_A & portion_B
        //  2. portion_A, is resized to be the requested size
        //      a. portion_A stays with the current node
        //  3. portion_b, is used to make a new node
        //      a. creating this new node means decreasing free memory by the size of 1 node
        //      b. the new node manages all of portion_b's memory

        // create a new node, which will live 1 byte past the requested alloc
        uintptr_t const next_node_offset = (uintptr_t)alloc_owning_node + alloc_owning_node_padding + bisect_at_offset;
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
            sprintf(assert_message, "Warning! PFAllocator_FreeList_t has lost track of some memory!\n\tCurrentNode.NewBlockSize=%zu\n\tNextNode.NewBlockSize=%zu\n\tCurrentNode.OldBlockSize=%zu\n\tMissingSize=%zu\n",
                current_node_block_size, next_node_block_size, alloc_owning_node_pre_bisect_block_size, (alloc_owning_node_pre_bisect_block_size - current_node_block_size - next_node_block_size));
            PF_ASSERT_MESSAGE((current_node_block_size + next_node_block_size == alloc_owning_node_pre_bisect_block_size),
                assert_message);
        }

        // update the block sizes
        pf_allocator_free_list_node_set_block_size(next_node, next_node_block_size);
        pf_allocator_free_list_node_set_block_size(alloc_owning_node, bisect_at_offset);
    } else {
        // there isn't enough memory left to justify cutting this block, so, in effect,
        // we've given out the last allocation we can give.  This user can still query
        // the block size to find out exactly how much they're allowed to use
        allocator->free_memory = 0;
    }

    return (void*)alloc_owning_node_user_memory_offset;
}

void * pf_allocator_free_list_realloc(
    PFAllocator_FreeList_t* allocator,
    void* memory,
    size_t const size)
{
    return NULL;
}

int32_t pf_allocator_free_list_free(
    PFAllocator_FreeList_t* allocator,
    void* memory)
{
    if (allocator == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Got null ptr to allocator!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Was asked to free ptr to null!");
        return PFEC_ERROR_NULL_PTR;
    }

    PFAllocator_FreeListNode_t* node = pf_allocator_free_list_get_managing_node(allocator, memory);
    if (node == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Could not retrieve node which manages this memory!");
    }

    size_t const block_size = pf_allocator_free_list_node_get_block_size(node);
    size_t const padding_size = pf_allocator_free_list_node_get_padding(node);
    size_t const node_size = sizeof(PFAllocator_FreeListNode_t);

    size_t const memory_size = block_size - node_size - padding_size; 
    memset(memory, 0, memory_size);

    pf_allocator_free_list_node_set_is_not_allocated(node);

    // loop the free list and coalesce unallocated nodes


   return PFEC_NO_ERROR; 
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





