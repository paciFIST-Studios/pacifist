// paciFIST studios. 2025. MIT License

// header
#include <engine/entry_point/PFEngineInitialization.h>
// stdlib
#include <stdlib.h>
// framework
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
// engine
#include <core/define.h>
#include <core/error.h>
#include <string/pstring.h>
#include <parse/parse_utilities.h>



static SDL_Window* s_sdl_program_window = NULL;
static SDL_Renderer* s_sdl_renderer = NULL;



//------------------------------------------------------------------------------------------------------------
// memory: move these
//------------------------------------------------------------------------------------------------------------

// FIXME: move these allocate and deallocate fns somewhere else
void * pf_allocate_engine_memory(size_t const engine_total_memory_allocation) {
    void * memory = NULL;

    //#if __linux__
    //   // see man mmap for more details
    //   // this is used in conjunction with MAP_HUGETLB,
    //   // to generate larger page sizes, which will hopefully
    //   // provide better cache coherency for the game
    //   #define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
    //   #define MAP_HUGE_16MB (24 << MAP_HUGE_SHIFT)
    //   
    //   // set to null so the compiler can pick wherever it likes
    //   void* game_base_memory_address = (void*)Gibibytes(6);
    //   
    //   memory = mmap(
    //      game_base_memory_address,
    //      game_total_memory_allocation,
    //      PROT_READ | PROT_WRITE,
    //      MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB | MAP_HUGE_16MB,
    //      -1,
    //      0
    //      );
    //#else
    //   void * memory = malloc(game_total_memory_allocation);
    //#endif

    memory = malloc(engine_total_memory_allocation);
    if (memory == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Could not allocate memory from OS!  Cannot continue.");
        return NULL;
    }
   
    return memory;
}

void pf_deallocate_engine_memory(void* memory, size_t engine_total_memory_allocation) {
    //#if __linux__ 
    //   // This is the ONLY place memory should be unmapped or freed from
    //   munmap(memory, size);
    //#else
    //   free(memory);
    //#endif
    free(memory);
}


//------------------------------------------------------------------------------------------------------------
// SDL initialization
//------------------------------------------------------------------------------------------------------------

int32_t pf_try_initialize_sdl_video_systems(int argc, char* argv[]) {
    // initialize SDL video system
    if (SDL_Init(SDL_INIT_VIDEO) == FALSE) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!",
           SDL_GetError(), NULL);
        return FALSE;
    }

    // create and initialize main window
    {
        SDL_WindowFlags const windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
        s_sdl_program_window = SDL_CreateWindow("paciFIST Studios", STARTUP_WINDOW_RES_X, STARTUP_WINDOW_RES_Y, windowFlags);
        if (s_sdl_program_window== NULL) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize main window!", SDL_GetError(), NULL);
            return FALSE;
        }
    }

    // create and initialize software renderer
    {
        s_sdl_renderer = SDL_CreateRenderer(s_sdl_program_window, NULL);
        if (s_sdl_renderer == NULL) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize renderer!", SDL_GetError(), NULL);
            SDL_DestroyWindow(s_sdl_program_window);
            return FALSE;
        }
    }

    return TRUE;
}

int32_t pf_try_initialize_sdl_image(int argc, char* argv[]) {
    return TRUE;
}

int32_t pf_try_initialize_sdl_font(int argc, char* argv[]) {
    return TRUE;
}

int32_t pf_try_initialize_sdl_audio(int argc, char* argv[]) {
    return TRUE;
}


//------------------------------------------------------------------------------------------------------------
// Engine Initialization
//------------------------------------------------------------------------------------------------------------

/**
 * @brief this fn manages the process of a request for memory
 *
 * @param memory_size 
 * @param out_memory 
 * @return 
 */
int32_t pf_try_allocate_engine_memory_from_os(size_t memory_size, void** out_memory) {
    if (out_memory == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Need an out-param in order to allocate engine memory from os!");
        return FALSE;
    }
    if (memory_size == 0) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Cannot allocate 0 bytes for engine memory!");
        return FALSE;
    }

    *out_memory = pf_allocate_engine_memory(memory_size);
    if (*out_memory == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Fn pf_allocate_engine_memory has returned NULL!");
        return FALSE;
    }

    return TRUE;
}

/**
 * @brief this fn creates a single, lifetime allocator, to manage all of the engine memory
 * @note: the lifetime allocator cannot deallocate, and this memory is held until
 * engine shutdown
 *
 * @param lifetime_memory_base 
 * @param memory_size 
 * @param out_lifetime_allocator 
 * @return 
 */
int32_t pf_try_create_engine_lifetime_allocator(
    void* lifetime_memory_base,
    size_t const memory_size,
    PFAllocator_MemoryArena_t** out_lifetime_allocator)
{
    if (lifetime_memory_base == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got NULL ptr for lifetime-memory-base param!");
        return FALSE;
    }
    if (memory_size == 0) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got zero for memory-size param!");
        return FALSE;
    }
    if (out_lifetime_allocator == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got null ptr for out-lifetime-allocator param!");
        return FALSE;
    }

    // all of our memory is allocated now.  in order to manage it, put in a
    // memory arena aligned to byte 0
    *out_lifetime_allocator = pf_allocator_memory_arena_create_with_memory(lifetime_memory_base, memory_size);
    return TRUE;
}

int32_t pf_try_create_engine_state_struct(
    void* memory_base,
    PFEngineState_t** out_engine_state_struct)
{
    if (memory_base == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got NULL ptr to memory-base param!");
        return FALSE;
    }
    if (out_engine_state_struct == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got NULL ptr to out-engine-state param!");
        return FALSE;
    }

    // first allocation from within the memory area holds the engine state.
    // This struct owns the string internment struct, 
    *out_engine_state_struct = PF_PUSH_STRUCT(memory_base, PFEngineState_t);
    memset(*out_engine_state_struct, 0, sizeof(PFEngineState_t));
    if (out_engine_state_struct == NULL) {
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------------------------------------
// Configuration Initialization
//------------------------------------------------------------------------------------------------------------

int32_t pf_try_read_engine_configuration(
    int argc,
    char *argv[],
    PFAllocator_MemoryArena_t* lifetime_allocator,
    PFEngineConfiguration_t** out_engine_configuration)
{
    return TRUE;
}

