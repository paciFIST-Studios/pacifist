// paciFIST Studios. 2025. MIT License.

// stdlib
#include <stdlib.h>
#include <sys/mman.h>
// framework
#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"
// engine
#include "core/define.h"
#include "core/error.h"
#include "engine/PFEngineState.h"
#include "log/log.h"
#include "memory/allocator/MemoryArena.h"
// game
#include "_games/snake.h"



/** NOTE:   With SDL3, initialization has changed to a callback system,
 *          and no longer uses the int main fn entry point.
 *
 */
//global_variable SDL_Window* gpWindow = NULL;
//global_variable SDL_Renderer* gpRenderer = NULL;
//
//global_variable FILE* gpLogFile = NULL;

static size_t const s_application_memory_size = Mebibytes(64);
static MemoryArena_t* s_program_lifetime_memory_arena = NULL;

static PFEngineState_t * s_engine_state = NULL;


// TODO: set up cppcheck (installed plugin), cscout, and GCC static analysis, analyzer(goblint)
// TODO: set up doxygen, to run in all cmake configs


//void pacifist_log_callback(void* userdata, int category, SDL_LogPriority priority, const char* message) {
//   if (gpLogFile != NULL) {
//      fprintf(gpLogFile, "[%s] %s\n", describe_log_category(category) ,message);
//   }
//
//   printf("[%s] %s\n", describe_log_category(category), message);
//}


void * allocate_application_memory(size_t const game_total_memory_allocation) {
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

   memory = malloc(game_total_memory_allocation);
   if (memory == NULL) {
      // error could not map memory for game, need to quit
      return NULL;
   }
   
   return memory;
}


void deallocate_application_memory(void* memory, size_t const size) {
//#if __linux__ 
//   // This is the ONLY place memory should be unmapped or freed from
//   munmap(memory, size);
//#else
//   free(memory);
//#endif
   free(memory);
}


SDL_AppResult SDL_AppInit(void ** appstate, int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == FALSE) {
       SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!",
          SDL_GetError(), NULL);
       return SDL_APP_FAILURE;
    }   

    // this is the only place in the program which should allocate memory
    void* application_memory = allocate_application_memory(s_application_memory_size);
    if (application_memory == NULL) {
       // cannot run program without memory allocation, shutting down
       return SDL_APP_FAILURE;
    }

    // all of our memory is allocated now.  in order to manage it, put in a
    // memory arena aligned to byte 0
    s_program_lifetime_memory_arena = memory_arena_create_with_memory(
       application_memory, s_application_memory_size);

   // first allocation from within the memory area holds the engine state.
   // This struct owns the string internment struct, 
    s_engine_state = PUSH_STRUCT(s_program_lifetime_memory_arena, PFEngineState_t);

   
    // 
    // first allocation from within the memory arena is the game state,
    // which includes a pointer to the arena itself,
    // so more things can be allocated from the arena later.
    SnakeGameState_t * snake_game_state = PUSH_STRUCT(s_program_lifetime_memory_arena, SnakeGameState_t);
    snake_game_state->program_lifetime_memory_arena = s_program_lifetime_memory_arena;

    // since we have a pointer to the arena living inside the game state, just set the game state as the app state
    *appstate = snake_game_state;





    
    return SDL_APP_CONTINUE; 
}


SDL_AppResult SDL_AppEvent(void * appstate, SDL_Event * event) {

   return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void * appstate) {
   SDL_Log("Hey girl! love youu!");
   SDL_Log("You're doing your best!");

   // end program
   return SDL_APP_SUCCESS;
}

void SDL_AppQuit(void * appstate, SDL_AppResult result) {
   deallocate_application_memory(s_program_lifetime_memory_arena, s_application_memory_size);

}



