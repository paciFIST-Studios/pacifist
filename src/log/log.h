// paciFIST studios. 2025. MIT License


#ifndef LOG_H
#define LOG_H

// stdlib
#include <stddef.h>
// framework
#include <SDL3/SDL_log.h>
// engine




/**
 * @brief Returns date time in the form: 120250719T131700(-07), and requires 22 char 
 *
 * @param out_buffer 
 * @param out_buffer_length 
 * @return 
 */
void get_date_time_string(char * out_buffer, size_t const out_buffer_length);


// the engine supplied callback which SDL uses to write all of the SDL_log commands
void paciFIST_log(void* userdata, int category, SDL_LogPriority priority, char const * message);





#endif //LOG_H
