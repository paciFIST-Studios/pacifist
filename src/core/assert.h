// paciFIST studios. 2025. MIT License.

#ifndef ASSERT_H
#define ASSERT_H

// stdlib
#include <stdio.h>

#define BUILD_SETTING_ALLOW_ASSERTS 1

#ifdef BUILD_SETTING_ALLOW_ASSERTS
static inline void pf_do_assertion(char const * fn, char const * file, int const line) {
    fprintf(stderr, "ASSERTION FAILED! fn: %s, file: %s, line: %d", fn, file, line);
    // force a crash using null deref
    *(int*)0 = 0;
}
#define PF_ASSERT(expression) do { if(!expression){ pf_do_assertion(__func__, __FILE__, __LINE__); } } while(0);
#else
#define PF_ASSERT(expression);
#endif

#ifdef BUILD_SETTING_ALLOW_ASSERTS
static inline void pf_do_assertion_message(char const * message, char const * fn, char const * file, int line) {
    fprintf(stderr, "ASSERTION FAILED!  %s  fn: %s, file: %s, line: %d", message, fn, file, line);
    // force a crash using null deref
    *(int*)0 = 0;
}

// Asserts that the condition is true.  If it is not, this message is printed to stderr, along with the function, file, and line 
#define PF_ASSERT_MESSAGE(expression, message) \
do { if(!expression){ pf_do_assertion_message(message, __func__, __FILE__, __LINE__); } } while(0);
#else

#endif

#endif //ASSERT_H
