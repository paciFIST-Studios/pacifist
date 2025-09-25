// paciFIST studios. 2025. MIT License

// header
#include <engine/entry_point/pf_engine_entry_point.h>

// stdlib
#include <stdio.h>
// framework
#include <SDL3/SDL_render.h>
// engine
#include <core/define.h>
#include <engine/engine_define.h>
#include <engine/PFEngineConfiguration.h>
#include <engine/entry_point/pf_engine_initialization.h>
#include <engine/project/PFProjectConfiguration.h>






//------------------------------------------------------------------------------------------------------------
// Static Variables
//------------------------------------------------------------------------------------------------------------

static SDL_Window* s_sdl_program_window = NULL;
static SDL_Renderer* s_sdl_renderer = NULL;

static SDL_Surface* s_sdl_surface = NULL;
static SDL_Texture* s_sdl_texture = NULL;

// the total amount of memory which will be requested from the OS for the engine (including game)
static size_t const s_engine_memory_size = ENGINE_MEMORY_TOTAL_ALLOCATION_SIZE;
// the base of all engine memory
static void* s_engine_memory_base = NULL;

/**
 * While technically an allocator, the EngineLifetimeScope, cannot deallocate,
 * so it enforces the validity of a single allocation from the os.  When the
 * program shuts down, this block is freed, and this scope exits.
 */
static PFAllocator_MemoryArena_t* s_engine_lifetime_scope = NULL;

static PFEngineConfiguration_t* s_engine_configuration = NULL;
static PFEngineState_t* s_engine_state = NULL;

//static PFProjectConfiguration_t* s_project_configuration = NULL;



//------------------------------------------------------------------------------------------------------------
// fn pf_app_init
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_init(void **appstate, int argc, char *argv[]) {
    // Systems Init - get SDL running
    // Engine Init - get engine running
    // Config Init - figure out the metadata
    // App Init - get game running


    
    // -------------------------------------------------------------------------------------------------------
    // Systems Init
    // -------------------------------------------------------------------------------------------------------

    if (!pf_try_initialize_sdl_video_systems(argc, argv)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize video systems!");
        return SDL_APP_FAILURE;
    } else {
        // these results are not returned from try init video, so just call them
        s_sdl_program_window = pf_get_program_window();
        s_sdl_renderer = pf_get_sdl_renderer();
    }


    if (!pf_try_initialize_sdl_image(argc, argv)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize SDL_Image!");
        return SDL_APP_FAILURE;
    }

    if (!pf_try_initialize_sdl_font(argc, argv)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize SDL_Font!");
        return SDL_APP_FAILURE;
    }

    if(!pf_try_initialize_sdl_audio(argc, argv)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize SDL_Audio!");
        return SDL_APP_FAILURE;
    }

    //if (!try_initialize_nuklear(argc, argv)) {
    //    PF_LOG_CRITICAL(PF_APPLICATION, "");
    //    return SDL_APP_FAILURE;
    //}


    // -------------------------------------------------------------------------------------------------------
    // Engine Init
    // -------------------------------------------------------------------------------------------------------
    if (!pf_try_allocate_engine_memory_from_os(s_engine_memory_size, &s_engine_memory_base)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not allocate engine memory!");
        return SDL_APP_FAILURE;
    }

    if (!pf_try_create_engine_lifetime_allocator(s_engine_memory_base,
        s_engine_memory_size, &s_engine_lifetime_scope))
    {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize engine lifetime allocator!");
        return SDL_APP_FAILURE;
    }

    if (!pf_try_create_engine_state_struct(s_engine_lifetime_scope, &s_engine_state)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize engine state struct!");
        return SDL_APP_FAILURE;
    }



    // -------------------------------------------------------------------------------------------------------
    // Configuration Init
    // -------------------------------------------------------------------------------------------------------

    if (!pf_try_read_engine_configuration(argc, argv, s_engine_state, &s_engine_configuration)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not read engine configuration!");
        return SDL_APP_FAILURE;
    }

    //if (!pf_try_read_project_configuration(argc, argv, &s_project_configuration)) {
    //    PF_LOG_CRITICAL(PF_APPLICATION, "Could not read project configuration!");
    //    return SDL_APP_FAILURE;
    //}


    // -------------------------------------------------------------------------------------------------------
    // Application init
    // -------------------------------------------------------------------------------------------------------

    // TODO: move this into the project files
    
    //SlainGameLifetimeState_t* slain_game_state = NULL;
    //if (!try_initialize_game_lifetime_state_struct(s_engine_lifetime_memory_arena,
    //    s_engine_lifetime_memory_arena, &slain_game_state)) {
    //    PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize game lifetime state struct!")
    //    return SDL_APP_FAILURE;
    //}


    //// we'll use the game state struct to handle all the calls from outside of the game:
    //// hardware, os, etc
    //*appstate = slain_game_state;

    *appstate = s_engine_state;

    s_sdl_renderer = pf_get_sdl_renderer();
    
    char const * splash_image_file_path = "/home/ellie/git/paciFIST/src/splash.bmp";
    s_sdl_surface = SDL_LoadBMP(splash_image_file_path);
    if (s_sdl_surface == NULL) {
       printf("Could not load splash image! %s\n", splash_image_file_path);
    }

    s_sdl_texture = SDL_CreateTextureFromSurface(s_sdl_renderer, s_sdl_surface);
    if (s_sdl_texture == NULL) {
       printf("Could not create sdl texture from surface!\n");
    }
    SDL_DestroySurface(s_sdl_surface);
    s_sdl_surface = NULL;

    return SDL_APP_CONTINUE;
}



