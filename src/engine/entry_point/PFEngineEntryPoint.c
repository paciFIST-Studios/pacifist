// paciFIST studios. 2025. MIT License

// stdlib
#include <stdio.h>
// framework
#include <SDL3/SDL_render.h>
// engine
#include <core/define.h>
#include <engine/PFEngineConfiguration.h>
#include <engine/entry_point/PFEngineEntryPoint.h>
#include <engine/entry_point/PFEngineInitialization.h>
#include <engine/project/PFProjectConfiguration.h>






//------------------------------------------------------------------------------------------------------------
// Static Variables
//------------------------------------------------------------------------------------------------------------

//static SDL_Window* s_sdl_program_window = NULL;
//static SDL_Renderer* s_sdl_renderer = NULL;

//static SDL_Surface* s_sdl_surface = NULL;
//static SDL_Texture* s_sdl_texture = NULL;

static size_t const s_engine_memory_size = Mebibytes(64);

static PFAllocator_MemoryArena_t* s_engine_lifetime_allocator = NULL;

static PFEngineConfiguration_t* s_engine_configuration = NULL;
static PFEngineState_t* s_engine_state = NULL;

//static PFProjectConfiguration_t* s_project_configuration = NULL;



//------------------------------------------------------------------------------------------------------------
// fn pf_app_init
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_init(void **appstate, int argc, char *argv[]) {


    // -------------------------------------------------------------------------------------------------------
    // Systems Init
    // -------------------------------------------------------------------------------------------------------

    if (!pf_try_initialize_sdl_video_systems (argc, argv)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize video systems!");
        return SDL_APP_FAILURE;
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
    if (!pf_try_create_engine_state_struct(s_engine_lifetime_allocator, &s_engine_state)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize engine state struct!");
        return SDL_APP_FAILURE;
    }


    void* engine_memory = NULL;
    if (!pf_try_allocate_engine_memory_from_os(s_engine_memory_size, &engine_memory)) {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not allocate engine memory!");
        return SDL_APP_FAILURE;
    }

    if (!pf_try_create_engine_lifetime_allocator(engine_memory,
        s_engine_memory_size, &s_engine_lifetime_allocator))
    {
        PF_LOG_CRITICAL(PF_APPLICATION, "Could not initialize engine lifetime allocator!");
        return SDL_APP_FAILURE;
    }
    
    // -------------------------------------------------------------------------------------------------------
    // Configuration Init
    // -------------------------------------------------------------------------------------------------------

    if (!pf_try_read_engine_configuration(argc, argv,
        s_engine_lifetime_allocator, &s_engine_configuration)) {
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


    //char const * splash_image_file_path = "/home/ellie/git/paciFIST/src/splash.bmp";
    //s_sdl_surface = SDL_LoadBMP(splash_image_file_path);
    //if (s_sdl_surface == NULL) {
    //   printf("Could not load splash image! %s\n", splash_image_file_path);
    //}

    //s_sdl_texture = SDL_CreateTextureFromSurface(s_sdl_renderer, s_sdl_surface);
    //if (s_sdl_texture == NULL) {
    //   printf("Could not create sdl texture from surface!\n");
    //}
    //SDL_DestroySurface(s_sdl_surface);
    //s_sdl_surface = NULL;

    return SDL_APP_CONTINUE;
}



//------------------------------------------------------------------------------------------------------------
// fn pf_app_event
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_event(void *app_state, SDL_Event *event) {
    return SDL_APP_CONTINUE;
}




//------------------------------------------------------------------------------------------------------------
// fn pf_app_iterate
//------------------------------------------------------------------------------------------------------------

SDL_AppResult pf_app_iterate(void *app_state) {
    return SDL_APP_SUCCESS;
}



//------------------------------------------------------------------------------------------------------------
// fn pf_app_quit
//------------------------------------------------------------------------------------------------------------

void pf_app_quit(void *app_state, SDL_AppResult result) {
}
