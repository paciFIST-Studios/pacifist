// paciFIST Studios. 2025. MIT License.

// stdlib
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
// framework

//#include <GL/glew.h>

#define SDL_MAIN_USE_CALLBACKS 1
#define SDL_ENABLE_OLD_NAMES
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengles2.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

// nuklear imgui
//#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
//#define NK_INCLUDE_STANDARD_VARARGS
//#define NK_INCLUDE_DEFAULT_ALLOCATOR
//#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
//#define NK_INCLUDE_FONT_BAKING
//#define NK_INCLUDE_DEFAULT_FONT
//#define NK_IMPLEMENTATION
//#define NK_SDL_RENDERER_IMPLEMENTATION
//#include "imgui/nuklear.h"
//#include "imgui/pf_nuklear_sdl3_renderer.h"


// engine
#include "core/define.h"
#include "core/error.h"
#include "engine/PFEngineState.h"
#include "engine/PFEngineConfiguration.h"
#include "engine/project/PFProjectConfiguration.h"
#include "engine/entry_point/PFEngineEntryPoint.h"
#include "log/log.h"
#include "memory/allocator/PFMemoryArena.h"
#include "string/pstring.h"
#include "parse/parse_utilities.h"
// game
#include "_games/Slain/Slain.h"

/** NOTE:   With SDL3, initialization has changed to a callback system,
 *          and no longer uses the int main fn entry point.
 *
 */

static SDL_Window* s_sdl_program_window = NULL;
static SDL_Renderer* s_sdl_renderer = NULL;

__attribute__((unused))
static SDL_Surface* s_sdl_surface = NULL;
static SDL_Texture* s_sdl_texture = NULL;



//static size_t const s_engine_memory_size = Mebibytes(64);
//static PFAllocator_MemoryArena_t* s_engine_lifetime_memory_arena = NULL;

//static PFEngineConfiguration_t* s_engine_configuration = NULL;
//static PFEngineState_t* s_engine_state = NULL;

//static PFProjectConfiguration_t* s_project_configuration = NULL;

// TODO: set up cppcheck (installed plugin), cscout, and GCC static analysis, analyzer(goblint)
// TODO: set up doxygen, to run in all cmake configs


//void pacifist_log_callback(void* userdata, int category, SDL_LogPriority priority, const char* message) {
//   if (gpLogFile != NULL) {
//      fprintf(gpLogFile, "[%s] %s\n", describe_log_category(category) ,message);
//   }
//
//   printf("[%s] %s\n", describe_log_category(category), message);
//}


//void * allocate_application_memory(size_t const game_total_memory_allocation) {
//   void * memory = NULL;
//
////#if __linux__
////   // see man mmap for more details
////   // this is used in conjunction with MAP_HUGETLB,
////   // to generate larger page sizes, which will hopefully
////   // provide better cache coherency for the game
////   #define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
////   #define MAP_HUGE_16MB (24 << MAP_HUGE_SHIFT)
////   
////   // set to null so the compiler can pick wherever it likes
////   void* game_base_memory_address = (void*)Gibibytes(6);
////   
////   memory = mmap(
////      game_base_memory_address,
////      game_total_memory_allocation,
////      PROT_READ | PROT_WRITE,
////      MAP_ANONYMOUS | MAP_PRIVATE | MAP_HUGETLB | MAP_HUGE_16MB,
////      -1,
////      0
////      );
////#else
////   void * memory = malloc(game_total_memory_allocation);
////#endif
//
//   memory = malloc(game_total_memory_allocation);
//   if (memory == NULL) {
//      // error could not map memory for game, need to quit
//      return NULL;
//   }
//   
//   return memory;
//}


//void deallocate_application_memory(void* memory, size_t const size) {
////#if __linux__ 
////   // This is the ONLY place memory should be unmapped or freed from
////   munmap(memory, size);
////#else
////   free(memory);
////#endif
//   free(memory);
//}


/**
 * @note: startup arg: `-return_false=try_initialize_video_systems`
 *
 *
 * @param arg_count 
 * @param args 
 * @return 
 */
