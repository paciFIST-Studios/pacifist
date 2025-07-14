// paciFIST studios. 2025. MIT License

#ifndef SNAKE_H
#define SNAKE_H

// stdlib
// framework
// engine
#include "../string/pstring.h"
#include "../memory/allocator/MemoryArena.h"


typedef struct SnakeGameState_t {
    PString_t window_title;

    MemoryArena_t * program_lifetime_memory_arena;
    
} SnakeGameState_t;



#endif //SNAKE_H
