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

// PF Error Codes
static const int32_t PFEC_NO_ERROR = 0;
static const int32_t PFEC_ERROR_NULL_PTR = 10;
static const int32_t PFEC_ERROR_INVALID_LENGTH = 20;
static const int32_t PFEC_FILE_DOES_NOT_EXIST = 30;



// utility fns -----------------------------------------------------------------------------------------------

// iirc, this is the strlen implementation
static inline size_t pf_strlen(char const * string) {
    size_t count = 0;
    while (string != NULL && *string++ != 0) {
        count++;
    }
    return count;
}



// Logging Macros --------------------------------------------------------------------------------------------

/**
 *  @brief this macro builds and logs a VERBOSE message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_VERBOSE(Category, Message) do {              \
    pf_log_verbose(Category, Message, __FILE__, __LINE__);  \
} while (0);                                                \

/**
 *  @brief this macro builds and logs a WARNING message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_WARNING(Category, Message) do {              \
    pf_log_warning(Category, Message, __FILE__, __LINE__);  \
} while (0);                                                \



/**
 *  @brief this macro builds and logs an ERROR message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_ERROR(Category, Message) do {               \
    pf_log_error(Category, Message, __FILE__, __LINE__);   \
} while (0);                                               \

/**
 *  @brief this macro builds and logs a CRITICAL message to SDL_Log, using the give category and message
 *
 * @param Category PFLogCategory_t
 * @param Message 
 */
#define PF_LOG_CRITICAL(Category, Message) do {            \
    pf_log_error(Category, Message, __FILE__, __LINE__);   \
} while (0);                                               \



/**
 * @brief this macro builds an error message, and stores it in the error message buffer.  Retrieve with pf_get_error()
 * 
 * @param message 
 */


#define PF_SUPPRESS_ERRORS do {                           \
    pf_set_error_suppressed();                            \
}while(0);                                                \


#define PF_UNSUPPRESS_ERRORS do {                         \
    pf_set_error_not_suppressed();                        \
} while(0);                                               \



/**
 *
 *  NOTES:
 *      Okay, so this is clearly two tasks, therefore, let me explain why it's in one fn.
 *
 *      1. error.c owns the singleton static error buffer.  Placing an error in this buffer,
 *          simply means copying each character over
 *
 *      2. building the message happens exactly the same way to every message which is logged.
 *          prefixed by date, followed by verbosity type, followed by message, followed by
 *          caller's file, followed by calling line number
 *
 *      3. in order to transport this data from one fn to the next, we'll need an additional
 *          buffer, of size ERROR_BUFFER_SIZE, which is 4096 bytes.  Right now, we're building
 *          it on the stack, but also cleaning it up at the end of the function.
 *
 *          in order to return this to the caller, we would have to allocate memory, or do
 *          yet more stack allocations of this size, and yet more copies of this size
 *
 *      4. combing these two tasks, occurs in part as an optimization--we don't want to
 *          introduce a stack overflow.  But primarily, this is done to prevent heap
 *          allocation.  Our game engine is only meant to have a single allocation at
 *          program start, and thereafter, allocate from that memory, using pf_allocators
 *          of some kind.  Not only is this work unfinished at the time of this writing,
 *          but also we want to avoid a future maintainer's temptation to allocate memory
 *          for the error messages, as a way of reducing the code surface area which
 *          directly interacts with memory from the OS
 *
 *      5. it's not perfect, and we could either query the size of the buffer needed,
 *          or simply always make a buffer of ERROR_BUFFER_SIZE.  However, I (Ellie Barrett, 1202507231051)
 *          believe this is the best compromise of logical rigor, and practical necessity.
 *          I agree to suffer the consequences of this decision in the future.  Future
 *          maintainers can be assured, this is explicitly my decision/fault.  Sorry,
 *          if it's a hassle for you.
 *
 *
 * @param message 
 * @param file 
 * @param line 
 */
void pf_build_and_set_error_message(char const * message, char const * file, int32_t const line);


// Logging fns -----------------------------------------------------------------------------------------------

// these logging fns can be used directly, but there are some simple macro wrappers,
// which supply the file name and line number for you


/**
 * @brief Logs a message with a Verbose modifier (won't print out unless user is using verbose logging)
 *
 * @param category
 * @param message
 * @param file
 * @param line
 */
void pf_log_verbose(PFLogCategory_t const category, char const * message, char const * file, int32_t const line);


/**
 * @brief Logs a message with a Warning modifier
 * 
 * @param category
 * @param message
 * @param file
 * @param line
 */
void pf_log_warning(PFLogCategory_t const category, char const * message, char const * file, int32_t const line);


/**
 * @brief Logs a message with an Error modifier
 * 
 * @param category
 * @param message
 * @param file
 * @param line
 */
void pf_log_error(PFLogCategory_t const category, char const * message, char const * file, int32_t const line);


/**
 * @brief Logs a message with a Critical modifier
 * 
 * @param category
 * @param message
 * @param file
 * @param line
 */
void pf_log_critical(PFLogCategory_t const category, char const * message, char const * file, int32_t const line);

// Error state fns -------------------------------------------------------------------------------------------

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
void pf_set_error_suppressed();

/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
void pf_set_error_not_suppressed();

// if errors are suppressed, calling get_error will return nullptr
/**
 * @brief DO NOT CALL - this fn is used internally by the PF_SUPPRESS_ERRORS marco, use that instead 
 */
int32_t pf_get_is_error_suppressed();

#endif //ERROR_H
