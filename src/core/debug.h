// paciFIST studios. 2025. MIT License

#ifndef DEBUG_H
#define DEBUG_H

// include
// stdlib
// framework
// engine
#include "error.h"

#define DEBUG_PRINT_BITS(x) do {           \
    printf("Printing debug bit output:\t");\
    size_t const type_size = sizeof(x);    \
    size_t const bits = type_size * 8;     \
    for(size_t i = 0; i < bits; i++){      \
        if ((x >> i) & 1){ printf("1"); }  \
        else { printf("0"); }              \
    }                                      \
    printf("\n");                          \
} while (0);                               \


#endif //DEBUG_H
