// paciFIST studios. 2025. MIT license

#ifndef PATH_H
#define PATH_H

//#include <sys/stat.h>
//
//#include "../core/define.h"
//
//static bool32 path_exists(const char* path) {
//    // is it null?
//    if (path == NULL) {
//        return FALSE;
//    }
//
//    struct stat path_stat;
//    // returns non-zero on error
//    if (stat(path, &path_stat) != 0) {
//        return FALSE;
//    }
//
//    // it is either a file or a directory
//    return TRUE;
//}
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


#endif //PATH_H
