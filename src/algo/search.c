// paciFIST studios. 2025. MIT License

// header
#include "search.h"

// stdlib
// framework
// project

int32 compare_int32(const void * a, const void * b) {
    // cast to int*, deref, subtract
    // positive result means a is bigger,
    // zero means they're equal
    // negative result means b is bigger
    return *(int32 *)a - *(int32 *)b;
}
