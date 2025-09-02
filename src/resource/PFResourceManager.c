// paciFIST studios. 2025. MIT License

// inlude
#include "PFResourceManager.h"

// stdlib
// framework
// engine
#include "../core/common.h"


int32_t pf_resource_manager_initialize(void * memory, PFResourceManager_t * resource_manager) {
    if (memory == NULL) {
        return FALSE;
    }
    
    return TRUE;
}

int32_t pf_resource_manager_register_resource_with_path(PString_t const path,
    int32_t const bAutoLoadResource) {
    return TRUE;
}

int32_t pf_resource_manager_load_resource_with_name(PString_t const resource_name) {
    return TRUE;
}

void * pf_resource_manager_get_resource_with_name(PString_t const resource_name) {
    return NULL;
}

int32_t pf_resource_manager_unload_resource_with_name(PString_t const resource_name) {
    return TRUE;
}

int32_t pf_resource_manager_unregister_resource_with_name(PString_t const resource_name) {
    return TRUE;
}
