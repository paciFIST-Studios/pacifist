// paciFIST studios. 2025. MIT License

// include
#include "log.h"

// stdlib
// framework
// engine
#include "core/define.h"
#include "date/datetime.h"

//static FILE* s_active_log_file;


void paciFIST_log(void* userdata, int category, SDL_LogPriority priority, char const * message)
{
    
}



int32_t initialize_log_file(char const * path, size_t const path_length) {
    if (path == NULL) {
        // error, invalid ptr to path
        return FALSE;
    }

    char datetime[22];
    get_datetime_string(datetime, 22);


    
    return TRUE;
}

int32_t close_log_file(char const * path, size_t const path_length) {
    if (path == NULL) {
        // error, invalid ptr to path
        return FALSE;
    }



    
    return TRUE;
}




