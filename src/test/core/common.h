// paciFIST Studios. 2025.  MIT License.

#ifndef COMMON_H
#define COMMON_H

#include <cstdio>

#include <SDL3/SDL.h>

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

#define BUILD_SETTING_PRINT_DEBUG 1 
#ifdef BUILD_SETTING_PRINT_DEBUG
// Prints message to stdout, along with file and line numbers
#define print_debug(format, ...) do { printf("%s(%d):\t" format, __FILE__, __LINE__, ##__VA_ARGS__); } while(0);
#else
#define print_debug(format, ...)
#endif



#endif //COMMON_H
