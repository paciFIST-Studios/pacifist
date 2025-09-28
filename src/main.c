// paciFIST Studios. 2025. MIT License.

// stdlib
// framework
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
// engine
#include "engine/entry_point/pf_engine_entry_point.h"
// game


// TODO: set up cppcheck (installed plugin), cscout, and GCC static analysis, analyzer(goblint)
// TODO: set up doxygen, to run in all cmake configs


SDL_AppResult SDL_AppInit(void ** appstate, int argc, char* argv[]) {
    return pf_app_init(appstate, argc, argv);
}


SDL_AppResult SDL_AppEvent(void * appstate, SDL_Event * event) {
    return pf_app_event(appstate, event);
}


SDL_AppResult SDL_AppIterate(void * appstate) {
    return pf_app_iterate(appstate);
}

void SDL_AppQuit(void * appstate, SDL_AppResult result) {
    pf_app_quit(appstate, result);
}


//int32_t try_read_engine_configuration(int arg_count, char* args[], void* engine_configuration) {
//
//    int32_t has_set_project_name = FALSE;
//    char project_name[64] = {0};
//    
//
//    
//    for (uint32_t i = 0; i < arg_count; i++) {
//        PString_t const argument_pstr = { .string = args[i], .length = strlen(args[i]) };
//
//        // project argument
//        if (pf_pstring_contains_char_sub(argument_pstr, "project", 7)) {
//            char const * project_name_value = args[i];
//            while (++project_name_value != NULL) {
//                if (*project_name_value == '=') { project_name_value++; break; }
//            }
//            size_t const project_name_length = strnlen(project_name_value, argument_pstr.length - (project_name_value - argument_pstr.string));
//            memcpy(project_name, project_name_value, project_name_length);
//
//            if (!pf_pstring_contains_char_sub(argument_pstr, "pfproject", 9)) {
//                memcpy((project_name + project_name_length), ".pfproject", 10);
//            }
//
//            has_set_project_name = TRUE;
//        }
//
//        // argument
//        
//    }
//
//    // string internment for all of the parsed arguments
//    if (has_set_project_name) {
//        //pf_string_internment_emplace_cstr()
//    }
//
//
//
//
//
//    
//    
//    // if contains "-project=name"
//    // split
//    //  project = name
//    // look for name in the files in this depot
//    // if not found, return false
//    // parse file
//    // 
//
//    
//    return TRUE;
//}
//
//
//
//
//int32_t try_initialize_game_lifetime_state_struct(void* memory, PFAllocator_MemoryArena_t* allocator,
//    SlainGameLifetimeState_t** out_game_lifetime_state_struct)
//{
//    if (memory == NULL) {
//        return FALSE;
//    }
//
//    if (allocator == NULL) {
//        return FALSE;
//    }
//    
//    // create the lifetime state of the game, which has access to the engine state, but isn't the engine state
//    SlainGameLifetimeState_t* state = PF_PUSH_STRUCT(memory, SlainGameLifetimeState_t);
//    if (state == NULL) {
//        return FALSE;
//    }
//
//    slain_game_lifetime_state_initialize(state);
//    state->program_lifetime_memory_arena = allocator;
//
//    *out_game_lifetime_state_struct = state;
//
//    return TRUE;
//}











