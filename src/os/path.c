// paciFIST studios. 2025. MIT License

// header
#include <os/path.h>
// stdlib
#include <stdio.h>
#include <sys/stat.h>
// framework
// engine
#include <core/define.h>
#include <core/error.h>










int32_t pf_os_path_exists_cstr_linux(char const * path, size_t const length) {
    if (path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to path!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid path length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return FALSE;
    }

    return TRUE;
}

int32_t pf_os_path_exists_pstr_linux(PString_t const pstr) {
    return pf_os_path_exists_cstr_linux(pstr.string, pstr.length);
}



int32_t pf_os_path_is_file_cstr_linux(char const * path, size_t length) {
    if (path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to path!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid path length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return FALSE;
    }

    return S_ISREG(path_stat.st_mode);
}


int32_t pf_os_path_is_file_pstr_linux(PString_t const pstr) {
    return pf_os_path_is_file_cstr_linux(pstr.string, pstr.length);
}

int32_t pf_os_path_is_directory_cstr_linux(char const * path, size_t length) {
    if (path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to path!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid path length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return FALSE;
    }

    return S_ISDIR(path_stat.st_mode);
}

int32_t pf_os_path_is_directory_pstr_linux(PString_t const pstr) {
    return pf_os_path_is_directory_cstr_linux(pstr.string, pstr.length);
}

size_t pf_os_file_size_cstr_linux(char const *path, size_t length) {
    if (path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to path!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (length == 0) {
        PF_LOG_ERROR(PF_OS, "Invalid path length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return (size_t) -1;
    }

    if (S_ISDIR(path_stat.st_mode)) {
        char error_message[256] = {0};
        sprintf(error_message, "Got path to a directory! Cannot give size in bytes!  Path=\"%s\"", path);
        PF_LOG_ERROR(PF_OS, error_message);
        return (size_t)-1;
    }
    
    return path_stat.st_size;
}

size_t pf_os_file_size_pstr_linux(PString_t const pstr) {
    return pf_os_file_size_cstr_linux(pstr.string, pstr.length);
}

size_t pf_os_path_join_cstr(char *out_path_buffer, size_t const path_buffer_size, char const * first_path,
    size_t const first_length, char const * second_path, size_t const second_length)
{
    if (out_path_buffer == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to out_path_buffer!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (path_buffer_size == 0) {
        PF_LOG_ERROR(PF_OS, "Buffer size has zero length!");
        return PFEC_ERROR_INVALID_LENGTH;
    }
    if (first_path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to first_path param!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (first_length == 0) {
        PF_LOG_ERROR(PF_OS, "Zero length for first_length param!");
        return PFEC_ERROR_INVALID_LENGTH;
    }
    if (second_path == NULL) {
        PF_LOG_ERROR(PF_OS, "Null ptr to second_path param!");
        return PFEC_ERROR_NULL_PTR;
    }
    if (second_length == 0) {
        PF_LOG_ERROR(PF_OS, "Zero length for second_length param!");
        return PFEC_ERROR_INVALID_LENGTH;
    }

    // calculate copy_length
    size_t const nul_terminator_length = 1;
    size_t const delimiter_length = 1;
    size_t const copy_length = first_length + delimiter_length + second_length + nul_terminator_length;

    if (path_buffer_size < copy_length) {
        PF_LOG_ERROR(PF_OS, "Out Buffer is not big enough to create joined path!");
        return PFEC_ERROR_NOT_ENOUGH_MEMORY;
    }

    // zero the out buffer
    memset(out_path_buffer, 0, copy_length);

    int32_t bHasForwardSlash = FALSE;
    for (size_t i = 0; i < first_length; i++) {
        if (first_path[i] == '/') {
            bHasForwardSlash = TRUE;
            break;
        }
        if (first_path[i] == '\\') {
            bHasForwardSlash = FALSE;
            break;
        }
    }

    // figure oout which slash to use as the delimiter
    char const delimiter = bHasForwardSlash ? '/' : '\\';

    snprintf(out_path_buffer, copy_length, "%s%c%s", first_path, delimiter, second_path);

    return copy_length;
}



