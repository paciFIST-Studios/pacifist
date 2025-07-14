// paciFIST studios. 2025. MIT License

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

// stdlib
#include <stdlib.h>
// framework
// engine
#include "../../core/assert.h"
#include "../../core/common.h"

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
typedef struct MemoryArena_t {
    size_t bytes_owned;
    size_t bytes_used;
    uint8_t* usable_base;
    uint8_t padding;
} MemoryArena_t;


static void memory_arena_set_bytes_to_zero(void* buffer, size_t const total_bytes) {
    if (buffer == NULL) {
        return;    
    }
    
    for(int32_t i = 0; i < total_bytes; i++) {
        uint8_t* byte = (uint8_t*)buffer + i;
        *byte = 0;
    } 
}


static MemoryArena_t* memory_arena_create_with_memory(void* base_memory, size_t const total_size) {
    memory_arena_set_bytes_to_zero(NULL, total_size);

    size_t const arena_size = sizeof(MemoryArena_t);

    MemoryArena_t* arena = base_memory;
    uint8_t* memory = base_memory;
    
    arena->usable_base = (uint8_t*)((uint64_t)memory + (uint64_t)arena_size);
    arena->bytes_owned = total_size;
    arena->bytes_used = 0;
    return arena; 
}



///**
// *  @brief creates a memory arena of a given size, using a call to a supplied allocator 
// *
// * @param usable_bytes
// * @param allocator_fn - can be called with size_t argument, for the number of usable
// *                       bytes requested.  32 bytes exist at the front of this allocation
// *                       to contain the arena struct itself
// * @return 
// */
//static MemoryArena_t* memory_arena_create_with_allocator(size_t const usable_bytes, AllocatorFunction_t allocator_fn) {
//    size_t const total_allocation = sizeof(MemoryArena_t) + usable_bytes;
//
//    void* mem = allocator_fn(total_allocation);
//    if (mem == NULL) {
//        return NULL;
//    }
//
//    memory_arena_set_bytes_to_zero(mem, total_allocation);
//
//    MemoryArena_t* arena = (MemoryArena_t*)mem;
//    arena->usable_base = (uint8_t*)mem + sizeof(MemoryArena_t);
//    arena->bytes_owned = total_allocation;
//    arena->bytes_used = 0;
//    
//    return arena;
//}
//
//
///**
// *  @brief creates a memory arena of a given size, using a call to malloc 
// *
// * @param usable_bytes 
// * @return 
// */
//static MemoryArena_t* memory_arena_create(size_t const usable_bytes) {
//    return memory_arena_create_with_allocator(usable_bytes, malloc);
//}


//static void memory_arena_destroy(MemoryArena_t* arena) {
//    // zero out all of this information, so if anyone is using it still,
//    // it'll hopefully be obvious that the memory has been freed.
//    memory_arena_set_bytes_to_zero(arena, arena->bytes_owned);
//    free(arena);
//}


#define PUSH_STRUCT(arena, type) (type *) _memory_arena_push_size(arena, sizeof(type))
#define PUSH_ARRAY(arena, count, type) (type *) _memory_arena_push_size(arena, (count) * sizeof(type))
static void * _memory_arena_push_size(MemoryArena_t* arena, size_t const size) {
    // arena used + next alloc less than total arena size
    ASSERT_MESSAGE(((arena->bytes_used + size) < (arena->bytes_owned - sizeof(MemoryArena_t))), "");
    void* result = arena->usable_base + arena->bytes_used;
    arena->bytes_used += size;
    return result;
}

#endif //MEMORY_ARENA_H
