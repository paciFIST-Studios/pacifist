// paciFIST studios. 2025. MIT License

// header
#include <os/path.h>
// stdlib
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

int32_t pf_os_is_directory_cstr_linux(char const *path, size_t length) {
    return -1;
}

int32_t pf_os_is_directory_pstr_linus(PString_t pstr) {
    return -1;
}


//
//
//static bool32 directory_exists(const char* dirPath) {
//    if (dirPath == NULL) {
//        return FALSE;
//    }
//
//    struct stat path_stat;
//    // returns non-zero on error
//    if (stat(dirPath, &path_stat) != 0) {
//        return FALSE;
//    }
//
//    // is it a directory?
//    return S_ISDIR(path_stat.st_mode);
//}
//
//static bool32 file_exists(const char* filePath) {
//    if (filePath == NULL) {
//        return FALSE;
//    }
//
//    struct stat path_stat;
//    // returns non-zero on error
//    if (stat(filePath, &path_stat) != 0) {
//        return FALSE;
//    }
//
//    // is it a regular file?
//    return S_ISREG(path_stat.st_mode);
//}
