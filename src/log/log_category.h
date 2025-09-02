// paciFIST studios. 2025. MIT License

#ifndef LOG_CATEGORY_H
#define LOG_CATEGORY_H

// stdlib
// framework
#include <SDL3/SDL_log.h>
// engine

typedef enum PFLogCategory_t {
    PF_APPLICATION = SDL_LOG_CATEGORY_CUSTOM, /* this one should always come first, even if it's not alphabetical */
    PF_ALLOCATOR,
    PF_CONFIGURATION,
    PF_ERROR_ERROR, /* an error in the error system itself */
    PF_FONT,
    PF_INITIALIZATION,
    PF_OS,
    PF_RESOURCE,
    PF_STRING,
    PF_TEXTURE,
} PFLogCategory_t;


#endif //LOG_CATEGORY_H
