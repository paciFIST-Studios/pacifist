// paciFIST studios. 2025. MIT License

#ifndef PF_ENGINE_INITIALIZATION_H
#define PF_ENGINE_INITIALIZATION_H

// stdlib
#include <stdint.h>
// framework
// engine
#include <engine/PFEngineState.h>
#include <engine/PFEngineConfiguration.h>
#include <log/log.h>
#include <memory/allocator/PFMemoryArena.h>

// defines ---------------------------------------------------------------------------------------------------

#define STARTUP_WINDOW_RES_X 1280
#define STARTUP_WINDOW_RES_Y 720
#define STARTUP_WINDOW_CENTER_X 640
#define STARTUP_WINDOW_CENTER_y 360


/**
 * @brief This fn allocates all of the memory used by the engine, for its entire lifetime, as a single block
 *
 * @param engine_total_memory_allocation 
 * @return 
 */
void* pf_allocate_engine_memory(size_t engine_total_memory_allocation);

/**
 * @brief This fn deallocates all of the engine memory
 * 
 * @param memory 
 * @param engine_total_memory_allocation 
 */
void pf_deallocate_engine_memory(void* memory, size_t engine_total_memory_allocation);

/**
 * 
 * @param argc 
 * @param argv 
 * @return 
 */
int32_t pf_try_initialize_sdl_video_systems(int argc, char* argv[]);

/**
 * 
 * @param argc 
 * @param argv 
 * @return 
 */
int32_t pf_try_initialize_sdl_image(int argc, char* argv[]);

/**
 * 
 * @param argc 
 * @param argv 
 * @return 
 */
int32_t pf_try_initialize_sdl_font(int argc, char* argv[]);

/**
 * 
 * @param argc 
 * @param argv 
 * @return 
 */
int32_t pf_try_initialize_sdl_audio(int argc, char* argv[]);



/**
 * @brief requests a single allocation, of all the memory the engine will own for its lifetime
 *
 * @param memory_size 
 * @param out_memory 
 * @return 
 */
int32_t pf_try_allocate_engine_memory_from_os(
    size_t memory_size,
    void** out_memory);

/**
 * @brief creates the engine lifetime allocator, using the supplied memory
 * 
 * @param lifetime_memory_base 
 * @param memory_size 
 * @param out_lifetime_allocator 
 * @return 
 */
int32_t pf_try_create_engine_lifetime_allocator(
    void* lifetime_memory_base,
    size_t memory_size,
    PFAllocator_MemoryArena_t** out_lifetime_allocator);


/**
 * @brief Creates a struct for the engine state, at the memory location
 *
 * @param allocator 
 * @param out_engine_state_struct 
 * @return 
 */
int32_t pf_try_create_engine_state_struct(
    PFAllocator_MemoryArena_t* allocator,
    PFEngineState_t** out_engine_state_struct);


/**
 * @brief reads in the engine configuration and creates the configuration object
 * 
 * @param argc 
 * @param argv
 * @param lifetime_allocator
 * @param out_engine_configuration 
 * @return 
 */
int32_t pf_try_read_engine_configuration(
    int argc,
    char* argv[],
    PFAllocator_MemoryArena_t* lifetime_allocator,
    PFEngineConfiguration_t** out_engine_configuration);

#endif // PF_ENGINE_INITIALIZATION_H



