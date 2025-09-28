// paciFIST studios. 2025. MIT License

// header
#include <engine/entry_point/pf_engine_initialization.h>
// stdlib
#include <stdlib.h>
// framework
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
//#include <SDL3/SDL_render.h>
// engine
#include <core/define.h>
#include <core/error.h>
#include <engine/engine_define.h>
#include <string/pstring.h>
#include <parse/parse_utilities.h>
#include <os/os_define.h>
#include <os/path.h>


static SDL_Window* s_sdl_program_window = NULL;
static SDL_Renderer* s_sdl_renderer = NULL;



//------------------------------------------------------------------------------------------------------------
// memory: move these
//------------------------------------------------------------------------------------------------------------

SDL_Window * pf_get_program_window(void) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)){
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Someone called pf_get_program_window before initializing SDL_VIDEO!");
        return NULL;
    }
    return s_sdl_program_window;
}

SDL_Renderer * pf_get_sdl_renderer(void) {
    if (!SDL_WasInit(SDL_INIT_VIDEO)){
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Someone called pf_get_sdl_renderer before initializing SDL_VIDEO!");
        return NULL;
    }
    return s_sdl_renderer;
}

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
        uint64_t const windowFlags = (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
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
int32_t pf_try_allocate_engine_memory_from_os(size_t const memory_size, void** out_memory) {
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
    PFAllocator_MemoryArena_t* engine_scope,
    PFEngineState_t** out_engine_state_struct)
{
    if (engine_scope == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got NULL ptr to allocator param!");
        return FALSE;
    }
    if (out_engine_state_struct == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got NULL ptr to out-engine-state param!");
        return FALSE;
    }

    // first allocation from within the memory area holds the engine state.
    // This struct owns the string internment struct,
    PFEngineState_t* engine_state = PF_PUSH_STRUCT(engine_scope, PFEngineState_t);
    if (engine_state == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Could not allocate from provided allocator!");
        return FALSE;
    }

    // zero it out and we'll build it fresh
    memset(engine_state, 0, sizeof(PFEngineState_t));


    //--------------------------------------------------------------------------------------------------------
    // Lifetime Allocations
    //--------------------------------------------------------------------------------------------------------
    // things which cannot return their memory until program shutdown go here
    // the last part of this section is the recoverable allocator

    //--------------------//
    // Lifetime Allocator //
    //--------------------//
    size_t const pfama_size = ENGINE_ALLOCATOR_SIZE__LIFETIME_MEMORY;
    void* pfama_memory_base = pf_allocator_memory_arena_push_size(engine_scope, pfama_size);
    PFAllocator_MemoryArena_t* lifetime_allocator = pf_allocator_memory_arena_create_with_memory(pfama_memory_base, pfama_size);
    engine_state->p_lifetime_memory_allocator = lifetime_allocator;
    
    //-------------------//
    // String Internment //
    //-------------------//
    // fist is the StringInternment singleton, which lasts the entire lifetime of the engine,
    // and which cannot deallocate an interned string

    size_t const slis_size = ENGINE_SYSTEM_SIZE__STRING_INTERNMENT_MEMORY;
    void* slis_memory_base = pf_allocator_memory_arena_push_size(lifetime_allocator, slis_size);
    PFStringLifetimeInternmentSingleton_t* string_internment = pf_string_lifetime_internment_create_with_memory(slis_memory_base, slis_size);
    engine_state->p_lifetime_string_internment = string_internment;


    //-------//
    // empty //
    //-------//





    //-----------------------//
    // Recoverable Allocator //
    //-----------------------//
    // the recoverable allocator itself is a lifetime allocation.
    // anything which can return its memory goes in the recoverable memory section

    size_t const pfafl_size = ENGINE_ALLOCATOR_SIZE__RECOVERABLE_MEMORY;
    void* pfafl_memory_base = pf_allocator_memory_arena_push_size(engine_scope, pfafl_size);
    PFAllocator_FreeList_t* pfafl_allocator = pf_allocator_free_list_create_with_memory(pfafl_memory_base, pfafl_size);
    engine_state->p_recoverable_memory_allocator = pfafl_allocator;

    //--------------------------------------------------------------------------------------------------------
    // Recoverable Memory
    //--------------------------------------------------------------------------------------------------------




    *out_engine_state_struct = engine_state;
    return TRUE;
}


//------------------------------------------------------------------------------------------------------------
// Configuration Initialization
//------------------------------------------------------------------------------------------------------------

int32_t pf_try_read_engine_configuration(
    int argc,
    char *argv[],
    PFEngineState_t const * engine_state,
    PFEngineConfiguration_t** out_engine_configuration)
{
    if (engine_state == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got null ptr to PFEngineState_t!");
        return FALSE;
    }
    if (out_engine_configuration == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Got null ptr to out_engine_configuration param!");
        return FALSE;
    }
    if (engine_state->p_lifetime_memory_allocator == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "PFEngineState_t had null ptr to lifetime_memory_allocator!");
        return FALSE;
    }
    if (engine_state->p_recoverable_memory_allocator == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "PFEngineState_t had null ptr to recoverable_memory_allocator!");
        return FALSE;
    }
    if (engine_state->p_lifetime_string_internment == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "PFEngineState_t had null ptr to lifetime_string_internment!");
        return FALSE;
    }

    PFEngineConfiguration_t* engine_configuration = PF_PUSH_STRUCT(engine_state->p_lifetime_memory_allocator, PFEngineConfiguration_t);
    PFStringLifetimeInternmentSingleton_t* string_internment = engine_state->p_lifetime_string_internment;

    // this is the path to the binary of the executing program
    PString_t program_execution_path = { .string = argv[0], .length=strlen(argv[0]) };
    pf_string_lifetime_internment_emplace_pstr(string_internment, program_execution_path);
    engine_configuration->program_execution_path = program_execution_path;


    char const slash = '/';
    size_t const slashes_in_exe_path = pf_pstring_count_character_occurrences_in_string(program_execution_path, slash);
    size_t const second_to_last_slash = slashes_in_exe_path - 1;
    size_t const second_to_last_slash_idx = pf_pstring_find_indexth_character_location(program_execution_path, slash, second_to_last_slash);

    PString_t engine_base_path = pf_pstring_slice(program_execution_path, 0, (int32_t)second_to_last_slash_idx);
    engine_base_path = pf_string_lifetime_internment_emplace_pstr(string_internment, engine_base_path);
    engine_configuration->engine_base_path = engine_base_path;

    
    //char path_join_buffer[256] = {0};
    //__attribute__((unused))
    //size_t const engine_config_path_len  = pf_os_path_join2D_pstr(
    //    path_join_buffer,
    //    engine_base_path,
    //    '/', "src/engine_config.pfengine");

    

    // take the program execution path
    // find the last two slashes('\') in it
    // cut from the second-to-last slash
    // intern that string
    // save that string as engine_base_path


    // take the engine base path
    // add engine_config.pfengine
    // intern that string
    // save that string as path to engine config

    // other parsing from engine config

    

    // this is parsing from project config
    char const * project_name = "project_name";
    char const * project_config_file_path = "project_config_file_path";

    // moar data?
    // logs?




    
    pf_string_lifetime_internment_emplace_cstr(string_internment, project_name, strlen(project_name));
    pf_string_lifetime_internment_emplace_cstr(string_internment, project_config_file_path, strlen(project_config_file_path));


    *out_engine_configuration = engine_configuration;
    return TRUE;
}

