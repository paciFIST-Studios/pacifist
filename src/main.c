// paciFIST Studios. 2025. MIT License.

// stdlib
// framework
#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_main.h"
// project
#include "core/common.h"

/** NOTE:   With SDL3, initialization has changed to a callback system,
 *          and no longer uses the int main fn entry point.
 *
 */
global_variable SDL_Window* gpWindow = NULL;
global_variable SDL_Renderer* gpRenderer = NULL;

global_variable FILE* gpLogFile = NULL;



//void pacifist_log_callback(void* userdata, int category, SDL_LogPriority priority, const char* message) {
//   if (gpLogFile != NULL) {
//      fprintf(gpLogFile, "[%s] %s\n", describe_log_category(category) ,message);
//   }
//
//   printf("[%s] %s\n", describe_log_category(category), message);
//}


SDL_AppResult SDL_AppInit(void ** pAppState, int argc, char* argv[]) {
   if (SDL_Init(SDL_INIT_VIDEO) == FALSE) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
      return SDL_APP_FAILURE;
   }   

   // todo: allocate the base memory arena here
   // todo: make a monolithic program state struct, and give it to pAppState
   
   return SDL_APP_CONTINUE; 
}


SDL_AppResult SDL_AppEvent(void * pAppState, SDL_Event * event) {

   return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void * pAppState) {
   SDL_Log("Hey girl! love youu!");
   
   return SDL_APP_SUCCESS;
}

void SDL_AppQuit(void * pAppState, SDL_AppResult result) {

   // todo: free memory here
}



