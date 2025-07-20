// paciFIST Studios. 2025. MIT License

#ifndef ERROR_H
#define ERROR_H

// stdlib
#include <stddef.h>
#include <stdint.h>
#include <string.h>
// framework
// engine
#include "../date/datetime.h"
// game







/**
 * @brief this macro builds an error message, and stores it in the error message buffer.  Retrieve with pf_get_error()
 * 
 * @param message 
 */
#define BUILD_AND_SET_ERROR_MESSAGE(message) do {               \
    size_t const size = pf_get_error_buffer_size();             \
    char error_message[size];                                   \
    for(size_t i = 0; i < size; i++){ error_message[i] = 0; }   \
    char dt[22];                                                \
    get_datetime_string(dt, 22);                                \
    sprintf(error_message, "%s:  ERROR %s  --  file: %s[%d]",   \
         dt, message, __FILE__, __LINE__);                      \
    pf_set_error(error_message, strlen(error_message));         \
} while(0);                                                     \



// gets the current allocation size of the error buffer
size_t pf_get_error_buffer_size();

// gets ptr to the start of the error buffer
char* pf_get_error();

/**
 *  @brief Sets an error message in the error buffer
 *
 * @param message 
 * @param message_len 
 */
void pf_set_error(char const * message, size_t const message_len);

// clears out any information from the error buffer
void pf_clear_error();


void pf_set_error_suppressed();

void pf_set_error_not_suppressed();

// if errors are suppressed, calling get_error will return nullptr
int32_t pf_get_is_error_suppressed();


#endif //ERROR_H
