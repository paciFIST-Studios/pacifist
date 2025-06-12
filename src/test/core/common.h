// paciFIST Studios. 2025.  MIT License.

#ifndef COMMON_H
#define COMMON_H


// Asserts -------------------------------------------------------------------------------------------------------------
#include <cstdio>

#define BUILD_SETTING_ALLOW_ASSERTS 1

#ifdef BUILD_SETTING_ALLOW_ASSERTS
inline void do_assertion(char const * fn, char const * file, int line) {
    fprintf(stderr, "ASSERTION FAILED! fn: %s, file: %s, line: %d", fn, file, line);
    // force a crash using null deref
    *(int*)0 = 0;
}
#define ASSERT(expression) do { if(!expression){ do_assertion(__func__, __FILE__, __LINE__); } } while(0);
#else
#define ASSERT(expression);
#endif

#ifdef BUILD_SETTING_ALLOW_ASSERTS
inline void do_assertion_message(char const * message, char const * fn, char const * file, int line) {
    fprintf(stderr, "ASSERTION FAILED!  %s  fn: %s, file: %s, line: %d", message, fn, file, line);
    // force a crash using null deref
    *(int*)0 = 0;
}

// Asserts that the condition is true.  If it is not, this message is printed to stderr, along with the function, file, and line 
#define ASSERT_MESSAGE(expression, message) \
    do { if(!expression){ do_assertion_message(message, __func__, __FILE__, __LINE__); } } while(0);
#else

#endif


// Print Debug ---------------------------------------------------------------------------------------------------------

#define BUILD_SETTING_PRINT_DEBUG 1 
#ifdef BUILD_SETTING_PRINT_DEBUG
// Prints message to stdout, along with file and line numbers
#define print_debug(format, ...) do { printf("%s(%d):\t" format, __FILE__, __LINE__, ##__VA_ARGS__); } while(0);
#else
#define print_debug(format, ...)
#endif


// Common Types --------------------------------------------------------------------------------------------------------

// use of LongLong (LL) indicates to most compilers that this is 64 bit.
// if the number is small enough, we can still assign it to an int32
#define Kibibytes(value) ((value) * 1024LL)
#define Mebibytes(value) (Kibibytes(value) * 1024LL)
#define Gibibytes(value) (Mebibytes(value) * 1024LL)
#define Tebibytes(value) (Gibibytes(value) * 1024LL)

#include <check_stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32_t bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define TRUE 1
#define FALSE 0

// more explicit terms for different uses of static
#define internal static
#define global_persist static
#define global_variable static

// The value of PI, in 32 bits
#define PI_32 = 3.14159265359f
// The value of Tau (2PI) in 32 bits
#define TAU_32 = 6.28318530718f 


// Counts the number of elements in an array
#define ARRAY_COUNT(array) ((sizeof(array))/(sizeof((array)[0])))  


// uses a loop to set array elements to zero, but    
// does not check to make sure your array elements   
// can actually be set to zero                       
#define ARRAY_ZERO_INITIALIZE(array) do {           \
    for(int _array_init_idx = 0;                    \
        _array_init_idx < (int)ARRAY_COUNT(array);  \
        _array_init_idx++)                          \
    {                                               \
        array[_array_init_idx] = 0;                 \
    }                                               \
} while(0);                                         \



#define TWO_OVER_SHORT 0.000030518f 


#endif //COMMON_H
