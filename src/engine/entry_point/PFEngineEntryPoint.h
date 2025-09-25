// paciFIST studios. 2025. MIT License.

#ifndef PF_ENGINE_ENTRY_POINT_H
#define PF_ENGINE_ENTRY_POINT_H

// stdlib
// framework
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
// engine


/**
 * Called by SDL_AppInit, and wraps all PFEngine functionality for that fn 
 */
SDL_AppResult pf_app_init(void ** appstate, int argc, char* argv[]);


/**
 * Called by SDL_AppEvent, and wraps all PFEngine functionality for that fn
 */
SDL_AppResult pf_app_event(void* app_state, SDL_Event* event);


/**
 * Called by SDL_AppResult, and wraps all PFEngine functionality for that fn
 */
SDL_AppResult pf_app_iterate(void* app_state);


/**
 * Called by SDL_AppQuit, and wraps all PFEngine functionality for that fn
 */
void pf_app_quit(void* app_state, SDL_AppResult result);


#endif // PF_ENGINE_ENTRY_POINT_H