//int32_t try_initialize_video_systems(int arg_count, char* args[]) {
//    PF_DEBUG_PARSE_FOR_RETURN_FALSE(arg_count, args, "try_initialize_video_systems");
//
//   // initialize SDL video system
//   if (SDL_Init(SDL_INIT_VIDEO) == FALSE) {
//      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!",
//         SDL_GetError(), NULL);
//      return FALSE;
//   }
//
//
//   // create and initialize main window
//   {
//      SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
//      s_sdl_program_window = SDL_CreateWindow("paciFIST Studios", WINDOW_RES_X, WINDOW_RES_Y, windowFlags);
//      if (s_sdl_program_window== NULL) {
//         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize main window!", SDL_GetError(), NULL);
//         return FALSE;
//      }
//
//   }
//
//   // create and initialize software renderer
//   {
//      s_sdl_renderer = SDL_CreateRenderer(s_sdl_program_window, NULL);
//      if (s_sdl_renderer == NULL) {
//         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize renderer!", SDL_GetError(), NULL);
//         SDL_DestroyWindow(s_sdl_program_window);
//         return FALSE;
//      }
//   }
//
//   return TRUE;
//}

int32_t try_initialize_sdl_image(int arg_count, char* args[]) {
   return TRUE;
}

int32_t try_initialize_sdl_font(int arg_count, char* args[]){
   return TRUE;
}

int32_t try_initialize_sdl_audio(int arg_count, char* args[]) {
   return TRUE;
}

int32_t try_initialize_nuklear(int arg_count, char* args[]) {
//   s_nk_context = nk_sdl_init(s_sdl_program_window, s_sdl_renderer);
//   const size_t max_memory = 1024 * 16;
//   nk_init_fixed(s_nk_context, calloc(1, max_memory), max_memory, NULL);
//
//   if (nk_begin(s_nk_context, "test_window", nk_rect(0, 0, 200, 200), 0)) {
//      nk_layout_row_static(s_nk_context, 30, 80, 1);
//      if (nk_button_label(s_nk_context, "button")) {
//         PF_LOG_VERBOSE(PF_APPLICATION, "button pressed!");
//      }
//   }


   
   return TRUE;
}

int32_t try_read_engine_configuration(int arg_count, char* args[], void* engine_configuration) {

    int32_t has_set_project_name = FALSE;
    char project_name[64] = {0};
    

    
    for (uint32_t i = 0; i < arg_count; i++) {
        PString_t const argument_pstr = { .string = args[i], .length = strlen(args[i]) };

        // project argument
        if (pf_pstring_contains_char_sub(argument_pstr, "project", 7)) {
            char const * project_name_value = args[i];
            while (++project_name_value != NULL) {
                if (*project_name_value == '=') { project_name_value++; break; }
            }
            size_t const project_name_length = strnlen(project_name_value, argument_pstr.length - (project_name_value - argument_pstr.string));
            memcpy(project_name, project_name_value, project_name_length);

            if (!pf_pstring_contains_char_sub(argument_pstr, "pfproject", 9)) {
                memcpy((project_name + project_name_length), ".pfproject", 10);
            }

            has_set_project_name = TRUE;
        }

        // argument
        
    }

    // string internment for all of the parsed arguments
    if (has_set_project_name) {
        //pf_string_internment_emplace_cstr()
    }





    
    
    // if contains "-project=name"
    // split
    //  project = name
    // look for name in the files in this depot
    // if not found, return false
    // parse file
    // 

    
    return TRUE;
}


int32_t try_read_project_configuration(int arg_count, char* args[], void* project_configuration) {
    return TRUE;
}


//int32_t try_allocate_application_memory(size_t const memory_size, void** out_memory) {
//    if (memory_size == 0) {
//        return FALSE;
//    }
//    
//    // this is the only place in the program which should allocate memory
//    *out_memory = allocate_application_memory(memory_size);
//    if (out_memory == NULL) {
//        return FALSE;
//    }
//    return TRUE;
//}

int32_t try_initialize_engine_lifetime_allocator(void* program_lifetime_memory, size_t const memory_size, PFAllocator_MemoryArena_t** out_arena) {
    if (program_lifetime_memory == NULL) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Tried to initialize program lifetime allocator, but got ptr to NULL for program_lifetime_memory!");
        return FALSE;
    }
    if (memory_size == 0) {
        PF_LOG_CRITICAL(PF_INITIALIZATION, "Tried to initialize program lifetime allocator, but got memory_size of zero!");
        return FALSE;
    }

    // all of our memory is allocated now.  in order to manage it, put in a
    // memory arena aligned to byte 0
    *out_arena = pf_allocator_memory_arena_create_with_memory(program_lifetime_memory, memory_size);
    return TRUE;
}

