// paciFIST studios. 2025. MIT License

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine

/**
 * @brief  an interface for allocations fns.  Malloc fits this description
 *
 *
 * @param total_bytes 
 * @return 
 */
typedef void* (AllocatorFunction_t)(size_t total_bytes);

/**
 * @brief  A linear allocator, which can portion out memory for smaller uses.
 *
 *  A MemoryArena_t is a linear allocator which begins life by being initialized
 *  with some memory.  During its lifetime, uses of PUSH_STRUCT and PUSH_ARRAY,
 *  can request memory from the arena.
 *
 *  The MemoryArena_t does not free any of its memory, and does not resize.
 *  The purpose of MemoryArena_t, is to enforce scope, and ideally, other
 *  allocators will use this memory for other purposes.
 *
 *      size_t bytes_owned          -   how many bytes of memory belong to this arena,
 *                                      including the bytes used by the arena struct itself
 *      size_t bytes_used           -   how much memory this arena has redistributed so far
 *      uint8_t* usable_base        -   ptr to first byte of memory this arena can redistribute
 *
 *  Freeing:
 *
 *  The MemoryArena_t itself sits at the beginning of the memory it owns, so any
 *  ptr to an arena can be used to free that arena
 *
 */
typedef struct PFAllocator_MemoryArena_t {
    size_t bytes_owned;
    size_t bytes_used;
    uint8_t* usable_base;
    uint8_t padding;
} PFAllocator_MemoryArena_t;


// TODO: put this in a common header or something
/**
 * @brief sets all supplied bytes to zero, but bespoke for the PFAllocator_MemoryArena_t
 *
 * @param buffer 
 * @param total_bytes 
 */
void pf_memory_arena_set_bytes_to_zero(void* buffer, size_t const total_bytes);


/**
 * @brief takes memory which the PFAllocator_MemoryArena_t will own, and then returns an initialized memory arena 
 * 
 * @param base_memory 
 * @param total_size 
 * @return 
 */
__attribute__((unused))
PFAllocator_MemoryArena_t* pf_memory_arena_create_with_memory(void* base_memory, size_t const total_size);


/**
 * @brief This macro reserves memory in a PFAllocator_MemoryArena_t and returns a ptr to that memory
 *
 * @param arena - the PFAllocator_MemoryArena_t being used 
 * @param type - the type of struct being stored here
 */
#define PF_PUSH_STRUCT(arena, type) (type *) memory_arena_push_size(arena, sizeof(type))

/**
 * @brief This macro reserves memory in a PFAllocator_MemoryArena_t, and returns a ptr to that memory
 *
 * @param arena - the PFAllocator_MemoryArena_t being used
 * @param count - how many structs should be stored here
 * @param type - the type of struct being stored here
 */
#define PF_PUSH_ARRAY(arena, count, type) (type *) memory_arena_push_size(arena, (count) * sizeof(type))


/**
 * @brief Reserves memory in a PFAllocator_MemoryArena_t and returns a ptr to that memory
 *
 * @param arena - the PFAllocator_MemoryArena_t to use
 * @param size_requested - how much memory you want 
 * @return 
 */
__attribute__((unused))
void * pf_memory_arena_push_size(PFAllocator_MemoryArena_t* arena, size_t const size_requested);


#endif //MEMORY_ARENA_H
