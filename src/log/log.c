// paciFIST studios. 2025. MIT License

// include
#include "log.h"

// stdlib
#include <stdio.h>
#include <time.h>
// framework
// engine


//static FILE* s_active_log_file;




void get_date_time_string(char * out_buffer, size_t const out_buffer_length) {
    int32_t const required_buffer_len = 22;

    if(out_buffer == NULL || out_buffer_length < required_buffer_len) {
        return;
    }

    for (size_t i = 0; i < required_buffer_len; i++) {
        out_buffer[i] = 0;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    int32_t const year   = tm->tm_year + 1900;
    int32_t const month  = tm->tm_mon + 1;
    int32_t const day    = tm->tm_mday;
    int32_t const hour   = tm->tm_hour;
    int32_t const minute = tm->tm_min;
    int32_t const second = tm->tm_sec;
    int32_t const zone   = tm->tm_gmtoff / 3600;

    sprintf(out_buffer, "1%d%02d%02dT%02d%02d%02d(%03d)",
        year, month, day, hour, minute, second, zone);
}



void paciFIST_log(void* userdata, int category, SDL_LogPriority priority, char const * message)
{
    
}



