// paciFIST studios. 2025. MIT License

// header
#include <core/error.h>
// stdlib
#include <stdio.h>
// framework
#include <SDL3/SDL_error.h>
// engine
#include <core/define.h>
#include <date/datetime.h>
// game

// comment out this definition to turn off error suppression, and print errors
// they'll also show up during the tests, so don't be alarmed
#define ERROR_SUPPRESSION_ALLOWED

//char const g_error_message__null_ptr[] = "Null ptr to %s param!";
//char const g_error_message__invalid_length[] = "Invalid length for %s param!";
//char const g_error_message__file_does_not_exist[] = "File does not exist! path=\"%s\"";
//char const g_error_message__could_not_get_time[] = "Could not get system time!";
//char const g_error_message__out_of_memory[] = "Out of memory! %s";
//char const g_error_message__out_of_bounds_memory_use[] = "Attempted out of bounds memory use! %s";



#define ERROR_BUFFER_SIZE 4096

// set to TRUE(1) if suppressed, FALSE(0) if not suppressed
static int32_t s_errors_are_suppressed = 0;

// This error buffer is meant to hold only one error at a time,
// but this is enough for the tests, which create specific
// circumstances in order to induce errors, and check to make sure
// they're written out correctly
static char s_error_buffer[ERROR_BUFFER_SIZE];

/**
 * @brief returns the size of the error buffer, in bytes
 *
 * @return 
 */
size_t pf_get_error_buffer_size(void) {
    return ERROR_BUFFER_SIZE;
}

/**
 * @brief returns a pointer to the start of the error buffer 
 *
 * @return 
 */
char* pf_get_error(void) {
    if (s_errors_are_suppressed) {
        return NULL;
    }

    return s_error_buffer;
}

/**
 *  @brief zeroes out the error buffer
 */
void pf_clear_error(void) {
    for (size_t i = 0; i < ERROR_BUFFER_SIZE; i++) {
        s_error_buffer[i] = 0;
    }
}

/**
 * @brief places a message in the error buffer
 *
 * @param message 
 * @param message_len 
 */
void pf_set_error(char const * message, size_t const message_len) {
    if (message == NULL) {
        return;
    }

    pf_clear_error();

    for (size_t i = 0; i < message_len; i++) {
        if(i >= ERROR_BUFFER_SIZE){
            return;
        }

        s_error_buffer[i] = message[i];
    }
}

/**
 * @brief if errors are suppress, they'll still be written to the
 * error buffer, but you won't be able to access them, until errors
 * are no longer suppressed
 */
void pf_set_error_suppressed(void) {
#ifdef ERROR_SUPPRESSION_ALLOWED
    s_errors_are_suppressed = TRUE;
#endif
}

/**
 * @brief sets errors to not-suppressed
 */
void pf_set_error_not_suppressed(void) {
#ifdef ERROR_SUPPRESSION_ALLOWED
    s_errors_are_suppressed = FALSE;
#endif
}

/**
 * @brief returns TRUE(1) if errors are suppressed now, otherwise FALSE(0)
 * 
 * @return 
 */
int32_t pf_get_is_error_suppressed(void) {
#ifdef ERROR_SUPPRESSION_ALLOWED
    return s_errors_are_suppressed;
#else
    return FALSE;
#endif
}

/**
 * @brief takes an error message, adds additional debug information,
 * and puts it in the error buffer
 *
 * @param message 
 * @param file 
 * @param line 
 */
void pf_build_and_set_error_message(char const * message, char const * file, int32_t const line) {
    char error_message[ERROR_BUFFER_SIZE] = {0};
    char dt[22];
    get_datetime_string(dt, 22);
    sprintf(error_message, "%s:  ERROR %s  --  file: %s[%d]\n", dt, message, file, line);
    pf_set_error(error_message, strlen(error_message));
}

void pf_log_verbose(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = strlen(error);
        SDL_LogVerbose((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_warning(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = strlen(error);
        SDL_LogWarn((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_error(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = strlen(error);
        SDL_LogError((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_critical(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = strlen(error);
        SDL_LogCritical((SDL_LogCategory)category, error, error_length);
    }
}


