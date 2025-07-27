// paciFIST studios. 2025. MIT License

#ifndef DATETIME_H
#define DATETIME_H

// stdlib
#include <stdint.h>
#include <stddef.h>
// framework
// engine


/**
 * @brief Returns date time in the form: 120250719T131700(-07), and requires 22 char 
 *
 * @param out_buffer 
 * @param out_buffer_length 
 * @return 
 */
int32_t get_datetime_string(char * out_buffer, size_t const out_buffer_length);


#endif //DATETIME_H
