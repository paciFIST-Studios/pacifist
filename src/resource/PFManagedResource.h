// paciFIST studios. 2025. MIT License.

#ifndef PF_MANAGED_RESOURCE_H
#define PF_MANAGED_RESOURCE_H

// stdlib
#include <stdint.h>
// framework
// engine
#include "project_data_types.h"


typedef struct PFManagedResource_t {
    EProjectDataTypes_t data_type;
    uint32_t size;
    void* data;
} PFManagedResource_t;


#endif //PF_MANAGED_RESOURCE_H