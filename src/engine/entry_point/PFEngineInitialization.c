// paciFIST studios. 2025. MIT License

// header
#include <engine/entry_point/PFEngineInitialization.h>
// stdlib
// framework
// engine


void * pf_allocate_engine_memory(size_t engine_total_memory_allocation) {
    return NULL;
}

void pf_deallocate_engine_memory(void* memory, size_t engine_total_memory_allocation) {
}

int32_t pf_try_initialize_sdl_video_systems(int argc, char* argv[]) {
    return 0;
}

int32_t pf_try_initialize_sdl_image(int argc, char* argv[]) {
    return 0;
}

int32_t pf_try_initialize_sdl_font(int argc, char* argv[]) {
    return 0;
}

int32_t pf_try_initialize_sdl_audio(int argc, char* argv[]) {
    return 0;
}

int32_t pf_try_allocate_engine_memory_from_os(size_t memory_size, void** out_memory) {
    return 0;
}

int32_t pf_try_create_engine_lifetime_allocator(
    void *lifetime_memory_base,
    size_t memory_size,
    PFAllocator_MemoryArena_t** out_lifetime_allocator)
{
    return 0;
}

int32_t pf_try_read_engine_configuration(
    int argc,
    char *argv[],
    PFAllocator_MemoryArena_t* lifetime_allocator,
    PFEngineConfiguration_t** out_engine_configuration)
{
    return 0;
}

int32_t pf_try_create_engine_state_struct(
    void* memory_base,
    PFEngineState_t** out_engine_state_struct)
{
    return 0;
}
