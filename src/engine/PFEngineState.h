// paciFIST studios. 2025. MIT License

#ifndef PF_ENGINE_STATE_H
#define PF_ENGINE_STATE_H

// stdlib
// framework
// engine
#include <core/error.h>
#include <memory/allocator/PFAllocator_FreeList.h>
#include <memory/allocator/PFMemoryArena.h>
#include <string/PFStringInternmentSingleton.h>
#include <engine/PFEngineConfiguration.h>

typedef struct PFEngineState_t {
    // has memory which cannot be deallocated
    PFStringLifetimeInternmentSingleton_t* p_lifetime_string_internment;

    PFAllocator_MemoryArena_t* p_lifetime_memory_allocator;
    
    // manages memory which can be deallocated
    PFAllocator_FreeList_t* p_recoverable_memory_allocator;

    //PFEngineConfiguration_t* engine_configuration;
} PFEngineState_t;





int32_t pf_engine_state_initialize(PFEngineState_t * engine_state);

int32_t pf_engine_state_destroy(PFEngineState_t * engine_state);

int32_t pf_engine_state_save(PFEngineState_t const * engine_state);

int32_t pf_engine_state_load(PFEngineState_t * engine_state);

#endif //PF_ENGINE_STATE_H