int32_t try_initialize_engine_state_struct(void* memory_base, PFEngineState_t** out_engine_state_struct) {
    if (memory_base == NULL) {
        return FALSE;
    }

    // first allocation from within the memory area holds the engine state.
    // This struct owns the string internment struct, 
    *out_engine_state_struct = PF_PUSH_STRUCT(memory_base, PFEngineState_t);
    if (out_engine_state_struct == NULL) {
        return FALSE;
    }
    return TRUE;
}

int32_t try_initialize_game_lifetime_state_struct(void* memory, PFAllocator_MemoryArena_t* allocator,
    SlainGameLifetimeState_t** out_game_lifetime_state_struct)
{
    if (memory == NULL) {
        return FALSE;
    }

    if (allocator == NULL) {
        return FALSE;
    }
    
    // create the lifetime state of the game, which has access to the engine state, but isn't the engine state
    SlainGameLifetimeState_t* state = PF_PUSH_STRUCT(memory, SlainGameLifetimeState_t);
    if (state == NULL) {
        return FALSE;
    }

    slain_game_lifetime_state_initialize(state);
    state->program_lifetime_memory_arena = allocator;

    *out_game_lifetime_state_struct = state;

    return TRUE;
}








SDL_AppResult SDL_AppInit(void ** appstate, int argc, char* argv[]) {
    return pf_app_init(appstate, argc, argv);
}


SDL_AppResult SDL_AppEvent(void * appstate, SDL_Event * event) {
    return pf_app_event(appstate, event);
    if (appstate == NULL) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Lost access to application lifetime state!");
        return SDL_APP_FAILURE;
    }

    SlainGameLifetimeState_t* ln_game_state = appstate;

    switch (event->type) {

        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN: {
           if (event->key.key == SDLK_ESCAPE) {
              return SDL_APP_SUCCESS;
           }

           if (ln_game_state->pf_handle_key_down != NULL) {
               ln_game_state->pf_handle_key_down(event->key.key, event->key.mod);
           } else {
               PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_key_down\"!");
               return SDL_APP_FAILURE;
           }
        } break;

        case SDL_EVENT_KEY_UP: {
            if (ln_game_state->pf_handle_key_up != NULL) {
                ln_game_state->pf_handle_key_up(event->key.key, event->key.mod);
            } else {
                PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_key_up\"!");
                return SDL_APP_FAILURE;
            }
        } break;

        case SDL_MOUSEMOTION: {
            if (ln_game_state->pf_handle_mouse_movement != NULL) {
                ln_game_state->pf_handle_mouse_movement(event->motion.x, event->motion.y);
            } else {
                PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_movement\"!");
                return SDL_APP_FAILURE;
            }
        } break;

        case SDL_MOUSEBUTTONDOWN: {
            if (ln_game_state->pf_handle_mouse_down != NULL) {
                ln_game_state->pf_handle_mouse_down(event->button.button);
            } else {
                PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_down\"!");
                return SDL_APP_FAILURE;
            }
        } break;

        case SDL_MOUSEBUTTONUP: {
            if (ln_game_state->pf_handle_mouse_up != NULL) {
                ln_game_state->pf_handle_mouse_up(event->button.button);
            } else {
                PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_up\"!");
                return SDL_APP_FAILURE;
            }
        } break;

      default:
         break;
    }

    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void * appstate) {
    return pf_app_iterate(appstate);

    //SDL_Log("Hey girl! love youu!");
   //SDL_Log("You're doing your best!");

   SDL_RenderClear(s_sdl_renderer);
   SDL_FRect destination_rect;
   destination_rect.w = 828;
   destination_rect.h = 988;
   destination_rect.x = 640 - (destination_rect.w / 2);
   destination_rect.y = 0;
   SDL_RenderTexture(s_sdl_renderer, s_sdl_texture, NULL, &destination_rect);
   SDL_RenderPresent(s_sdl_renderer);

   
   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void * appstate, SDL_AppResult result) {
    pf_app_quit(appstate, result);
    return;
    //nk_end(s_nk_context);
   
   //deallocate_application_memory(s_engine_lifetime_memory_arena, s_engine_memory_size);

   SDL_DestroyRenderer(s_sdl_renderer);
   SDL_DestroyWindow(s_sdl_program_window);
}



