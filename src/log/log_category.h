// paciFIST studios. 2025. MIT License

#ifndef LOG_CATEGORY_H
#define LOG_CATEGORY_H

// stdlib
// framework
#include <SDL3/SDL_log.h>
// engine

typedef enum PFLogCategory_t {
    PF_APPLICATION = SDL_LOG_CATEGORY_CUSTOM,
    PF_OS,
} PFLogCategory_t;


#endif //LOG_CATEGORY_H
