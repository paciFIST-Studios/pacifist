// paciFIST studios. 2025. MIT License

#ifndef MEMORY_ARENA_H
#define MEMORY_ARENA_H

// stdlib
#include <stdlib.h>
// framework
// project
#include "../../core/common.h"



/**
 * @brief  A linear allocator, which can portion out memory for smaller uses.
 *
 *  A MemoryArena_t is a linear allocator which begins life by being initialized
 *  with some memory.  During its lifetime, uses of PUSH_STRUCT and PUSH_ARRAY,
 *  can request memory from the arena.
 *
 *  The MemoryArena_t does not free any of its memory, until the end of its own
 *  lifetime, when the user should dispose of the memory granted to the arena,
 *  and let the arena itself fall out of scope.
 *
 *      sie_t bytes_owned           -   how many bytes of memory belong to this arena
 *      sie_t bytes_used            -   how much memory this arena has redistributed so far
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
} MemoryArena_t;


/**
 *  @brief creates a memory arena of a given size, using a call to malloc 
 *
 * @param usable_bytes 
 * @return 
 */
static MemoryArena_t* memory_arena_create(size_t const usable_bytes) {
    size_t const total_allocation = sizeof(MemoryArena_t) + usable_bytes;

    void* mem = malloc(total_allocation);
    if (mem == NULL) {
        // error, could not allocate memory from os
        return NULL;
    }

    // pre-initialize all of this to zero, case the user is going to forget
    memset(mem, 0, total_allocation);

    MemoryArena_t* arena = (MemoryArena_t*)mem;
    arena->usable_base = (uint8_t*)mem + sizeof(MemoryArena_t);
    arena->bytes_owned = usable_bytes;
    arena->bytes_used = 0;
    
    return arena;
}



static void memory_arena_destroy(MemoryArena_t* arena) {
    // zero out all of this information, so if anyone is using it still,
    // it'll hopefully be obvious.
    memset(arena, 0, sizeof(MemoryArena_t) + arena->bytes_owned);
    
    free(arena);
}


//internal void initialize_arena(MemoryArena_t* arena, size_t size, uint8* base) {
//    // WError=type-limits
//    if (arena != NULL) {
//        arena->bytes_owned = size;
//        arena->allocation_base = base;
//        arena->bytes_used = 0;
//    }
//}



#define PUSH_STRUCT(arena, type) (type *) _push_size(arena, sizeof(type))
#define PUSH_ARRAY(arena, count, type) (type *) _push_size(arena, (count) * sizeof(type))
inline void * _push_size(MemoryArena_t* arena, MemoryIndex_t const size) {
    // arena used + next alloc less than total arena size
    ASSERT_MESSAGE(arena->bytes_used + size < arena->bytes_owned, "");
    void* result = arena->usable_base + arena->bytes_used;
    arena->bytes_used += size;
    return result;
}

#endif //MEMORY_ARENA_H
