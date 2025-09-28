// paciFIST studios. 2025. MIT License.

// include
#include "os_utility.h"

// stdlib
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// framework
#include <SDL3/SDL_log.h>
// engine
#include "core/define.h"
#include "core/error.h"



int64_t file_size(char const * file_path, size_t const file_path_length) {
    if (file_path == NULL) {
        PF_LOG_ERROR(PF_OS, "Invalid ptr to file path");
        return PFEC_ERROR_NULL_PTR;
    }
    if (file_path_length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid file path length");
        return  PFEC_ERROR_INVALID_LENGTH;
    }
    if (!is_file(file_path, strlen(file_path))) {
        PF_LOG_ERROR(PF_OS, "File does not exist");
        return PFEC_ERROR_FILE_DOES_NOT_EXIST;
    }
    
    struct stat buffer;
    stat(file_path, &buffer); 
    return buffer.st_size;
}


int32_t is_file(const char * file_path, size_t const file_path_length) {
    if (file_path == NULL) {
        PF_LOG_ERROR(PF_OS, "Invalid ptr to file path");
        return PFEC_ERROR_NULL_PTR;
    }
    if (file_path_length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid file path length");
        return PFEC_ERROR_INVALID_LENGTH;
    } 

    struct stat buffer;
    return ( stat(file_path, &buffer) == 0 );
}


int32_t is_directory(const char * file_path, size_t const file_path_length) {
    if (file_path == NULL) {
        PF_LOG_ERROR(PF_OS, "Invalid ptr to file path");
        return PFEC_ERROR_NULL_PTR;
    }
    if (file_path_length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid file path length");
        return PFEC_ERROR_INVALID_LENGTH;
    } 

    struct stat buffer;
    if( stat(file_path, &buffer) != 0 ) {
        return FALSE;
    }
    return S_ISDIR(buffer.st_mode);
}



