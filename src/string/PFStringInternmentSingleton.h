// paciFIST studios. 2025. MIT License

#ifndef STRING_INTERNMENT_SINGLETON_H
#define STRING_INTERNMENT_SINGLETON_H

// stdlib
// framework
// engine
#include "pstring.h"


/**
 * the idea of string internment here, is to make the singleton at the base
 * of program memory, or as a static variable in main.c;  Then, any time 
 * someone wants to use a string, they can store it in the 
 * StringInternmentSingleton_t.  This will check to see if the string is
 * already present.  if it is, the new copy will not be stored, and
 * instead, a pointer will be returned to the existing copy.
 *
 * In addition, the internment singleton contains some functionality for
 * looking up a string, when given a ptr, and looking up a ptr when
 * given a string
 *
 * The goal, is to keep all game strings stored in contiguous memory together.
 * In this version, there is no functionality for unloading strings,
 * except during the program shutdown stage
 *
 * strings are expected to be immutable, once stored
 *
 */


#define PFSLI_MAX_STRINGS 254

// 120250727 - struct is 4096 bytes right now
typedef struct PFStringLifetimeInternmentSingleton_t {
    void * usable_memory_base;
    size_t owned_memory_size;
    size_t used_memory_size;
    uint8_t next_unused_idx;

    PString_t strings[PFSLI_MAX_STRINGS];
} PFStringLifetimeInternmentSingleton_t;


PFStringLifetimeInternmentSingleton_t* pf_string_lifetime_internment_create_with_memory(
    void* memory_base,
    size_t memory_size);


/**
 * NOTE: when copying string over, string_internment will use 1 byte
 *          to add a nul-terminator for the interned string
 * 
 * NOTE: on error, fn returns a PString_t with a null string,
 *          and the error code placed in PString_t.length.
 *          The error is also logged
 * 
 * @param string_internment 
 * @param cstring 
 * @param length 
 * @return 
 */
PString_t pf_string_lifetime_internment_emplace_cstr(
    PFStringLifetimeInternmentSingleton_t * string_internment,
    char const * cstring,
    size_t length);

/**
 * 
 * 
 * NOTE: when copying string over, string_internment will use 1 byte
 *          to add a nul-terminator for the interned string
 * 
 * NOTE: on error, fn returns a PString_t with a null string,
 *          and the error code placed in PString_t.length.
 *          The error is also logged
 * 
 * @param string_internment 
 * @param pstring 
 * @return 
 */
PString_t pf_string_lifetime_internment_emplace_pstr(
    PFStringLifetimeInternmentSingleton_t * string_internment,
    PString_t pstring);


#endif //STRING_INTERNMENT_SINGLETON_H
