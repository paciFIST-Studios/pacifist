// paciFIST studios. 2025. MIT License

// stdlib
// framework
// engine
#include "error.h"

#include "memory/hash_table/HashTable.h"
// game

// comment out this definition to turn off error suppression, and print errors
// they'll also show up during the tests, so don't be alarmed
#define ERROR_SUPPRESSION_ALLOWED


#define ERROR_BUFFER_SIZE 4096



static int32_t s_errors_are_suppressed = 0;


static char s_error_buffer[ERROR_BUFFER_SIZE];


size_t pf_get_error_buffer_size() {
    return ERROR_BUFFER_SIZE;
}


char* pf_get_error() {
    if (s_errors_are_suppressed) {
        return NULL;
    }
        
    return s_error_buffer;
}


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


void pf_clear_error() {
    for (size_t i = 0; i < ERROR_BUFFER_SIZE; i++) {
        s_error_buffer[i] = 0;
    }
}



void pf_set_error_suppressed() {
#ifdef ERROR_SUPPRESSION_ALLOWED
    s_errors_are_suppressed = TRUE;
#endif
}

void pf_set_error_not_suppressed() {
#ifdef ERROR_SUPPRESSION_ALLOWED
    s_errors_are_suppressed = FALSE;
#endif
}

int32_t pf_get_is_error_suppressed() {
#ifdef ERROR_SUPPRESSION_ALLOWED
    return s_errors_are_suppressed;
#else
    return FALSE;
#endif
}

void pf_build_and_set_error_message(char const * message, char const * file, int32_t const line) {
    char error_message[ERROR_BUFFER_SIZE];
    for(size_t i = 0; i < ERROR_BUFFER_SIZE; i++){ error_message[i] = 0; }
    char dt[22];
    get_datetime_string(dt, 22);
    sprintf(error_message, "%s:  ERROR %s  --  file: %s[%d]\n", dt, message, file, line);
    pf_set_error(error_message, strlen(error_message));
}



void pf_log_verbose(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = pf_strlen(error);
        SDL_LogVerbose((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_warning(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = pf_strlen(error);
        SDL_LogWarn((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_error(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = pf_strlen(error);
        SDL_LogError((SDL_LogCategory)category, error, error_length);
    }
}

void pf_log_critical(PFLogCategory_t const category, char const * message, char const * file, int32_t const line) {
    pf_build_and_set_error_message(message, file, line);
    if (!pf_get_is_error_suppressed()) {
        char const * error = pf_get_error();
        size_t const error_length = pf_strlen(error);
        SDL_LogCritical((SDL_LogCategory)category, error, error_length);
    }
}


