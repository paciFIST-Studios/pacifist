// paciFIST studios. 2025. MIT License

#ifndef DEFINES_H
#define DEFINES_H

// Common Types --------------------------------------------------------------------------------------------------------

// use of LongLong (LL) indicates to most compilers that this is 64 bit.
// if the number is small enough, we can still assign it to an int32
#define Kibibytes(value) ((value) * 1024LL)
#define Mebibytes(value) (Kibibytes(value) * 1024LL)
#define Gibibytes(value) (Mebibytes(value) * 1024LL)
#define Tebibytes(value) (Gibibytes(value) * 1024LL)


#define TRUE 1
#define FALSE 0

// Constant Values -----------------------------------------------------------------------------------------------------


// The value of PI, in 32 bits
#define PI_32 = 3.14159265359f
// The value of Tau (2PI) in 32 bits
#define TAU_32 = 6.28318530718f 

// 2 / 65535
#define TWO_OVER_INT16 0.000030518f 




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
    if(pf_get_is_error_suppressed()) { break; }                         \
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
    if(pf_get_is_error_suppressed()) { break; }                         \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogWarning((SDL_LogCategory)Category, error, error_length );    \
} while (0);                                                            \


/**
 *  @brief this macro builds and logs an ERROR message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_ERROR(Category, Message) do {                            \
    BUILD_AND_SET_ERROR_MESSAGE(Message);                               \
    if(pf_get_is_error_suppressed()) { break; }                         \
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
    if(pf_get_is_error_suppressed()) { break; }                         \
    char const * error = pf_get_error();                                \
    size_t const error_length = pf_strlen(error);                       \
    SDL_LogCritical((SDL_LogCategory)Category, error, error_length );   \
} while (0);                                                            \



#endif //DEFINES_H
