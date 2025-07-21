// paciFIST studios. 2025. MIT License

#ifndef TYPED_POINTER_H
#define TYPED_POINTER_H

#include "project_data_types.h"


typedef union TypedPtr_t{
    struct {
        uint8_t type;
        uint8_t flags;
        uint16_t type_specific;
    };
    uint32_t data;

    void* pData;
    
} TypedPtr_t;


#endif //TYPED_POINTER_H
