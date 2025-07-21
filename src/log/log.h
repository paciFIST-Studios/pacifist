// paciFIST studios. 2025. MIT License


#ifndef LOG_H
#define LOG_H

// stdlib
#include <stddef.h>
// framework
#include <SDL3/SDL_log.h>
// engine






// the engine supplied callback which SDL uses to write all of the SDL_log commands
void paciFIST_log(void* userdata, int category, SDL_LogPriority priority, char const * message);



int32_t initialize_log_file(char const * path, size_t const path_length);



int32_t close_log_file(char const * path, size_t const path_length);


#endif //LOG_H
