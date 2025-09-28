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
int32_t pf_os_path_is_directory_cstr_linux(char const * path, size_t length);
int32_t pf_os_path_is_directory_pstr_linux(PString_t pstr);

/**
 * @brief Returns the size of this file on disk, and writes error if there is no file
 *
 * @param path 
 * @param length
 * @return 
 */
size_t pf_os_file_size_cstr_linux(char const * path, size_t length);
size_t pf_os_file_size_pstr_linux(PString_t pstr);


/**
 * 
 * @param out_path_buffer 
 * @param first 
 * @param first_length 
 * @param second 
 * @param second_length 
 * @return 
 */
int32_t pf_os_path_join2(
    char * out_path_buffer,
    char const * first,
    size_t first_length,
    char const * second,
    size_t second_length);

/**
 * @brief joins to cstr to create a path.
 * @note: will check to see if / or \ is used, and use the first found as a delimiter between the two cstr
 *
 * @param out_path_buffer
 * @param path_buffer_size
 * @param first_path 
 * @param first_length 
 * @param second_path 
 * @param second_length 
 * @return 
 */
size_t pf_os_path_join_cstr(
    char * out_path_buffer,
    size_t path_buffer_size,
    char const * first_path,
    size_t first_length,
    char const * second_path,
    size_t second_length);

/**
 * @brief joins two pstr, and adds a delimiter between them.  Uses / or \ as delim, depening on which is present in the first string
 *
 * @param out_path_buffer 
 * @param first 
 * @param second 
 * @return 
 */
size_t pf_os_path_join_pstr(
    PString_t out_path_buffer,
    PString_t first,
    PString_t second);



#endif //PATH_H
