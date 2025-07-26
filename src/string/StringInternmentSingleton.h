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
 * StringInternmnetSingleton_t.  This will check to see if the string is
 * already present.  if it is, the new copy will not be stored, and
 * instead, a pointer will be returned to the existing copy.
 *
 * In addition, the internment singleton contains some functionality for
 * looking up a string, when given a ptr, and looking up a ptr when
 * given a stirng
 *
 * The goal, is to keep all game strings stored in continguous memory together.
 * In this version, there is not functionality for unloading strings,
 * except during the program shutdown stage
 *
 * strings are expected to be immutable, once stored
 *
 */
typedef struct StringInternmentSingleton_t {
    void * memory_base;
    size_t owned_memory_size;
    size_t used_memory_size;

    void * intern_string_fn;
    void * lookup_from_string_fn;
    void * lookup_from_ptr_fn;

} StringInternmentSingleton_t;




#endif //STRING_INTERNMENT_SINGLETON_H
