// paciFIST studios. 2025. MIT License

// stdlib
// framework
// engine
#include "error.h"
// game


#define ERROR_BUFFER_SIZE 4096


static char s_error_buffer[ERROR_BUFFER_SIZE];


size_t get_error_buffer_size() {
    return ERROR_BUFFER_SIZE;
}


char* get_error() {
    return s_error_buffer;
}


void set_error(char const * message, size_t const message_len) {
    if (message == NULL) {
        return;
    }

    for (size_t i = 0; i < message_len; i++) {
        if(i >= ERROR_BUFFER_SIZE){
            return;
        }

        s_error_buffer[i] = message[i];
    }
}


void clear_error() {
    for (size_t i = 0; i < ERROR_BUFFER_SIZE; i++) {
        s_error_buffer[i] = 0;
    }
}




