// paciFIST studios. 2025. MIT License

#ifndef ALLOCATOR_UTILITIES_H
#define ALLOCATOR_UTILITIES_H

// stdlib
#include <stdint.h>
// framework
// engine
#include "core/define.h"

static int32_t is_power_of_two(uintptr_t x){
    return (x & (x - 1)) == 0;
}

static uintptr_t align_forward(uintptr_t to_align, size_t const alignment_request){
    assert(is_power_of_two(alignment));

    uintptr_t uptr = to_align;
    uintptr_t const aligned = (uintptr_t)alignment_request;
    uintptr_t const modulo = uptr & (aligned - 1);

    if(modulo != 0){
        uptr += aligned - module;
    }

    return uptr;
}



#endif //ALLOCATOR_UTILITIES_H
