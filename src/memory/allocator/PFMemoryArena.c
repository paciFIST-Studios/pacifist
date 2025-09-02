// paciFIST studios. 2025. MIT License

// include
#include "../../memory/allocator/PFMemoryArena.h"

// stdlib
// framework
// engine
#include "../../core/assert.h"
#include "../../core/error.h"



void pf_memory_arena_set_bytes_to_zero(void* buffer, size_t const total_bytes) {
    if (buffer == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot clear out memory! Got ptr to NULL!");
        return;
    }
    
    for(int32_t i = 0; i < total_bytes; i++) {
        uint8_t* byte = (uint8_t*)buffer + i;
        *byte = 0;
    } 
}


__attribute__((unused))
PFAllocator_MemoryArena_t* pf_memory_arena_create_with_memory(void* base_memory, size_t const total_size) {
    if (base_memory == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot create memory arena without ptr to valid memory!");
        return NULL;
    }
    
    pf_memory_arena_set_bytes_to_zero(base_memory, total_size);

    size_t const arena_size = sizeof(PFAllocator_MemoryArena_t);

    PFAllocator_MemoryArena_t* arena = base_memory;
    uint8_t* memory = base_memory;

    arena->usable_base = (uint8_t*)((uint64_t)memory + (uint64_t)arena_size);
    arena->bytes_owned = total_size;
    arena->bytes_used = 0;
    return arena; 
}




void * pf_memory_arena_push_size(PFAllocator_MemoryArena_t* arena, size_t const size_requested) {
    if (arena == NULL) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot push data to PFAllocator_MemoryArena_t, without valid ptr to arena!");
        return NULL;
    }

    if (size_requested == 0) {
        PF_LOG_CRITICAL(PF_ALLOCATOR, "Cannot fulfill a memory request of 0 bytes!");
        return NULL;
    }

    size_t const available_memory = arena->bytes_owned - arena->bytes_used - sizeof(PFAllocator_MemoryArena_t);
    if (size_requested >= available_memory) {
        char error_message[128];
        sprintf(error_message, "Cannot push data to PFAllocator_MemoryArena_t. Not enough memory! Request: %lu bytes, have: %lu bytes", size_requested, available_memory);
        PF_LOG_CRITICAL(PF_ALLOCATOR, error_message);
        return NULL;
    }


    void* result = arena->usable_base + arena->bytes_used;
    arena->bytes_used += size_requested;
    return result;
}
