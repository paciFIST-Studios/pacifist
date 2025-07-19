// paciFIST studios. 2025. MIT License

#ifndef PSTRING_H
#define PSTRING_H

// stdlib
#include <stddef.h>
#include <stdint.h>
// framework
// engine


typedef struct PString_t {
    char * string;
    size_t length;
} PString_t;


// thought:  basically make a string arena, except make it a free list allocator,
//           so strings can either be interned for the duration of the program,
//           or they can be allocated in a free-list, for short-term usage
//typedef struct StringAllocatorSingleton_t {
//    
//} StringAllocatorSingleton_t;



int32_t pstring_contains_char_sub(PString_t const a, char const * substring, size_t const length);


// returns 1 is A contains B
// returns 0 otherwise
int32_t pstring_contains_pstr_sub(PString_t const a, PString_t const b);




#endif //PSTRING_H
