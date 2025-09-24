// paciFIST studios. 2025. MIT License

#ifndef PACIFIST_ENGINE_DEFINE_H
#define PACIFIST_ENGINE_DEFINE_H

// stdio
// framework
// engine
#include <core/define.h>

// 100% of the memory
#define ENGINE_MEMORY_TOTAL_ALLOCATION_SIZE Kibibytes(256)

// these two should (about) equal the total allocation
#define ENGINE_ALLOCATOR_SIZE__LIFETIME_MEMORY Kibibytes(128)
// there's some wonkiness with the allocator here, I don't know why it's doing this
#define ENGINE_ALLOCATOR_SIZE__RECOVERABLE_MEMORY Kibibytes(128) - sizeof(PFAllocator_FreeList_t)

// the engine systems should all live within the lifetime memory
#define ENGINE_SYSTEM_SIZE__STRING_INTERNMENT_MEMORY Kibibytes(32)
// 32
// 32
// 32





#endif //PACIFIST_ENGINE_DEFINE_H