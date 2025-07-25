// paciFIST studios. 2025. MIT License

// include
#include "datetime.h"

// stdlib
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "../core/error.h"
// framework
// engine


void get_datetime_string(char * out_buffer, size_t const out_buffer_length) {
    int32_t const required_buffer_len = 22;

    if (out_buffer == NULL) {
        PF_LOG_ERROR(PF_OS, "Requires valid ptr to buffer!");
        return;
    }
    
    if(out_buffer_length < required_buffer_len) {
        PF_LOG_ERROR(PF_OS, "Output buffer too small!");
        return;
    }

    for (size_t i = 0; i < required_buffer_len; i++) {
        out_buffer[i] = 0;
    }

    time_t const t = time(NULL);
    struct tm const * tm = localtime(&t);

    if (tm == NULL) {
        PF_LOG_ERROR(PF_OS, "Could not get local time!");
        return;
    }

    int32_t const year   = tm->tm_year + 1900;
    int32_t const month  = tm->tm_mon + 1;
    int32_t const day    = tm->tm_mday;
    int32_t const hour   = tm->tm_hour;
    int32_t const minute = tm->tm_min;
    int32_t const second = tm->tm_sec;
    int32_t const zone   = (int32_t)tm->tm_gmtoff / 3600;

    sprintf(out_buffer, "1%d%02d%02dT%02d%02d%02d(%03d)",
        year, month, day, hour, minute, second, zone);
}




