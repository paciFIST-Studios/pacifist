// paciFIST studios. 2025. MIT License

#ifndef OS_UTILITY_H
#define OS_UTILITY_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine



/**
 * @brief returns the size of this file in bytes, -1 on error
 *  
 * @param file_path
 * @param file_path_length
 * @return 
 */
int64_t file_size(const char * file_path, size_t const file_path_length);

/**
 * @brief returns 1 if path is a file, 0 if not, -1 on error
 *
 * @param file_path
 * @param file_path_length
 * @return
 */
int32_t is_file(const char * file_path, size_t const file_path_length);


/**
 * @brief returns 1 if path is a directory, 0 if not, -1 on error
 *
 * @param file_path
 * @param file_path_length
 * @return
 */
int32_t is_directory(const char * file_path, size_t const file_path_length);







#endif //OS_UTILITY_H
