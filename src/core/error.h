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
#include "../log/log_category.h"
// game




// Logging Macros ------------------------------------------------------------------------------------------------------

// stdlib
// framework
// engine
#include "../core/error.h"
#include "../log/log_category.h"

// iirc, this is the strlen implementation
static inline size_t pf_strlen(char const * string) {
    size_t count = 0;
    while (string != NULL && *string++ != 0) {
        count++;
    }
    return count;
}



/**
 *  @brief this macro builds and logs a VERBOSE message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_VERBOSE(Category, Message) do {                          \
    BUILD_AND_SET_ERROR_MESSAGE(Message);                               \
    if(dnc__pf_get_is_error_suppressed()) { break; }                    \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogVerbose((SDL_LogCategory)Category, error, error_length );    \
} while (0);                                                            \

/**
 *  @brief this macro builds and logs a WARNING message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_WARNING(Category, Message) do {                          \
    BUILD_AND_SET_ERROR_MESSAGE(Message);                               \
    if(dnc__pf_get_is_error_suppressed()) { break; }                    \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogWarning((SDL_LogCategory)Category, error, error_length );      \
} while (0);                                                            \



/**
 *  @brief this macro builds and logs an ERROR message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_ERROR(Category, Message) do {                            \
    BUILD_AND_SET_ERROR_MESSAGE(Message);                               \
    if(dnc__pf_get_is_error_suppressed()) { break; }                    \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogError((SDL_LogCategory)Category, error, error_length );      \
} while (0);                                                            \

/**
 *  @brief this macro builds and logs a CRITICAL message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_CRITICAL(Category, Message) do {                         \
    BUILD_AND_SET_ERROR_MESSAGE(Message);                               \
    if(dnc__pf_get_is_error_suppressed()) { break; }                    \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogCritical((SDL_LogCategory)Category, error, error_length );   \
} while (0);                                                            \



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


#define PF_SUPPRESS_ERRORS do {                                 \
    dnc__pf_set_error_suppressed();                             \
}while(0);                                                      \


#define PF_UNSUPPRESS_ERRORS do {                               \
    dnc__pf_set_error_not_suppressed();                         \
} while(0);                                                     \

// Error -----------------------------------------------------------------------------------------------------


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


/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
void dnc__pf_set_error_suppressed();

/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
void dnc__pf_set_error_not_suppressed();

// if errors are suppressed, calling get_error will return nullptr
/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
int32_t dnc__pf_get_is_error_suppressed();

/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
int32_t dnc__pf_is_error_suppression_balanced();

/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
char* dnc__pf_get_error_suppression_unbalanced_message();

#endif //ERROR_H
