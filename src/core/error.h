// paciFIST Studios. 2025. MIT License

#ifndef ERROR_H
#define ERROR_H

// stdlib
#include <stddef.h>
// framework
// engine
// game


// gets the current allocation size of the error buffer
size_t get_error_buffer_size();

// gets ptr to the start of the error buffer
char* get_error();

/**
 *  @brief Sets an error message in the error buffer
 *
 * @param message 
 * @param message_len 
 */
void set_error(char const * message, size_t const message_len);

// clears out any information from the error buffer
void clear_error();


#endif //ERROR_H
