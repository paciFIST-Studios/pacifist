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
 *      MemoryIndex_t size  -   how many bytes of memory belong to this arena
 *      MemoryIndex_t used  -   how much memory this arena has redistributed so far
 *      uint8* base         -   the pointer to the first byte of owned memory   
 *  
 */
typedef struct MemoryArena_t {
    MemoryIndex_t size;
    MemoryIndex_t used;
    uint8* base;
} MemoryArena_t;


internal void* memory_arena_create(size_t const usable_bytes) {
    size_t const total_allocation = sizeof(MemoryArena_t) + usable_bytes;
    
    void* mem = calloc(total_allocation,1 ); 
    if (mem == NULL) {
        // error, could not allocate memory from os
        return NULL;
    }

    MemoryArena_t* arena = (MemoryArena_t*)mem;
    arena->base = (uint8*)mem + sizeof(MemoryArena_t);
    arena->size = usable_bytes;
    arena->used = 0;
    
    return arena;
}



//inline void memory_arena_destroy(MemoryArena_t* arena) {
//    free(arena->base);
//    free(arena);
//}


internal void initialize_arena(MemoryArena_t* arena, MemoryIndex_t size, uint8* base) {
    // WError=type-limits
    if (arena != NULL) {
        arena->size = size;
        arena->base = base;
        arena->used = 0;
    }
}



#define PUSH_STRUCT(arena, type) (type *) _push_size(arena, sizeof(type))
#define PUSH_ARRAY(arena, count, type) (type *) _push_size(arena, (count) * sizeof(type))
inline void * _push_size(MemoryArena_t* arena, MemoryIndex_t const size) {
    // arena used + next alloc less than total arena size
    ASSERT_MESSAGE(arena->used + size < arena->size, "");
    void* result = arena->base + arena->used;
    arena->used += size;
    return result;
}

#endif //MEMORY_ARENA_H