//------------------------------------------------------------------------------------------------------------
// fn pf_app_event
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_event(void* app_state, SDL_Event* event) {
    if (app_state == NULL) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Lost access to application lifetime state!");
        return SDL_APP_FAILURE;
    }

    //SlainGameLifetimeState_t* ln_game_state = appstate;

    switch (event->type) {

        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN: {
           if (event->key.key == SDLK_ESCAPE) {
              return SDL_APP_SUCCESS;
           }

           //if (ln_game_state->pf_handle_key_down != NULL) {
           //    ln_game_state->pf_handle_key_down(event->key.key, event->key.mod);
           //} else {
           //    PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_key_down\"!");
           //    return SDL_APP_FAILURE;
           //}
        } break;

        case SDL_EVENT_KEY_UP: {
            //if (ln_game_state->pf_handle_key_up != NULL) {
            //    ln_game_state->pf_handle_key_up(event->key.key, event->key.mod);
            //} else {
            //    PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_key_up\"!");
            //    return SDL_APP_FAILURE;
            //}
        } break;

        case SDL_EVENT_MOUSE_MOTION: {
            //if (ln_game_state->pf_handle_mouse_movement != NULL) {
            //    ln_game_state->pf_handle_mouse_movement(event->motion.x, event->motion.y);
            //} else {
            //    PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_movement\"!");
            //    return SDL_APP_FAILURE;
            //}
        } break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            //if (ln_game_state->pf_handle_mouse_down != NULL) {
            //    ln_game_state->pf_handle_mouse_down(event->button.button);
            //} else {
            //    PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_down\"!");
            //    return SDL_APP_FAILURE;
            //}
        } break;

        case SDL_EVENT_MOUSE_BUTTON_UP: {
            //if (ln_game_state->pf_handle_mouse_up != NULL) {
            //    ln_game_state->pf_handle_mouse_up(event->button.button);
            //} else {
            //    PF_LOG_CRITICAL(PF_APPLICATION, "Application is missing function: \"pf_handle_mouse_up\"!");
            //    return SDL_APP_FAILURE;
            //}
        } break;

      default:
         break;
    }

    return SDL_APP_CONTINUE;
}




//------------------------------------------------------------------------------------------------------------
// fn pf_app_iterate
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_iterate(void* app_state) {
    //SDL_Log("Hey girl! love youu!");
    //SDL_Log("You're doing your best!");
    //return SDL_APP_SUCCESS;


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



//------------------------------------------------------------------------------------------------------------
// fn pf_app_quit
//------------------------------------------------------------------------------------------------------------

void pf_app_quit(void* app_state, SDL_AppResult result) {
    pf_deallocate_engine_memory(s_engine_memory_base, s_engine_memory_size);

    SDL_DestroyRenderer(s_sdl_renderer);
    s_sdl_renderer = NULL;

    SDL_DestroyWindow(s_sdl_program_window);
    s_sdl_program_window = NULL;
}
