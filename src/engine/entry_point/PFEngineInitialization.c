// paciFIST studios. 2025. MIT License

// header
#include <engine/entry_point/PFEngineInitialization.h>
// stdlib
#include <stdlib.h>
// framework
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
// engine



static SDL_Window* s_sdl_program_window = NULL;
static SDL_Renderer* s_sdl_renderer = NULL;







void * pf_allocate_engine_memory(size_t engine_total_memory_allocation) {
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

int32_t pf_try_initialize_sdl_video_systems(int argc, char* argv[]) {
    PF_DEBUG_PARSE_FOR_RETURN_FALSE(argc, argv, "try_initialize_video_systems");

    // initialize SDL video system
    if (SDL_Init(SDL_INIT_VIDEO) == FALSE) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!",
           SDL_GetError(), NULL);
        return FALSE;
    }

    // create and initialize main window
    {
        SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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

int32_t pf_try_allocate_engine_memory_from_os(size_t memory_size, void** out_memory) {
    return TRUE;
}

int32_t pf_try_create_engine_lifetime_allocator(
    void *lifetime_memory_base,
    size_t memory_size,
    PFAllocator_MemoryArena_t** out_lifetime_allocator)
{
    return TRUE;
}

int32_t pf_try_read_engine_configuration(
    int argc,
    char *argv[],
    PFAllocator_MemoryArena_t* lifetime_allocator,
    PFEngineConfiguration_t** out_engine_configuration)
{
    return TRUE;
}

int32_t pf_try_create_engine_state_struct(
    void* memory_base,
    PFEngineState_t** out_engine_state_struct)
{
    return TRUE;
}
