// paciFIST studios. 2025. MIT license

#ifndef PATH_H
#define PATH_H

// stdlib
#include <stdint.h>
#include <stddef.h>
// framework
// engine
#include <string/pstring.h>


/**
 * @brief Returns TRUE(1) if this path can be found on disk, else FALSE(0)
 *
 * @param path
 * @param length
 * @return 
 */
int32_t pf_os_path_exists_cstr_linux(char const * path, size_t length);
int32_t pf_os_path_exists_pstr_linux(PString_t pstr);


/**
 * @brief returns TRUE(1) if this is a path to a file, else FALSE(0)
 *
 * @param path 
 * @param length
 * @return 
 */
int32_t pf_os_path_is_file_cstr_linux(char const * path, size_t length);
int32_t pf_os_path_is_file_pstr_linux(PString_t pstr);

/**
 * @brief Returns TRUE(1) if this is a path to a directory, else FALSE(0)
 *
 * @param path 
 * @param length
 * @return 
 */
int32_t pf_os_is_directory_cstr_linux(char const * path, size_t length);
int32_t pf_os_is_directory_pstr_linus(PString_t pstr);

/**
 * @brief Returns the size of this file on disk, and writes error if there is no file
 *
 * @param path 
 * @param length
 * @return 
 */
size_t pf_os_file_size_cstr_linux(char const * path, size_t length);
size_t ps_os_file_size_pstr_linus(PString_t pstr);


#endif //PATH_H
